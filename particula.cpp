#include "particula.h"

#include <cmath>
#include <ctime>
#include <iostream>


Particula::Particula(int screen_width, int screen_height, int x, int y)
: screen_width(screen_width), screen_height(screen_height)
{
    this->cor[0] = this->random(32, 255);
    this->cor[1] = this->random(64, 255);
    this->cor[2] = this->random(128, 255);

    this->raio = this->random(10,80);
    this->massa = this->raio/10;
    this->pos = Vetor(x, y);
    this->velocidade = Vetor(0.f, 0.f);
    this->aceleracao = Vetor(0.f, 0.f);

//    std::cout << std::hex << "Particle (0x" << this << ")" << std::dec
//              << " pos " << this->pos.getX() << "," << this->pos.getY() << " raio:" << this->raio << std::endl;
}

Particula::Particula(const Particula& obj)
{
    this->cor[0] = obj.cor[0];
    this->cor[1] = obj.cor[1];
    this->cor[2] = obj.cor[2];

    this->raio = obj.raio;
    this->massa = obj.massa;
    this->pos = obj.pos;
    this->velocidade = obj.velocidade;
    this->aceleracao = obj.aceleracao;

    this->screen_width = obj.screen_width;
    this->screen_height = obj.screen_height;

//    std::cout << std::hex << "Particle #2 (0x" << this << ")" << std::dec
//              << " pos " << this->pos.getX() << "," << this->pos.getY() << " Raio:" << this->raio << std::endl;
}

Particula::~Particula()
{
//    std::cout << std::hex << "~Particle (0x" << this << ")" << std::dec
//              << " pos " << this->pos.getX() << "," << this->pos.getY() << " Raio:" << this->raio << std::endl;
}

float Particula::getMassa()
{
    return this->massa;
}

Vetor Particula::getVelocidade()
{
    return this->velocidade;
}

Vetor Particula::getPosicao()
{
    return this->pos;
}

int Particula::random(int minimo, int maximo)
{
    static bool seed = false;
    if (!seed)
    {
        srand(time(NULL));
        seed = true;
    }

    return rand() % (maximo - minimo) + minimo;
}

void Particula::aplicarForca(Vetor forca)
{
    forca.div(this->massa);
    this->aceleracao.add(forca);
}

void Particula::atualizar()
{
    this->velocidade.add(this->aceleracao);
    this->pos.add(this->velocidade);
    this->aceleracao.mult(0);
}

void Particula::checarLimites()
{
//    std::cout << std::hex << "checarLimites (0x" << this << ")" << std::dec
//              << " pos " << this->pos.getX() << "," << this->pos.getY()
//              << " Raio:" << this->raio
//              << " WxH:" << _screen_width << "x" << _screen_height << std::endl;

    if (this->pos.getX() > this->screen_width)
    {
        this->pos = Vetor(this->screen_width, this->pos.getY());
        this->velocidade = Vetor(this->velocidade.getX() * -1, this->velocidade.getY());
    }
    else if (this->pos.getX() < 0)
    {
        this->pos = Vetor(0, this->pos.getY());
        this->velocidade = Vetor(this->velocidade.getX() * -1, this->velocidade.getY());
    }

    if (this->pos.getY() > this->screen_height)
    {
        this->velocidade = Vetor(this->velocidade.getX(), this->velocidade.getY() * -1);
        this->pos = Vetor(this->pos.getX(), this->screen_height);
    }
}

void Particula::display()
{
    glLoadIdentity();
    glTranslatef(this->pos.getX(), this->pos.getY(), 0.0f);

    glColor3ub(this->cor[0], this->cor[1], this->cor[2]);
    glBegin(GL_POLYGON);
        for (int angle = 0; angle < 360; angle++)
        {
            float x = this->raio * std::cos(angle * 3.1415 / 180);
            float y = this->raio * std::sin(angle * 3.1415 / 180);
            glVertex2f(x, y);
        }
    glEnd();
}
