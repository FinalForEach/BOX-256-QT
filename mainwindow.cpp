#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QHBoxLayout>
#include <QtMath>
#include <QTableWidget>
#include <QHeaderView>
#include <QButtonGroup>
#include <QPushButton>
#include <QFile>

#include "box256glwidget.h"
#include "tablecelledit.h"
static QString getHexNum(int i, int numLeadingZeros=0)
{
    QString hexNum = "";
    for(int x=1;x<=numLeadingZeros;x++)
    {
        if(i<qPow(16,x))
        {
            hexNum+="0";
        }
    }
    hexNum+=QString::number(i,16);
    return hexNum.toUpper();
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , dialogLoad(this)
    , playTimer()
{
    ui->setupUi(this);


    ui->gridLayout->addWidget(new QLabel("Source",this),0,0);
    ui->gridLayout->addWidget(new QLabel("Memory",this),0,1);
    ui->gridLayout->addWidget(new QLabel("Output",this),0,2);

    QTableWidget *srcTable = new QTableWidget(64,4,this);
    QTableWidget *memTable = new QTableWidget(64,4,this);
    Box256GLWidget* box256Widget = new Box256GLWidget(&machine);
    ui->gridLayout->addWidget(srcTable,1,0,Qt::AlignLeft);
    ui->gridLayout->addWidget(memTable,1,1,Qt::AlignLeft);
    ui->gridLayout->addWidget(box256Widget,1,2,1,2);

    QPushButton *stopButton = new QPushButton("Stop",this);
    QPushButton *stepButton = new QPushButton("Step",this);
    QPushButton *playButton = new QPushButton("Play",this);

    ui->gridLayout->addWidget(stopButton, 2,0);
    ui->gridLayout->addWidget(stepButton, 2,1);
    ui->gridLayout->addWidget(playButton, 2,2);

    connect(stopButton, SIGNAL(released()), this, SLOT(stopMachine()));
    connect(stepButton, SIGNAL(released()), this, SLOT(stepBtnReleased()));
    connect(playButton, SIGNAL(released()), this, SLOT(playMachine()));

    //Setup table labels
    QStringList srcColLabels = {"Cmd","A","B","C"};
    QStringList srcRowLabels = {};
    for(int i=0;i<256;i+=4)
    {
        QString lbl = "";
        lbl=getHexNum(i,1);
        srcRowLabels<<lbl;
    }
    srcTable->setVerticalHeaderLabels(srcRowLabels);
    srcTable->setHorizontalHeaderLabels(srcColLabels);

    QStringList memColLabels = {"","","",""};
    QStringList memRowLabels = {};
    for(int i=0;i<256;i+=4)
    {
        QString lbl = "";
        lbl=getHexNum(i,1);
        memRowLabels<<lbl;
    }
    memTable->setVerticalHeaderLabels(memRowLabels);
    memTable->setHorizontalHeaderLabels(memColLabels);

    //Setup table sizes
    for(int c=0;c<4;c++)
    {
        srcTable->setColumnWidth(c,30);
        memTable->setColumnWidth(c,10);

    }
    srcTable->setMaximumSize(190,16777215);
    memTable->setMaximumSize(190,16777215);

    //Setup source table
    for(int r=0;r<64;r++)
    {
        for(int c=0;c<4;c++)
        {
            auto cellText = new TableCellEdit(srcTable,r,c);
            cellTexts[r][c]=cellText;
            cellText->setPlainText(getHexNum(0,0));

            srcTable->setCellWidget(r,c,cellText);
            connect(cellText,SIGNAL(textChanged()),this,SLOT(on_srcCellChanged()));
        }
    }
    //Setup memory table
    for(BOXBYTE r=0;r<64;r++)
    {
        for(BOXBYTE c=0;c<4;c++)
        {
            auto data = machine.getValue(AccessMethod::ADDRESS,r*4 + c);
            auto cellText = new QLabel(getHexNum(data),memTable);
            memLabels[r][c] = cellText;
            cellText->setAlignment(Qt::AlignCenter);
            memTable->setCellWidget(r,c,cellText);
        }
    }
    memTable->setShowGrid(false);
    memTable->horizontalHeader()->hide();
    on_actionLoad_Example_Program_triggered();
}
void MainWindow::loadBoxFile(const QString& fileName)
{
    QFile boxFile(fileName);
    QTextStream boxStream(&boxFile);
    if(!boxFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Could not load box file.",
                             "Error: <b>"+ boxFile.errorString()+"</b>");
    }
    setMachineSource(boxStream);

    boxFile.close();
}
void MainWindow::setMachineSource(QTextStream& boxStream)
{
    int r=0;
    while(!boxStream.atEnd())
    {
        auto fields = boxStream.readLine().split(" ");
        if(r>=64)
        {
            QMessageBox::warning(this,"Could not load box file.",
                                 "The save file had too many lines to fit in the source table.<br/>"
                                 "Could only load <b>" + QString::number(r)+"</b> lines");
            break;
        }
        cellTexts[r][0]->setPlainText("0");
        cellTexts[r][1]->setPlainText("0");
        cellTexts[r][2]->setPlainText("0");
        cellTexts[r][3]->setPlainText("0");
        if(fields.length()>=1){
            cellTexts[r][0]->setPlainText(fields[0]);
        }
        if(fields.length()>=2){
            cellTexts[r][1]->setPlainText(fields[1]);
        }
        if(fields.length()>=3){
            cellTexts[r][2]->setPlainText(fields[2]);
        }
        if(fields.length()>=4){
            cellTexts[r][3]->setPlainText(fields[3]);
        }
        r++;
    }
}
void MainWindow::setMachineSource(QString& src)
{
    QTextStream boxStream(&src);
    setMachineSource(boxStream);
}
QString MainWindow::getMachineSource()
{
    QString srcCode ="";
    for(BOXBYTE r=0;r<64;r++)
    {
        auto cmdText = cellTexts[r][0]->toPlainText();
        auto pAText = cellTexts[r][1]->toPlainText();
        auto pBText = cellTexts[r][2]->toPlainText();
        auto pCText = cellTexts[r][3]->toPlainText();

        srcCode+=cmdText + " " + pAText+ " " + pBText+ " " + pCText + "\n";
    }
    return srcCode;

}
void MainWindow::stopMachine()
{
    machine.reset();
    playTimer.stop();
    updateMemoryLabels();
}
AccessMethod getAccessMethodFromSymbol(QChar c)
{
    if(c=='0' || c=='-')return AccessMethod::CONSTANT;
    if(c=='@')return AccessMethod::ADDRESS;
    if(c=='*')return AccessMethod::POINTER;
    return AccessMethod::NONE;
}
void MainWindow::stepBtnReleased()
{
    playTimer.stop();
    stepMachine();
}
void MainWindow::stepMachine()
{
    if(machine.curCycle==0)//Convert source to memory code on start.
    {
        for(BOXBYTE r=0;r<64;r++)
        {
            auto cmdText = cellTexts[r][0]->toPlainText();
            auto pAText = cellTexts[r][1]->toPlainText();
            auto pBText = cellTexts[r][2]->toPlainText();
            auto pCText = cellTexts[r][3]->toPlainText();
            //Add constant addressing in the case of a number that has no zero at the start.
            if(pAText[0]!='0' && pAText[0]!='@' && pAText[0]!='*'){pAText.insert(0,'0');}
            if(pBText[0]!='0' && pBText[0]!='@' && pBText[0]!='*'){pBText.insert(0,'0');}
            if(pCText[0]!='0' && pCText[0]!='@' && pCText[0]!='*'){pCText.insert(0,'0');}

            //Commutative expressions
            if(cmdText=="ADD" || cmdText == "MUL")
            {
                //Ensure that the constant parameter(if any) is second
                if(getAccessMethodFromSymbol(pAText[0])==AccessMethod::CONSTANT)
                {
                    auto tmp = pBText;
                    pBText = pAText;
                    pAText = tmp;
                }
                //Ensure address is after pointer if needed.
                if(getAccessMethodFromSymbol(pAText[0])==AccessMethod::ADDRESS
                        && getAccessMethodFromSymbol(pBText[0])==AccessMethod::POINTER)
                {
                    if((cmdText=="ADD" && getAccessMethodFromSymbol(pCText[0])==AccessMethod::POINTER)
                            || cmdText=="MUL")
                    {//Ensure address is second for MUL or if pointer is third for ADD to be true to spec.
                        auto tmp = pBText;
                        pBText = pAText;
                        pAText = tmp;
                    }
                }
            }
            //Algebraic expressions
            if(cmdText=="ADD"||cmdText=="SUB"||cmdText=="MUL"||cmdText=="DIV"||cmdText=="MOD")
            {
                if(getAccessMethodFromSymbol(pAText[0])==AccessMethod::CONSTANT&&
                        getAccessMethodFromSymbol(pBText[0])==AccessMethod::CONSTANT)
                {//Can be compiled into MOV.
                    BOXBYTE constA = pAText.toInt(nullptr,16);
                    BOXBYTE constB = pBText.toInt(nullptr,16);
                    BOXBYTE result = 0x0;
                    QString storeAddrStr = pCText;
                    if(cmdText=="ADD"){
                        result = constA + constB;
                    }
                    if(cmdText=="SUB"){
                        result = constA - constB;
                    }
                    if(cmdText=="MUL"){
                        result = constA * constB;
                    }
                    if(cmdText=="DIV"){
                        if(constB==0)result=0;
                        else result = constA / constB;
                    }
                    if(cmdText=="MOD"){
                        if(constB==0)result=0;
                        else result = constA % constB;
                    }

                    cmdText = "MOV";
                    pAText = getHexNum(result,2);
                    pBText = storeAddrStr;
                    pCText = "001";
                }
            }

            BOXBYTE op = machine.getOpcodeFromCommand(cmdText,
                getAccessMethodFromSymbol(pAText[0]),
                getAccessMethodFromSymbol(pBText[0]),
                getAccessMethodFromSymbol(pCText[0]));
            if(cmdText[0]=='0')//If number stored, do not write as opcode.
            {
                cmdText.remove(0,1);
                if(cmdText=="")cmdText="0";
                bool isCmdNum;
                BOXBYTE i = cmdText.toInt(&isCmdNum,16);
                if(isCmdNum)op=i;
            }
            //Get rid of addressing symbol
            pAText.remove(0,1);
            pBText.remove(0,1);
            pCText.remove(0,1);
            //Get values
            bool okA, okB, okC;
            BOXBYTE numA = static_cast<BOXBYTE>(pAText.toInt(&okA,16));
            BOXBYTE numB = static_cast<BOXBYTE>(pBText.toInt(&okB,16));
            BOXBYTE numC = static_cast<BOXBYTE>(pCText.toInt(&okC,16));
            if(!okA)numA=0;
            if(!okB)numB=0;
            if(!okC)numC=0;
            machine.writeValue(op,r*0x04 + 0x0);
            machine.writeValue(numA,r*0x04 + 0x1);
            machine.writeValue(numB,r*0x04 + 0x2);
            machine.writeValue(numC,r*0x04 + 0x3);
        }
    }
    if(machine.curCycle==0)
    {
        machine.curCycle+=1;
    }else
    {
        machine.step();
    }
    //Update memory table after step.
    updateMemoryLabels();
}
void MainWindow::updateMemoryLabels()
{
    for(BOXBYTE r=0;r<64;r++){
        for(BOXBYTE c=0;c<4;c++){
            auto data = machine.getValue(AccessMethod::ADDRESS,r*4 + c);
            memLabels[r][c]->setText(getHexNum(data,1));
            memLabels[r][c]->setStyleSheet("QLabel {background: white}");
            for(int t=0;t<machine.getNumThreads();t++)
            {
                int pc = machine.getValue(AccessMethod::ADDRESS,machine.getPC(t));
                if(pc/4 ==r)
                {
                    memLabels[r][c]->setStyleSheet("QLabel {background: blue}");
                }
            }
        }
    }
}
void MainWindow::timerEvent(QTimerEvent * timer)
{
    stepMachine();
}
void MainWindow::playMachine()
{
    playTimer.start(1000 / 60,this);
    /*for(int i=0;i<100;i++)
    {
        stepMachine();
    }*/
}
void MainWindow::on_srcCellChanged()
{
    auto srcTblCell = static_cast<TableCellEdit*>(sender());
    QString text = srcTblCell->toPlainText();
    text = text.toUpper();

    if(text.length()>3)//Limit size of input
    {
        text = ""+text[0]+text[1]+text[2];
    }
    QChar c1 = text[0];
    QChar c2 = text[1];
    QChar c3 = text[2];

    if(srcTblCell->getCellColumn()==0)
    {
        text="";
        if(c1.isLetter()||c1.isNumber())text+=c1;
        if(c2.isLetter()||c2.isNumber())text+=c2;
        if(c3.isLetter()||c3.isNumber())text+=c3;
    }
    if(srcTblCell->getCellColumn()>0)
    {
        text="";
        if(c1.isNumber()||c1=="-"||c1=="@"||c1=="*"){
            text+=c1;
        }
        if(c2.isNumber() || (c2>='A' && c2<='F')){
            text+=c2;
        }
        if(c3.isNumber() || (c3>='A' && c3<='F')){
            text+=c3;
        }


        if(text=="")text="0";
    }
    text = text.toUpper();
    if(text!=srcTblCell->toPlainText())
    {
        srcTblCell->setPlainText(text);
        srcTblCell->moveCursor(QTextCursor::End);
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionAbout_QT_triggered()
{
    QMessageBox::aboutQt(this,"About QT");
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,"About Box256QT","<h3>About Box256QT</h3> A Box256 Emulator made with QT. Made by <a href=\"https://github.com/FinalForEach\">FinalForEach</a>. <br/>"
                                             "The original Box256 reference implementation by <a href=\"http://box-256.com\">Juha Kiili</a>");
}

void MainWindow::on_actionNew_triggered()
{
    for(int r=0;r<64;r++)
    {
        cellTexts[r][0]->setPlainText("0");
        cellTexts[r][1]->setPlainText("0");
        cellTexts[r][2]->setPlainText("0");
        cellTexts[r][3]->setPlainText("0");
    }
    machine.reset();
    updateMemoryLabels();
}

void MainWindow::on_actionLoad_Example_Program_triggered()
{
    loadBoxFile(":/machine/default_blue_square.box256");
}

void MainWindow::on_actionLoad_triggered()
{
    dialogLoad.setSrcText(getMachineSource());
    dialogLoad.show();
}
