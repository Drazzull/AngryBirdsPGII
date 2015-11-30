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

bool Particula::checarColisaoEntreParticulas(Particula m)
{
    if (!this->detectarColisaoEntreParticulas(m))
    {
        return false;
    }

    this->pos = this->ultimaPosValida;
    m.pos = m.ultimaPosValida;

    // Armazena as velocidades originais
    Vetor v1Original = this->velocidade;
    Vetor v2Original = m.velocidade;

    // Cria as novas velocidades para o cálculo da partícula atual
    Vetor velocidadeNova = this->velocidade;
    Vetor velocidadeNovaM = m.velocidade;

    // Calcula a nova velocidade da particula atual
    // vf1 = (vi1(m1 - m2) + vi2(2 * m2)) / (m1 + m2)
    velocidadeNova.mult(this->massa - m.massa);
    velocidadeNovaM.mult(2 * m.massa);
    velocidadeNova.add(velocidadeNovaM);
    velocidadeNova.div(this->massa + m.massa);
    this->velocidade = velocidadeNova;
    qDebug() << "VF1: " << velocidadeNova.getX() << ", " << velocidadeNova.getY();

    // Reinicia as velocidades para o cálculo da velocidade da outra partícula
    velocidadeNova = v1Original;
    velocidadeNovaM = v2Original;

    // Faz o cálculo da nova velocidade da segunda partícula
    // vf2 = (vi2(m2 - m1) + vi1(2 * m1)) / (m1 + m2)
    velocidadeNovaM.mult(m.massa - this->massa);
    velocidadeNova.mult(2 * this->massa);
    velocidadeNovaM.add(velocidadeNova);
    velocidadeNovaM.div(this->massa + m.massa);
    m.velocidade = velocidadeNovaM;
    qDebug() << "VF2: " << velocidadeNovaM.getX() << ", " << velocidadeNovaM.getY();

    // Define que foi colidido
    return true;
}

bool Particula::detectarColisaoEntreParticulas(Particula m)
{
    if (((this->formaGeometrica == 'C') && (m.formaGeometrica == 'C')) &&
            (this->checarColisaoCirculoCirculo(m.pos, m.raio)))
    {
        //this->pos = this->ultimaPosValida;
        //this->velocidade = Vetor(this->velocidade.getX() * -0.75, this->velocidade.getY() * -0.75);
        return true;
    }

    if (((this->formaGeometrica == 'R') && (m.formaGeometrica == 'C')) &&
            (this->checarColisaoRetanguloCirculo(m.pos, m.raio)))
    {
        /*this->pos = this->ultimaPosValida;
        this->velocidade = Vetor(0, 0);*/
        return true;
    }

    if (((this->formaGeometrica == 'C') && (m.formaGeometrica == 'R')) &&
            (this->checarColisaoCirculoRetangulo(m.pos, m.largura, m.altura)))
    {
        /*this->pos = this->ultimaPosValida;
        this->velocidade = Vetor(0, 0);*/
        return true;
    }

    if (((this->formaGeometrica == 'R') && (m.formaGeometrica == 'R')) &&
            (this->checarColisaoRetanguloRetangulo(m.pos, m.largura, m.altura)))
    {
        /*this->pos = this->ultimaPosValida;
        this->velocidade = Vetor(0, 0);*/
        return true;
    }

    return false;
}

bool Particula::checarColisaoPlaneta(Vetor planeta, GLfloat raioPlaneta)
{
    switch(this->formaGeometrica)
    {
    case 'C':
        if(this->checarColisaoCirculoCirculo(planeta, raioPlaneta))
        {
            this->pos = this->ultimaPosValida;
            this->velocidade = Vetor(this->velocidade.getX() * -0.5, this->velocidade.getY() * -0.5);
            return true;
        }

        return false;

    case 'R':
        if(this->checarColisaoRetanguloCirculo(planeta, raioPlaneta))
        {
            this->pos = this->ultimaPosValida;
            this->velocidade = Vetor(this->velocidade.getX() * -0.5, this->velocidade.getY() * -0.5);
            return true;
        }

        return false;

    default:
        return false;
    }
}

bool Particula::checarColisaoCirculoCirculo(Vetor objetoDestino, GLfloat raioDestino)
{
    return (sqrt(pow(objetoDestino.getX() - this->pos.getX(), 2) +
                 pow(objetoDestino.getY() - this->pos.getY(), 2)) <= raioDestino + this->raio);
}

bool Particula::checarColisaoRetanguloCirculo(Vetor objetoDestino, GLfloat raioDestino)
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
        return false;
    }

    // Verifica se eles estão colidindo nas laterais
    if ((distanciaCentros.getX() <= (this->largura / 2)) ||
            (distanciaCentros.getY() <= (this->altura / 2)))
    {
        return true;
    }

    // Verifica se o circulo está colidindo com o centro dos vértices
    GLfloat distanciaVertices = pow(distanciaCentros.getX() - (this->largura / 2), 2) +
            pow(distanciaCentros.getY() - (this->altura / 2), 2);
    if (distanciaVertices <= pow(raioDestino, 2))
    {

        return true;
    }

    // Define a última posição válida
    return false;
}

bool Particula::checarColisaoCirculoRetangulo(Vetor objetoDestino, GLfloat largura, GLfloat altura)
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
        return false;
    }

    // Verifica se eles estão colidindo nas laterais
    if ((distanciaCentros.getX() <= (largura / 2)) ||
            (distanciaCentros.getY() <= (altura / 2)))
    {
        return true;
    }

    // Verifica se o circulo está colidindo com o centro dos vértices
    GLfloat distanciaVertices = pow(distanciaCentros.getX() - (largura / 2), 2) +
            pow(distanciaCentros.getY() - (altura / 2), 2);
    if (distanciaVertices <= pow(this->raio, 2))
    {
        return true;
    }

    return false;
}

bool Particula::checarColisaoRetanguloRetangulo(Vetor objetoDestino, GLfloat largura, GLfloat altura)
{
    return (((objetoDestino.getY() + altura) > this->pos.getY()) &&
            (objetoDestino.getY() < (this->pos.getY() + this->altura)) &&
            (objetoDestino.getX() < (this->pos.getX() + this->largura)) &&
            ((objetoDestino.getX() + largura) > this->pos.getX()));
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

void Particula::setUltimaPosValida(Vetor posicao)
{
    this->ultimaPosValida = posicao;
}
