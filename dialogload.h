#ifndef DIALOGLOAD_H
#define DIALOGLOAD_H

#include <QDialog>

namespace Ui {
class DialogLoad;
}
class MainWindow;

class DialogLoad : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLoad(MainWindow *mainWindow, QWidget *parent = nullptr);
    ~DialogLoad();
    void setSrcText(const QString& srcText);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogLoad *ui;
    MainWindow *mainWindow;
};

#endif // DIALOGLOAD_H
