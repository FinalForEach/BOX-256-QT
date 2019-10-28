#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QHBoxLayout>
#include <QtMath>
#include <QTableWidget>
#include <QLabel>
#include <QHeaderView>

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
    return hexNum;
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->gridLayout->addWidget(new QLabel("Source",this),0,0);
    ui->gridLayout->addWidget(new QLabel("Memory",this),0,1);
    ui->gridLayout->addWidget(new QLabel("Output",this),0,2);

    QTableWidget *srcTable = new QTableWidget(64,4,this);
    QTableWidget *memTable = new QTableWidget(64,4,this);
    Box256GLWidget* box256Widget = new Box256GLWidget();
    ui->gridLayout->addWidget(srcTable,1,0,Qt::AlignLeft);
    ui->gridLayout->addWidget(memTable,1,1,Qt::AlignLeft);
    ui->gridLayout->addWidget(box256Widget,1,2,1,2);

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
            cellText->setPlainText(getHexNum(sourceData[r][c].toInt(),0));

            srcTable->setCellWidget(r,c,cellText);
            connect(cellText,SIGNAL(textChanged()),this,SLOT(on_srcCellChanged()));
        }
    }
    //Setup memory table
    for(int r=0;r<64;r++)
    {
        for(int c=0;c<4;c++)
        {
            memData[r*4+c]=0;
            auto cellText = new QLabel(getHexNum(memData[r*4+c]),memTable);
            cellText->setAlignment(Qt::AlignCenter);
            memTable->setCellWidget(r,c,cellText);
        }
    }
    memTable->setShowGrid(false);
    memTable->horizontalHeader()->hide();
}
void MainWindow::on_srcCellChanged()
{
    auto srcTblCell = static_cast<TableCellEdit*>(sender());
    QString text = srcTblCell->toPlainText();

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
        if(c2.isNumber()){
            text+=c2;
        }
        if(c3.isNumber()){
            text+=c3;
        }


        if(text=="")text="0";
    }
    text = text.toUpper();
    sourceData[srcTblCell->getCellRow()][srcTblCell->getCellColumn()]=text;
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
