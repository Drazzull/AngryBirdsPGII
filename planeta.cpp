#include "planeta.h"
#include <cmath>

float constrain(float value, float min, float max)
{
    return (value > max) ? max : min;
}

Planeta::Planeta(int screen_width, int screen_height)
{
    this->pos = Vetor(screen_width / 1.25, screen_height / 2);
    this->massa = 20;
    this->G = 5;
    this->raio = screen_width > screen_height ? screen_width / 8 : screen_height / 8;
    this->raioAtmosfera = this->raio + (this->raio / 1.25);
}

bool Planeta::particulaAdentrouAtmosfera(Particula m)
{
    return (sqrt(pow(this->pos.getX() - m.getPosicao().getX(), 2) +
                 pow(this->pos.getY() - m.getPosicao().getY(), 2)) <= this->raioAtmosfera + m.getRaio());
}

Vetor Planeta::calcularAtracao(Particula m)
{
    if (!this->particulaAdentrouAtmosfera(m))
    {
        return Vetor(0, 0);
    }

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
    float strength = (this->G * this->massa * m.getMassa()) / pow(distance, 2);

    // Pegar o vetor força --> magnitude * direction
    forca.mult(strength);

    return forca;
}

void Planeta::display()
{
    this->displayAtmosfera();

    glLoadIdentity();
    glTranslatef(this->pos.getX(), this->pos.getY(), 0.0f);

    glColor3ub(255, 128, 0);
    glBegin(GL_POLYGON);

    for (int angulo = 0; angulo < 360; angulo++)
    {
        float x = this->raio * std::cos(angulo * 3.1415 / 180);
        float y = this->raio * std::sin(angulo * 3.1415 / 180);
        glVertex2f(x, y);
    }

    glEnd();
}

void Planeta::displayAtmosfera()
{
    glLoadIdentity();
    glTranslatef(this->pos.getX(), this->pos.getY(), 0.0f);

    glColor4ub(0, 0, 255, 25);
    glBegin(GL_POLYGON);

    for (int angulo = 0; angulo < 360; angulo++)
    {
        float x = this->raioAtmosfera * std::cos(angulo * 3.1415 / 180);
        float y = this->raioAtmosfera * std::sin(angulo * 3.1415 / 180);
        glVertex2f(x, y);
    }

    glEnd();
}

// Getters
Vetor Planeta::getPos()
{
    return this->pos;
}

GLfloat Planeta::getRaio()
{
    return this->raio;
}
