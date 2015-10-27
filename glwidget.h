#pragma once
#include "planeta.h"
#include "particula.h"
#include "vetor.h"
#include <QGLWidget>
#include <QMouseEvent>


class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    GLWidget(QWidget* parent = 0);
    ~GLWidget();

    void startObjects();

    // OpenGL initialization
    void initializeGL();

    // Viewport resizing
    void resizeGL( int width, int height);

    // Painting
    void paintGL();

private slots:
    void tick();

private:
    int width, height;
    int max_fps;

    std::vector<Particula> particulas;
    Planeta stern;
};
