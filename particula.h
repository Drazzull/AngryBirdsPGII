#pragma once

#include "vetor.h"
#include <QGLWidget>

class Particula
{
public:
    Particula(int screen_width = 1920, int screen_height = 1080, int x = 0, int y = 0, int raio = 0);
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

    /**
     * @brief checarColisaoPlaneta - Verifica se a partícula colidiu com o planeta e corrige a rota
     * @param planeta - Vetor com a posição do planeta a ser verificado
     * @param raioPlaneta - Tamanho do planeta
     */
    void checarColisaoPlaneta(Vetor planeta, int raioPlaneta);

    // Getters
    GLfloat getMassa();
    Vetor getVelocidade();
    Vetor getPosicao();
    bool estaEmMovimento();
    int getRaio();

    // Setters
    void setPosicao(Vetor posicaoNova);

private:
    int screen_width, screen_height, raio;
    GLbyte cor[3];
    Vetor aceleracao, velocidade, pos;
    GLfloat massa;
    bool emMovimento;
    Vetor ultimaPosValida;

    int random(int minimo, int maximo);
};
