#pragma once

#include "vetor.h"
#include "particula.h"
#include <QGLWidget>

/**
 * @brief Classe contendo as definições do planeta
 */
class Planeta
{
public:
    /**
     * @brief Planeta - Construtor já definindo o tamanho da tela
     * de modo que o planeta seja definido conforme essas dimensões
     * @param screen_width
     * @param screen_height
     */
    Planeta(int screen_width = 1920, int screen_height = 1080);

    /**
     * @brief calcularAtracao - Faz o cálculo da atração do objeto passado por parâmetro
     * @param m - Partícula que será calculada
     * @return Vetor de alteração da posição da partícula
     */
    Vetor calcularAtracao(Particula m);

    /**
     * @brief display - Apresenta o planeta
     */
    void display();

    /**
     * @brief displayAtmosfera - Apresenta a atmosfera do planeta
     */
    void displayAtmosfera();

    /**
     * @brief particulaAdentrouAtmosfera - Verifica se a partícula está sob efeito da gravidade
     * @param m - Partícula a ser verificada
     * @return True em caso dela estar sob efeito, false caso contrário
     */
    bool particulaAdentrouAtmosfera(Particula m);

    // Getters
    Vetor getPos();
    GLfloat getRaio();

private:
    Vetor pos, dragOffset;
    GLfloat massa, G, raioAtmosfera, raio;
};
