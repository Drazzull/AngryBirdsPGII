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
    Planeta(int screen_width = 1366, int screen_height = 768);

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

private:
    Vetor pos, dragOffset;
    float massa, G;
};
