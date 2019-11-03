#ifndef BOX256GLWIDGET_H
#define BOX256GLWIDGET_H

#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QBasicTimer>
#include <QTimerEvent>

#include "box256machine.h"
class Box256GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
private:
    QBasicTimer qTimer;
    double time;
    QOpenGLShaderProgram gridShader;
    QOpenGLBuffer gridVertBuffer;
    QOpenGLShaderProgram pixShader;
    QOpenGLBuffer pixVertBuffer;
    int numGridVerts;
    int numPixelVerts;
    Box256Machine *machine;
public:
    Box256GLWidget(Box256Machine *m);
    ~Box256GLWidget() override;
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void timerEvent(QTimerEvent *) override;
    void drawGrid();
    void drawPixels();
    void updatePixels();
};

#endif // BOX256GLWIDGET_H
