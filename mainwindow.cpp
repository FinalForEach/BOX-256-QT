#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QHBoxLayout>
#include <QtMath>


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
    Box256GLWidget* box256Widget = new Box256GLWidget();
    ui->horizontalLayout->addWidget(box256Widget);

    auto *srcTable = ui->sourceTableWidget;
    QStringList rowLabels = {};
    for(int i=0;i<256;i+=4)
    {
        QString lbl = "";
        lbl=getHexNum(i,1);
        rowLabels<<lbl;
    }
    srcTable->setVerticalHeaderLabels(rowLabels);

    uint8_t data[256] = {};

    for(int r=0;r<64;r++)
    {
        for(int c=0;c<4;c++)
        {
            auto cellText = new TableCellEdit(srcTable,r,c);
            cellTexts[r][c]=cellText;
            cellText->setPlainText(getHexNum(data[r*4 + c],2));
            srcTable->setCellWidget(r,c,cellText);
            connect(cellText,SIGNAL(textChanged()),this,SLOT(on_srcCellChanged()));
        }
    }
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
        }else{text+="0";}
        if(c2.isNumber()){
            text+=c2;
        }else{text+="0";}
        if(c3.isNumber()){
            text+=c3;
        }else{text+="0";}
    }

    text = text.toUpper();
    if(text!=srcTblCell->toPlainText())
    {
        srcTblCell->setPlainText(text);
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
