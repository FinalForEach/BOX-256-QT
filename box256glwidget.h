#ifndef BOX256GLWIDGET_H
#define BOX256GLWIDGET_H

#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QBasicTimer>
#include <QTimerEvent>

class Box256GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
private:
    QBasicTimer qTimer;
    double time;
public:
    Box256GLWidget();
    ~Box256GLWidget() override;
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void timerEvent(QTimerEvent *) override;
};

#endif // BOX256GLWIDGET_H
