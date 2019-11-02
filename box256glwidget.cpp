#include "box256glwidget.h"
#include <QtMath>
#include <QOpenGLFunctions>
#include <iostream>
Box256GLWidget::Box256GLWidget()
    : qTimer(), time(0), gridVertBuffer(QOpenGLBuffer::VertexBuffer), numGridVerts(0), machine()
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


    QString vertShaderStr = "#version 330 core\n"
            "layout (location = 0) in vec2 aPos;\n"
            "void main()\n"
            "{"
            "   gl_Position = vec4(aPos.xy, 0.0, 1.0);\n"
            "}";
    QString gridFragShaderStr = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{"
            "   FragColor = vec4(1.0f);\n"
            "}";
    gridShader.addShaderFromSourceCode(QOpenGLShader::Vertex,vertShaderStr);
    gridShader.addShaderFromSourceCode(QOpenGLShader::Fragment,gridFragShaderStr);
    gridShader.link();
    gridVertBuffer.create();
    gridVertBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    gridVertBuffer.bind();



    QVector<float> verticesVec;
    float s = 0.5f / 0xF;
    for(int i=0x0;i<=0xF;i++)
    {
        float x = i*2*s - 0.5f;
        for(int j=0x0;j<=0xF;j++)
        {
            float y = j*2*s - 0.5f;
#define vx(f) verticesVec.push_back(f+x); numGridVerts++
#define vy(f) verticesVec.push_back(f+y); numGridVerts++
            vx(-s);vy(-s);vx(-s);vy(s);
            vx(-s);vy(s);vx(s);vy(s);
            vx(s);vy(s);vx(s);vy(-s);
            vx(s);vy(-s);vx(-s);vy(-s);
#undef vx
#undef vy
        }
    }
    gridVertBuffer.allocate(verticesVec.data(),verticesVec.size() * static_cast<int>(sizeof(float)) );
}
void Box256GLWidget::resizeGL(int w, int h)
{
    glViewport( 0, 0, w, qMax( h, 1 ) );
}
void Box256GLWidget::drawGrid()
{
    gridShader.bind();

    gridShader.setAttributeBuffer("aPos",GL_FLOAT,0,2);
    gridShader.enableAttributeArray("aPos");
    glDrawArrays(GL_LINES, 0, numGridVerts);
}
void Box256GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    drawGrid();
}
void Box256GLWidget::timerEvent(QTimerEvent * timer)
{
    if (timer->timerId() == qTimer.timerId())
    {
        time++;
        update();
    }
}
