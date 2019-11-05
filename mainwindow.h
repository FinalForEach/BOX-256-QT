#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QLabel>

#include <box256machine.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAbout_QT_triggered();
    void on_actionAbout_triggered();
    void on_srcCellChanged();
    void stopMachine();
    void stepMachine();
    void playMachine();

private:
    Ui::MainWindow *ui;
    QPlainTextEdit *cellTexts[64][4];
    QLabel *memLabels[64][4];
    Box256Machine machine;
    void updateMemoryLabels();
    void loadBoxFile(const QString& fileName);

};
#endif // MAINWINDOW_H
