#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QLabel>
#include <QTextStream>
#include <QBasicTimer>

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
    ~MainWindow() override;

    void setMachineSource(QString& src);
    void setMachineSource(QTextStream& boxStream);
    QString getMachineSource();

private slots:
    void stopMachine();
    void playMachine();

    void stepBtnReleased();

    void on_actionAbout_QT_triggered();

    void on_actionAbout_triggered();

    void on_srcCellChanged();

    void on_actionNew_triggered();

    void on_actionLoad_Example_Program_triggered();

    void on_actionLoad_triggered();

private:
    void stepMachine();
    Ui::MainWindow *ui;
    QPlainTextEdit *cellTexts[64][4];
    QLabel *memLabels[64][4];
    Box256Machine machine;
    void updateMemoryLabels();
    void loadBoxFile(const QString& fileName);
    DialogLoad dialogLoad;
    QBasicTimer playTimer;
    void timerEvent(QTimerEvent *) override;

};
#endif // MAINWINDOW_H
