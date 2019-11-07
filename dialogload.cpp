#include "dialogload.h"
#include "ui_dialogload.h"

#include "mainwindow.h"

DialogLoad::DialogLoad(MainWindow *mainWindow_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLoad),
    mainWindow(mainWindow_)
{
    ui->setupUi(this);
}

DialogLoad::~DialogLoad()
{
    delete ui;
}

void DialogLoad::on_buttonBox_accepted()
{
    QString srcText = ui->newSrcTxtEdit->toPlainText();
    mainWindow->setMachineSource(srcText);
}
void DialogLoad::setSrcText(const QString& srcText)
{
    ui->newSrcTxtEdit->setPlainText(srcText);
}
