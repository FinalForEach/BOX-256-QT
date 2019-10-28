#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>

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

private:
    Ui::MainWindow *ui;
    QPlainTextEdit *cellTexts[64][4];
    QString sourceData[64][4];
    uint8_t memData[256];
};
#endif // MAINWINDOW_H
