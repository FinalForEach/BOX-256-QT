#include "box256glwidget.h"
#include <QtMath>
#include <iostream>
Box256GLWidget::Box256GLWidget(): qTimer(), time(0)
{
    qTimer.start(1000/60,this);
}
Box256GLWidget::~Box256GLWidget()
{
    qTimer.stop();
}

void Box256GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
}
void Box256GLWidget::resizeGL(int w, int h)
{
}
void Box256GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.5f + qSin(time/10.0), 0.0f, 1.0f);
}
void Box256GLWidget::timerEvent(QTimerEvent * timer)
{
    if (timer->timerId() == qTimer.timerId())
    {
        time++;
        update();
    }
}
