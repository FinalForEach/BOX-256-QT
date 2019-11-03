#include "box256glwidget.h"
#include <QtMath>
#include <QOpenGLFunctions>
#include <iostream>
Box256GLWidget::Box256GLWidget(Box256Machine *m)
    : qTimer(), time(0), gridVertBuffer(QOpenGLBuffer::VertexBuffer), numGridVerts(0),numPixelVerts(0), machine(m)
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
    QString pixelVertShaderStr = "#version 330 core\n"
            "layout (location = 0) in vec2 aPos;\n"
            "layout (location = 1) in vec3 aColor;\n"
            "out vec3 pixColor;\n"
            "void main()\n"
            "{"
            "   gl_Position = vec4(aPos.xy, 0.0, 1.0);\n"
            "   pixColor = aColor;\n"
            "}";
    QString pixelFragShaderStr = "#version 330 core\n"
            "in vec3 pixColor;\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{"
            "   FragColor = vec4(pixColor, 1.0f);\n"
            "}";
    gridShader.addShaderFromSourceCode(QOpenGLShader::Vertex,vertShaderStr);
    gridShader.addShaderFromSourceCode(QOpenGLShader::Fragment,gridFragShaderStr);
    gridShader.link();
    gridVertBuffer.create();
    gridVertBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);

    pixShader.addShaderFromSourceCode(QOpenGLShader::Vertex,pixelVertShaderStr);
    pixShader.addShaderFromSourceCode(QOpenGLShader::Fragment,pixelFragShaderStr);
    pixShader.link();



    pixVertBuffer.create();
    pixVertBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);

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

    updatePixels();
}
void Box256GLWidget::updatePixels()
{
    numPixelVerts=0;
    pixVertBuffer.bind();
    QVector<float> verticesVec;
    float s = 0.5f / 0xF;
    for(BOXBYTE i=0x0;i<=0xF;i++)
    {
        float x = i*2*s - 0.5f;
        for(BOXBYTE j=0x0;j<=0xF;j++)
        {
            float y = j*2*s - 0.5f;
            BOXBYTE color = machine->getPixel(i*0xF + j);
#define setColor(red,green,blue) r=red/255.0f;g=green/255.0f;b=blue/255.0f
            float r=0,g=0,b=0;
            switch (color % 0xF) {
                case 0x0:{setColor(50,0,0);break;}
                case 0x1:{setColor(31,46,80);break;}
                case 0x2:{setColor(116,50,83);break;}
                case 0x3:{setColor(61,129,84);break;}
                case 0x4:{setColor(159,88,65);break;}
                case 0x5:{setColor(94,87,80);break;}
                case 0x6:{setColor(194,195,199);break;}
                case 0x7:{setColor(253,241,233);break;}
                case 0x8:{setColor(233,65,91);break;}
                case 0x9:{setColor(242,165,67);break;}
                case 0xA:{setColor(255,249,93);break;}
                case 0xB:{setColor(109,221,100);break;}
                case 0xC:{setColor(84,175,247);break;}
                case 0xD:{setColor(128,121,153);break;}
                case 0xE:{setColor(237,133,170);break;}
                case 0xF:{setColor(247,205,176);break;}
            }
#undef setColor
#define vx(f) verticesVec.push_back(f+x); numPixelVerts++
#define vy(f) verticesVec.push_back(f+y); numPixelVerts++
#define vc verticesVec.push_back(r);verticesVec.push_back(g);verticesVec.push_back(b);numPixelVerts+=3
            vx(-s);vy(-s);vc;vx(-s);vy(s);vc;vx(s);vy(s);vc;
            vx(-s);vy(-s);vc;vx(s);vy(-s);vc;vx(s);vy(s);vc;
#undef vc
#undef vx
#undef vy
        }
    }
    pixVertBuffer.allocate(verticesVec.data(),verticesVec.size() * static_cast<int>(sizeof(float)) );
}
void Box256GLWidget::resizeGL(int w, int h)
{
    glViewport( 0, 0, w, qMax( h, 1 ) );
}
void Box256GLWidget::drawGrid()
{
    gridVertBuffer.bind();
    gridShader.bind();
    gridShader.setAttributeBuffer("aPos",GL_FLOAT,0,2);
    gridShader.enableAttributeArray("aPos");
    glDrawArrays(GL_LINES, 0, numGridVerts);
}
void Box256GLWidget::drawPixels()
{
    pixVertBuffer.bind();
    pixShader.bind();
    pixShader.setAttributeBuffer("aPos",GL_FLOAT,0,2,5*sizeof(float));
    pixShader.enableAttributeArray("aPos");
    pixShader.setAttributeBuffer("aColor",GL_FLOAT,2*sizeof(float),3,5*sizeof(float));
    pixShader.enableAttributeArray("aColor");
    glDrawArrays(GL_TRIANGLES, 0, numPixelVerts);
}
void Box256GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    updatePixels();
    drawPixels();
    drawGrid();
}
void Box256GLWidget::timerEvent(QTimerEvent * timer)
{
    if (timer->timerId() == qTimer.timerId()){
        time++;
        update();
    }
}
