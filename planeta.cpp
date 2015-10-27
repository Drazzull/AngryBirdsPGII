#include "planeta.h"
#include <cmath>

float constrain(float value, float min, float max)
{
    return (value > max) ? max : min;
}

Planeta::Planeta(int screen_width, int screen_height)
{
    this->pos = Vetor(screen_width / 2, screen_height / 2);
    this->massa = 20;
    this->G = 1;
}

Vetor Planeta::calcularAtracao(Particula m)
{
    // Cálculo da direção da força
    Vetor forca = Vetor::sub(this->pos, m.getPosicao());

    // Distância entre objetos
    float distance = forca.mag();

    // Limitar a distância para eliminar resultados extremos
    // para objetos muito próximos ou muito distantes
    distance = constrain(distance, 5, 25);

    // Normalizar vetor
    forca.normalize();

    // Calcular a magnitude da força gravitacional
    float strength = (this->G * this->massa * m.getMassa()) / (distance * distance);

    // Pegar o vetor força --> magnitude * direction
    forca.mult(strength);

    return forca;
}

void Planeta::display()
{
    glLoadIdentity();
    glTranslatef(this->pos.getX(), this->pos.getY(), 0.0f);

    glColor3ub(255, 128, 0);
    glBegin(GL_POLYGON);
    float raio = 225;

    for (int angulo = 0; angulo < 360; angulo++)
    {
        float x = raio * std::cos(angulo * 3.1415 / 180);
        float y = raio * std::sin(angulo * 3.1415 / 180);
        glVertex2f(x, y);
    }

    glEnd();
}
