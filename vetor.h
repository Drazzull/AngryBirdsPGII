#pragma once
#include <QGLWidget>
#include <math.h>

class Vetor
{
public:
    /**
     * @brief Vetor - Cria um objeto do tipo Vetor com os dados pré-definidos
     * @param x - posição do eixo x pré-definida
     * @param y - posição do eixo y pré-definida
     */
    Vetor(GLfloat x = 0, GLfloat y = 0)
    {
        this->x = x;
        this->y = y;
    }

    /**
     * @brief Vetor - Cria um novo objeto do tipo Vetor copiando os dados de um já existente
     * @param obj - Objeto a ser copiado
     */
    Vetor(const Vetor& obj)
    {
        this->x = obj.x;
        this->y = obj.y;
    }

    /**
     * @brief add - Adiciona ao vetor atual os dados de um vetor existente
     * @param v - Vetor que será adicionado
     */
    void add(Vetor v)
    {
        this->x += v.getX();
        this->y += v.getY();
    }

    /**
     * @brief sub - Subtrai do vetor atual os dados de um vetor existente
     * @param v - Vetor que será subtraído
     */
    void sub(Vetor v)
    {
        this->x -= v.getX();
        this->y -= v.getY();
    }

    /**
     * @brief mult - Multiplica uma constante ao vetor atual
     * @param s - Constante que será multiplicada
     */
    void mult(float s)
    {
        this->x *= s;
        this->y *= s;
    }

    /**
     * @brief div - Divide o vetor atual por uma constante
     * @param s - Constante pela qual o vetor será dividido
     */
    void div(float s)
    {
        this->x /= s;
        this->y /= s;
    }

    /**
     * @brief mag - Calcula a magnitude do vetor
     * @return Magnitude calculada
     */
    GLfloat mag()
    {
        return sqrt(pow(this->x, 2) + pow(this->y, 2));
    }

    /**
     * @brief normalize - Normaliza o vetor
     */
    void normalize()
    {
        if (mag() > 0)
        {
            div(mag());
        }
    }

    /**
     * @brief getX - Retorna o valor do eixo X
     * @return Eixo X
     */
    GLfloat getX()
    {
        return this->x;
    }

    /**
     * @brief getY - Retorna o valor do eixo y
     * @return Eixo Y
     */
    GLfloat getY()
    {
        return this->y;
    }

    /**
     * @brief sub - Subtrai dois vetores (v1 - v2)
     * @param v1 - Vetor primário da subtração
     * @param v2 - Vetor secundário da subtração
     * @return Resultado da subtração
     */
    static Vetor sub(Vetor v1, Vetor v2)
    {
        return Vetor(v1.getX() - v2.getX(), v1.getY() - v2.getY());
    }

private:
    GLfloat x, y;
};
