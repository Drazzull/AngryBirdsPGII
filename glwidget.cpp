#include <GL/glew.h>
#include "GLWidget.h"
#include <iostream>
#include <cmath>
#include <QKeyEvent>
#include <QTimer>


GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent), width(1366), height(728), max_fps(100)
{
    resize(width, height);

    QTimer::singleShot(1000/max_fps, this, SLOT(tick()));
}

GLWidget::~GLWidget()
{
}

void GLWidget::tick()
{
    repaint();
    QTimer::singleShot(1000/max_fps, this, SLOT(tick()));
}

void GLWidget::startObjects()
{
    this->particulas.clear();
    for (int i = 0; i < 4; i++)
    {
        particulas.push_back(Particula(width, height, 100 + i * width/6, 50));
    }
}

void GLWidget::initializeGL()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    this->startObjects();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    this->stern.display();

    for(unsigned int i = 0; i < this->particulas.capacity(); i++)
    {
        Vetor force = this->stern.calcularAtracao(this->particulas[i]);
        this->particulas[i].aplicarForca(force);
        this->particulas[i].atualizar();
        this->particulas[i].display();
    }
    glFlush();
}

void GLWidget::resizeGL(int w, int h)
{
    width = w;
    height = h;

    glViewport     ( 0, 0, w, h );

    glMatrixMode   ( GL_PROJECTION );
    glLoadIdentity ( );

    gluOrtho2D(0, w, h, 0);
}
