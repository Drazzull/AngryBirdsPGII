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

    /**
     * @brief startObjects - Inicia os objetos
     */
    void startObjects();

    /**
     * @brief initializeGL - Inicialização do OpenGL
     */
    void initializeGL();

    /**
     * @brief resizeGL - Redimensionamento da tela
     * @param width - Largura
     * @param height - Altura
     */
    void resizeGL( int width, int height);

    /**
     * @brief paintGL - Pintura do OpenGL
     */
    void paintGL();

private slots:
    void tick();

private:
    // Propriedades da classe
    int width, height;
    int max_fps;
    int indicePassaro;
    std::vector<Particula> particulas;
    Planeta planetoide;
    bool passaroEmMovimento;

    // Desenha o plano de fundo
    void displayBackGround();

    // Eventos
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
};
