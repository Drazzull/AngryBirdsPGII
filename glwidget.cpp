#include <GL/glew.h>
#include "GLWidget.h"
#include <iostream>
#include <cmath>
#include <QKeyEvent>
#include <QTimer>

static GLuint texName;

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent), width(1920), height(1080), max_fps(60)
{
    resize(this->width, this->height);
    QTimer::singleShot(1000/max_fps, this, SLOT(tick()));
    this->backgroundSound = new QMediaPlayer(this);
    this->backgroundSound->setMedia(QUrl("qrc:/Sons/MainTheme.mp3"));
    this->backgroundSound->setVolume(50);
    this->velocidadeLancamento = 0.001f;
    this->magnitudeLancamento = 0.0f;
    this->direcaoLancamento = 'X';
}

GLWidget::~GLWidget()
{
}

void GLWidget::tick()
{
    repaint();
    QTimer::singleShot(1000/max_fps, this, SLOT(tick()));
}

void GLWidget::startObjects()
{
    // Limpa as propriedades de controle do passaro atual
    this->indicePassaro = 0;
    this->passaroEmMovimento = false;
    this->atirado = false;

    // Cria os objetos
    this->particulas.clear();
    this->particulas.push_back(
                Particula(this->width, this->height, 100 + (1 * (this->width / 6)), this->height / 2, 40, 0, 0,
                          'C', 'P'));

    // Quadrados
    this->particulas.push_back(
                Particula(this->width, this->height, this->planetoide.getPos().getX(),
                          this->planetoide.getPos().getY() - this->planetoide.getRaio() - 40, 0, 20, 40, 'R', 'W'));

    this->particulas.push_back(
                Particula(this->width, this->height, this->planetoide.getPos().getX(),
                          this->planetoide.getPos().getY() - this->planetoide.getRaio() - 80, 0, 20, 40, 'R', 'W'));

    // Cria o planetóide
    this->planetoide = Planeta(this->width, this->height);

    // Toca o som
    this->backgroundSound->stop();
    this->backgroundSound->play();
}

void GLWidget::initializeGL()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    this->startObjects();

    // Carrega a imagem
    QImage img;
    if (!img.load(":/Texturas/Background.jpg"))
    {
        qDebug("Erro ao carregar a imagem");
        return;
    }

    // Configura a Textura
    QImage imgTextura = QGLWidget::convertToGLFormat(img);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgTextura.width(), imgTextura.height(), 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, imgTextura.bits());

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Vetor GLWidget::atirarPassaro()
{
    // Se a magnitude do lançamento for maior que 100, altera ele para o tamanho máximo
    if(this->magnitudeLancamento > 100.0f)
    {
        this->magnitudeLancamento = 100.0f;
    }

    switch(this->direcaoLancamento)
    {
    case 'S':
        return Vetor(cos(this->angulo * PI / 180) * this->magnitudeLancamento * this->velocidadeLancamento,
                     (sin(this->angulo * PI / 180) * (this->magnitudeLancamento * this->velocidadeLancamento)));

    case 'C':
        return Vetor(cos(this->angulo * PI / 180) * this->magnitudeLancamento * this->velocidadeLancamento,
                     (sin(this->angulo * PI / 180) * (this->magnitudeLancamento * this->velocidadeLancamento)) * -1);

    default:
        return Vetor(0, 0);
    }
}

void GLWidget::paintGL()
{
    // Inicializa a pintura
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Desenha o plano de fundo
    this->displayBackGround();

    // Mostra o planeta
    this->planetoide.display();

    for(unsigned int i = 0; i < this->particulas.capacity(); i++)
    {
        // Calcula a atração, aplica a força da mesma
        Vetor force = this->planetoide.calcularAtracao(this->particulas[i]);
        this->particulas[i].aplicarForca(force);

        // Se a partícula atual for o índice do pássaro atira o mesmo
        if ((i == this->indicePassaro) && this->atirado)
        {
            Vetor estilingue = this->atirarPassaro();
            this->particulas[i].aplicarForca(estilingue);
        }

        bool colidido = false;

        // Checa a colisão com as demais partículas
        for(unsigned int j = 0; j < this->particulas.capacity(); j++)
        {
            if (j == i)
            {
                continue;
            }

            if ((this->particulas[i].checarColisaoEntreParticulas(this->particulas[j])) &&
                    !colidido)
            {
                colidido = true;
            }
        }

        // Por fim checa a colisão com o planeta
        if ((this->particulas[i].checarColisaoPlaneta(this->planetoide.getPos(), this->planetoide.getRaio())) &&
                !colidido)
        {
            colidido = true;
        }

        // Define a última posição válida
        if (!colidido)
        {
            this->particulas[i].setUltimaPosValida(this->particulas[i].getPosicao());
        }

        this->particulas[i].atualizar();
        this->particulas[i].display();
    }
    glFlush();
}

void GLWidget::resizeGL(int w, int h)
{
    this->width = w;
    this->height = h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0, w, h, 0);
}

void GLWidget::displayBackGround()
{
    // Reinicia a posição
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, 0.0f);

    // Habilita a textura
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, texName);

    // Constrói o plano de fundo
    glBegin(GL_POLYGON);
    {
        glTexCoord2f(0.0, 0.0); glVertex2f(0, 0);
        glTexCoord2f(1.0, 0.0); glVertex2f(this->width, 0);
        glTexCoord2f(1.0, 1.0); glVertex2f(this->width, this->height);
        glTexCoord2f(0.0, 1.0); glVertex2f(0, this->height);
    }
    glEnd();

    // Desabilita as texturas
    glFlush();
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    QGLWidget::keyPressEvent(event);

    if (event->key() == (Qt::Key_R) && (event->modifiers().testFlag(Qt::ControlModifier)))
    {
        // Reinicia os objetos
        this->startObjects();
    }
}

void GLWidget::mousePressEvent(QMouseEvent* event)
{
    QGLWidget::mousePressEvent(event);

    // Somente executa as particularidades do método se o usuário clicou com o botão esquerdo do mouse
    if (event->button() !=  Qt::LeftButton)
    {
        return;
    }

    // Se o pássaro foi atirado, cancela a execução do método
    if (this->atirado)
    {
        return;
    }

    // Se o usuário não clicar no passaro não inicia a contagem
    int particulaX = this->particulas[this->indicePassaro].getPosicao().getX();
    int particulaY = this->particulas[this->indicePassaro].getPosicao().getY();
    GLfloat mag = sqrt(pow(event->pos().x() - particulaX, 2) +
                       pow(event->pos().y() - particulaY, 2));
    if (mag > this->particulas[this->indicePassaro].getRaio())
    {
        return;
    }

    this->passaroEmMovimento = true;
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QGLWidget::mouseReleaseEvent(event);
    if (!this->passaroEmMovimento)
    {
        return;
    }

    // Define que o pássaro parou de se movimentar por função do jogador
    this->passaroEmMovimento = false;

    // Define o uso do vetorFinal
    Vetor vetorFinal = Vetor(event->x(), event->y());

    // Calcula a magnitude entre os dois vetores
    this->magnitudeLancamento = Vetor::magnitudeEntreVetores(
                this->particulas[this->indicePassaro].getPosicaoInicial(), vetorFinal);

    // Define a direção do lançamento
    this->direcaoLancamento =
            vetorFinal.getY() > this->particulas[this->indicePassaro].getPosicaoInicial().getY() ? 'C' : 'S';

    // Calcula o ângulo
    this->angulo = Vetor::anguloEntreVetores(this->particulas[this->indicePassaro].getPosicaoInicial(), vetorFinal,
            this->magnitudeLancamento);

    // A velocidade do lançamento vai ser 0.0005% da magniute
    this->velocidadeLancamento = this->magnitudeLancamento * 0.00005;
    this->atirado = true;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    QGLWidget::mouseMoveEvent(event);

    if (!this->passaroEmMovimento)
    {
        return;
    }

    // Se o pássaro entrou em órbita, cancela a execução do método
    if (this->planetoide.particulaAdentrouAtmosfera(
                this->particulas[this->indicePassaro]))
    {
        return;
    }

    this->particulas[this->indicePassaro].setPosicao(
                Vetor(event->pos().x(), event->pos().y()));
}
