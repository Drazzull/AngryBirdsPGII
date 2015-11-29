#include "particula.h"
#include <cmath>
#include <ctime>
#include <iostream>


Particula::Particula(int screen_width, int screen_height, int x, int y,
                     GLfloat raio, GLfloat largura, GLfloat altura,
                     char formaGeometrica, char tipoParticula)
    : screen_width(screen_width), screen_height(screen_height)
{
    this->raio = raio;
    this->largura = largura;
    this->altura = altura;

    // Verificar cálculo da massa
    this->massa = this->raio > 0 ? this->raio / 10 : this->largura / this->altura * 5;

    this->pos = Vetor(x, y);
    this->ultimaPosValida = this->pos;
    this->posInicial = this->pos;
    this->velocidade = Vetor(0.f, 0.f);
    this->aceleracao = Vetor(0.f, 0.f);
    this->formaGeometrica = formaGeometrica;
    this->tipoParticula = tipoParticula;
    this->pintarObjeto();

    //    std::cout << std::hex << "Particle (0x" << this << ")" << std::dec
    //              << " pos " << this->pos.getX() << "," << this->pos.getY() << " raio:" << this->raio << std::endl;
}

Particula::Particula(const Particula& obj)
{
    this->cor[0] = obj.cor[0];
    this->cor[1] = obj.cor[1];
    this->cor[2] = obj.cor[2];

    this->pos = obj.pos;
    this->ultimaPosValida = obj.ultimaPosValida;
    this->posInicial = obj.posInicial;
    this->velocidade = obj.velocidade;
    this->aceleracao = obj.aceleracao;
    this->emMovimento = obj.emMovimento;

    this->formaGeometrica = obj.formaGeometrica;
    this->tipoParticula = obj.tipoParticula;

    this->raio = obj.raio;
    this->massa = obj.massa;
    this->largura = obj.largura;
    this->altura = obj.altura;

    this->screen_width = obj.screen_width;
    this->screen_height = obj.screen_height;

    //    std::cout << std::hex << "Particle #2 (0x" << this << ")" << std::dec
    //              << " pos " << this->pos.getX() << "," << this->pos.getY() << " Raio:" << this->raio << std::endl;
}

Particula::~Particula(){ }

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

void Particula::checarColisaoEntreParticulas(Particula m)
{
    if ((this->formaGeometrica == 'C') && (m.formaGeometrica == 'C'))
    {
        this->checarColisaoCirculoCirculo(m.pos, m.raio);
        return;
    }

    if ((this->formaGeometrica == 'R') && (m.formaGeometrica == 'C'))
    {
        this->checarColisaoRetanguloCirculo(m.pos, m.raio);
        return;
    }

    if ((this->formaGeometrica == 'C') && (m.formaGeometrica == 'R'))
    {
        this->checarColisaoCirculoRetangulo(m.pos, m.largura, m.altura);
    }

    if ((this->formaGeometrica == 'R') && (m.formaGeometrica == 'R'))
    {
        this->checarColisaoRetanguloRetangulo(m.pos, m.largura, m.altura);
    }
}

void Particula::checarColisaoPlaneta(Vetor planeta, GLfloat raioPlaneta)
{
    switch(this->formaGeometrica)
    {
    case 'C':
        this->checarColisaoCirculoCirculo(planeta, raioPlaneta);
        break;

    case 'R':
        this->checarColisaoRetanguloCirculo(planeta, raioPlaneta);
        break;
    }
}

void Particula::checarColisaoCirculoCirculo(Vetor objetoDestino, GLfloat raioDestino)
{
    if (sqrt(pow(objetoDestino.getX() - this->pos.getX(), 2) +
             pow(objetoDestino.getY() - this->pos.getY(), 2)) <= raioDestino + this->raio)
    {
        this->pos = this->ultimaPosValida;
        this->velocidade = Vetor(this->velocidade.getX() * -0.75, this->velocidade.getY() * -0.75);
        return;
    }

    this->ultimaPosValida = Vetor(this->pos.getX(), this->pos.getY());
}

void Particula::checarColisaoRetanguloCirculo(Vetor objetoDestino, GLfloat raioDestino)
{
    // Centro do quadrado
    GLfloat centroX = this->pos.getX() + (this->largura / 2);
    GLfloat centroY = this->pos.getY() + (this->altura / 2);

    // Calcula a distância entre o centro do circulo e o centro do retângulo
    //Vetor distanciaCentros = Vetor(objetoDestino.getX() - centroX, objetoDestino.getY() - centroY);
    Vetor distanciaCentros = Vetor(abs(objetoDestino.getX() - centroX), abs(objetoDestino.getY() - centroY));

    // Verifica se os objetos estão próximos o suficiente para causar uma colisão
    if ((distanciaCentros.getX() > ((this->largura / 2) + raioDestino)) ||
            (distanciaCentros.getY() > ((this->altura / 2) + raioDestino)))
    {
        return;
    }

    // Verifica se eles estão colidindo nas laterais
    if ((distanciaCentros.getX() <= (this->largura / 2)) ||
            (distanciaCentros.getY() <= (this->altura / 2)))
    {
        this->velocidade = Vetor(0, 0);
        this->pos = this->ultimaPosValida;
        return;
    }

    // Verifica se o circulo está colidindo com o centro dos vértices
    GLfloat distanciaVertices = pow(distanciaCentros.getX() - (this->largura / 2), 2) +
            pow(distanciaCentros.getY() - (this->altura / 2), 2);

    if (distanciaVertices <= pow(raioDestino, 2))
    {
        this->velocidade = Vetor(0, 0);
        this->pos = this->ultimaPosValida;
        return;
    }

    // Define a última posição válida
    this->ultimaPosValida = Vetor(this->pos.getX(), this->pos.getY());
}

void Particula::checarColisaoCirculoRetangulo(Vetor objetoDestino, GLfloat largura, GLfloat altura)
{
    // Centro do quadrado
    GLfloat centroX = objetoDestino.getX() + (largura / 2);
    GLfloat centroY = objetoDestino.getY() + (altura / 2);

    // Calcula a distância entre o centro do circulo e o centro do retângulo
    Vetor distanciaCentros = Vetor(abs(this->pos.getX() - centroX), abs(this->pos.getY() - centroY));

    // Verifica se os objetos estão próximos o suficiente para causar uma colisão
    if ((distanciaCentros.getX() > ((largura / 2) + this->raio)) ||
            (distanciaCentros.getY() > ((altura / 2) + this->raio)))
    {
        return;
    }

    // Verifica se eles estão colidindo nas laterais
    if ((distanciaCentros.getX() <= (largura / 2)) ||
            (distanciaCentros.getY() <= (altura / 2)))
    {
        this->velocidade = Vetor(this->velocidade.getX() * -0.9, this->velocidade.getY() * -0.9);
        this->pos = this->ultimaPosValida;
        return;
    }

    // Verifica se o circulo está colidindo com o centro dos vértices
    GLfloat distanciaVertices = pow(distanciaCentros.getX() - (largura / 2), 2) +
            pow(distanciaCentros.getY() - (altura / 2), 2);

    if (distanciaVertices <= pow(this->raio, 2))
    {
        this->velocidade = Vetor(this->velocidade.getX() * -0.75, this->velocidade.getY() * -0.75);
        this->pos = this->ultimaPosValida;
        return;
    }

    // Define a última posição válida
    this->ultimaPosValida = Vetor(this->pos.getX(), this->pos.getY());
}

void Particula::checarColisaoRetanguloRetangulo(Vetor objetoDestino, GLfloat largura, GLfloat altura)
{
    if (((objetoDestino.getX() + largura) > this->pos.getX()) &&
            ((objetoDestino.getY() - altura) < this->pos.getY()))
    {
        qDebug("Colidiu acima Antes");
        return;
    }

    if ((objetoDestino.getX() < (this->pos.getX() + largura)) &&
            ((objetoDestino.getY() - altura) < this->pos.getY()))
    {
        qDebug("Colidiu acima Depois");
        return;
    }

    if (((objetoDestino.getX() + largura) > this->pos.getX()) &&
            (objetoDestino.getY() < (this->pos.getY() - this->altura)))
    {
        qDebug("Colidiu abaixo Antes");
        return;
    }

    if ((objetoDestino.getX() < (this->pos.getX() + largura)) &&
            (objetoDestino.getY() < (this->pos.getY() - this->altura)))
    {
        qDebug("Colidiu abaixo Depois");
        return;
    }
}

void Particula::display()
{
    switch(this->formaGeometrica)
    {
    case 'C':
        this->displayCirculo();
        break;

    case 'R':
        this->displayRetangulo();
        break;
    }
}

void Particula::displayCirculo()
{
    glLoadIdentity();
    glTranslatef(this->pos.getX(), this->pos.getY(), 0.0f);

    glColor3ub(this->cor[0], this->cor[1], this->cor[2]);
    glBegin(GL_POLYGON);
    {
        for (int angle = 0; angle < 360; angle++)
        {
            float x = this->raio * std::cos(angle * 3.1415 / 180);
            float y = this->raio * std::sin(angle * 3.1415 / 180);
            glVertex2f(x, y);
        }
    }
    glEnd();
}

void Particula::displayRetangulo()
{
    glLoadIdentity();
    glTranslatef(this->pos.getX(), this->pos.getY(), 0.0f);

    glColor3ub(this->cor[0], this->cor[1], this->cor[2]);
    glBegin(GL_POLYGON);
    {
        glVertex2f(0, 0);
        glVertex2f(0, this->altura);
        glVertex2f(this->largura, this->altura);
        glVertex2f(this->largura, 0);
    }
    glEnd();
}

void Particula::pintarObjeto()
{
    switch(this->tipoParticula)
    {
    case 'B': // Bird
        this->cor[0] = this->random(32, 255);
        this->cor[1] = this->random(64, 255);
        this->cor[2] = this->random(128, 255);
        break;

    case 'P': // Pig
        this->cor[0] = 0;
        this->cor[1] = 114;
        this->cor[2] = 0;
        break;

    case 'W': // Wood
        this->cor[0] = 125;
        this->cor[1] = 85;
        this->cor[2] = 50;
        break;

    case 'G': // Glass
        this->cor[0] = 135;
        this->cor[1] = 205;
        this->cor[2] = 250;
        break;

    case 'S': // Stone
        this->cor[0] = 192;
        this->cor[1] = 192;
        this->cor[2] = 192;
        break;

    default:
        qDebug("Tipo da partícula não implementada ainda.");
        break;
    }
}

// Getters
bool Particula::estaEmMovimento()
{
    return this->emMovimento;
}

GLfloat Particula::getMassa()
{
    return this->massa;
}

Vetor Particula::getVelocidade()
{
    return this->velocidade;
}

int Particula::getRaio()
{
    return this->raio;
}

Vetor Particula::getPosicao()
{
    return this->pos;
}

Vetor Particula::getPosicaoInicial()
{
    return this->posInicial;
}

// Setters
void Particula::setPosicao(Vetor posicaoNova)
{
    GLfloat xNovo = posicaoNova.getX();
    GLfloat yNovo = posicaoNova.getY();

    this->pos = Vetor(xNovo, yNovo);
}
