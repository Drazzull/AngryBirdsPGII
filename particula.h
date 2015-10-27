#pragma once
#include "vetor.h"

#include <QGLWidget>


class Particula
{
public:
    Particula(int screen_width = 1366, int screen_height = 768, int x = 0, int y = 0);
    Particula(const Particula& obj);
    ~Particula();

    /**
     * @brief checarLimites - Checa os limites da tela para verificar se houve colisão por parte da partícula
     */
    void checarLimites();

    /**
     * @brief aplicarForca - Aplica um vetor que representa uma força à partícula atual
     * @param forca - Força que será aplicada
     */
    void aplicarForca(Vetor forca);

    /**
     * @brief atualizar - Atualiza a posição do objeto após a aplicação da velocidade
     */
    void atualizar();

    /**
     * @brief display - Desenha a partícula
     */
    void display();

    float getMassa();
    Vetor getVelocidade();
    Vetor getPosicao();

private:
    int random(int minimo, int maximo);

    int screen_width, screen_height;
    GLbyte cor[3];
    Vetor aceleracao, velocidade, pos;
    float massa, raio;
};
