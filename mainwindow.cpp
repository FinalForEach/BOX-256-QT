#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QHBoxLayout>

#include "box256glwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Box256GLWidget* box256Widget = new Box256GLWidget();
    ui->horizontalLayout->addWidget(box256Widget);
    ui->sourceTableWidget->setRowCount(64);
    ui->sourceTableWidget->setColumnCount(4);

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
    QString qStr = "";

}
