#pragma once

#include "vetor.h"
#include <QGLWidget>

class Particula
{
public:
    Particula(int screen_width = 1920, int screen_height = 1080, int x = 0,
              int y = 0, GLfloat raio = 0, GLfloat largura = 0, GLfloat altura = 0,
              char formaGeometrica = 'X', char tipoParticula = 'X');
    Particula(const Particula& obj);
    ~Particula();

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
    void checarColisaoPlaneta(Vetor planeta, GLfloat raioPlaneta);

    /**
     * @brief checarColisaoEntreParticulas - Checa a colisão entre as partículas
     * @param m - Partícula que a atual deverá verificar se colidiu
     */
    void checarColisaoEntreParticulas(Particula m);

    // Getters
    GLfloat getMassa();
    Vetor getVelocidade();
    Vetor getPosicao();
    bool estaEmMovimento();
    int getRaio();
    Vetor getPosicaoInicial();

    // Setters
    void setPosicao(Vetor posicaoNova);

private:
    int screen_width, screen_height;
    GLbyte cor[3];
    Vetor aceleracao, velocidade, pos, posInicial;
    bool emMovimento;
    Vetor ultimaPosValida;

    // Propriedades com o tipo da partícula e a forma geométrica (C e R)
    char tipoParticula, formaGeometrica;

    // Todos os objetos têm massa,
    // Os circulos têm raio para controlar o tamanho,
    // E os retangulos  têm largura e altura.
    GLfloat massa, raio, largura, altura;

    int random(int minimo, int maximo);

    /**
     * @brief pintarObjeto - Pinta o objeto conforme as propriedades dele
     */
    void pintarObjeto();

    /**
     * @brief checarColisaoCirculoCirculo - Faz a detecção de colisão de uma partícula circular com um circulo
     * @param objetoDestino - Vetor com as propriedades do circulo de destino
     * @param raioDestino - Raio do destino
     */
    void checarColisaoCirculoCirculo(Vetor objetoDestino, GLfloat raioDestino);

    /**
     * @brief checarColisaoRetanguloCirculo - Faz a detecção de colisão de uma partícula retangular com um circulo
     * @param objetoDestino - Vetor com as propriedades do circulo de destino
     * @param raioDestino - Raio do destino
     */
    void checarColisaoRetanguloCirculo(Vetor objetoDestino, GLfloat raioDestino);

    /**
     * @brief checarColisaoRetanguloCirculo - Faz a detecção de colisão de uma partícula circular com um retângulo
     * @param objetoDestino - Vetor com as propriedades do circulo de destino
     * @param largura - Largura do objeto
     * @param altura - Altura do objeto
     */
    void checarColisaoCirculoRetangulo(Vetor objetoDestino, GLfloat largura, GLfloat altura);

    /**
     * @brief checarColisaoRetanguloCirculo - Faz a detecção de colisão de uma partícula retangular com um retângulo
     * @param objetoDestino - Vetor com as propriedades do circulo de destino
     * @param largura - Largura do objeto
     * @param altura - Altura do objeto
     */
    void checarColisaoRetanguloRetangulo(Vetor objetoDestino, GLfloat largura, GLfloat altura);

    /**
     * @brief displayCirculo - Desenha a partícula com propriedades de circulo
     */
    void displayCirculo();

    /**
     * @brief displayRetangulo - Desenha a partícula com propriedades de retangulo
     */
    void displayRetangulo();
};
