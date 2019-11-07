#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QLabel>
#include <QTextStream>

#include <box256machine.h>
#include <dialogload.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setMachineSource(QString& src);
    void setMachineSource(QTextStream& boxStream);
    QString getMachineSource();

private slots:
    void stopMachine();
    void stepMachine();
    void playMachine();

    void on_actionAbout_QT_triggered();

    void on_actionAbout_triggered();

    void on_srcCellChanged();

    void on_actionNew_triggered();

    void on_actionLoad_Example_Program_triggered();

    void on_actionLoad_triggered();

private:
    Ui::MainWindow *ui;
    QPlainTextEdit *cellTexts[64][4];
    QLabel *memLabels[64][4];
    Box256Machine machine;
    void updateMemoryLabels();
    void loadBoxFile(const QString& fileName);
    DialogLoad dialogLoad;

};
#endif // MAINWINDOW_H
