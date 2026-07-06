
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

// IMPORTANTE: Inclui o cabeçalho com as funções que criamos nos outros arquivos
#include "algoritmos.h"

#define TAMANHO_VETOR 40
#define VALOR_MAXIMO 50
#define VELOCIDADE_MS 20

// Estados do Sistema (Menu ou Representações Ativas)
enum EstadoSistema { ESTADO_MENU, ESTADO_BUBBLE, ESTADO_SELECTION };
EstadoSistema estado_atual = ESTADO_MENU;

// Variáveis Globais de Estado do Vetor
std::vector<int> vetor(TAMANHO_VETOR);
std::vector<bool> ordenado(TAMANHO_VETOR, false);

int cenario_atual = 1;    
bool simulacao_ativa = false; 
bool concluido = false;

// Variáveis de controle interno dos algoritmos (FSM) que os outros arquivos usam
int i_bubble = 0, j_bubble = 0;
int i_select = 0, j_select = 0, min_idx = 0;
int comp_1 = -1, comp_2 = -1;

// Variáveis de Câmera e Resolução da Janela
float camera_angulo_X = 0.0f;
float camera_angulo_Y = 5.0f;
int largura_janela = 1024;
int altura_janela = 768;

// Configuração de Textura Procedimental Matemática
#define TEXTURA_LARGURA 64
#define TEXTURA_ALTURA 64
GLubyte imagem_textura[TEXTURA_ALTURA][TEXTURA_LARGURA][4];
GLuint id_textura;

// Aloca os valores aleatórios iniciais e zera os ponteiros de controle
void resetarVetorEDados() {
    for (int i = 0; i < TAMANHO_VETOR; i++) {
        vetor[i] = (rand() % VALOR_MAXIMO) + 1;
        ordenado[i] = false;
    }
    i_bubble = 0; j_bubble = 0;
    i_select = 0; j_select = 0; min_idx = 0;
    comp_1 = -1;  comp_2 = -1;
    concluido = false;
    simulacao_ativa = false; // Começa sempre parado!
}

void gerarTexturaProcedimental() {
    for (int i = 0; i < TEXTURA_ALTURA; i++) {
        for (int j = 0; j < TEXTURA_LARGURA; j++) {
            GLubyte c = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) ? 240 : 180;
            imagem_textura[i][j][0] = c; 
            imagem_textura[i][j][1] = c; 
            imagem_textura[i][j][2] = c; 
            imagem_textura[i][j][3] = 255; 
        }
    }
    glGenTextures(1, &id_textura);
    glBindTexture(GL_TEXTURE_2D, id_textura);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURA_LARGURA, TEXTURA_ALTURA, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagem_textura);
}

void configurarIluminacao() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat posicao_luz[] = { 0.0f, 12.0f, 6.0f, 1.0f };
    GLfloat luz_ambiente[] = { 0.25f, 0.25f, 0.25f, 1.0f };
    GLfloat luz_difusa[]   = { 0.85f, 0.85f, 0.85f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);
    glLightfv(GL_LIGHT0, GL_AMBIENT, luz_ambiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa);
}

// Renderizador de Strings em 2D sobreposto à cena 3D (HUD)
void renderizarTextoHUD(float x, float y, const char* texto, void* fonte) {
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glRasterPos2f(x, y);
    while (*texto) {
        glutBitmapCharacter(fonte, *texto);
        texto++;
    }
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
}

// Interface gráfica tridimensional do Menu Principal
void desenharMenuPrincipal() {
    glColor3f(1.0f, 1.0f, 1.0f);
    renderizarTextoHUD(-0.35f, 0.6f, "MOTOR DE ORDENACAO", GLUT_BITMAP_TIMES_ROMAN_24);
    renderizarTextoHUD(-0.22f, 0.45f, "Selecione a Representacao:", GLUT_BITMAP_HELVETICA_18);

    gluLookAt(0.0f, 0.0f, 8.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    // Renderiza Botão 3D 1 (Bubble Sort)
    glPushMatrix();
        glTranslatef(0.0f, 0.5f, 0.0f);
        glScalef(4.0f, 0.8f, 0.5f);
        glColor3f(0.2f, 0.4f, 0.7f);
        glutSolidCube(1.0f);
    glPopMatrix();
    renderizarTextoHUD(-0.11f, 0.1f, "1. BUBBLE SORT", GLUT_BITMAP_HELVETICA_12);

    // Renderiza Botão 3D 2 (Selection Sort)
    glPushMatrix();
        glTranslatef(0.0f, -1.0f, 0.0f);
        glScalef(4.0f, 0.8f, 0.5f);
        glColor3f(0.2f, 0.6f, 0.4f);
        glutSolidCube(1.0f);
    glPopMatrix();
    renderizarTextoHUD(-0.14f, -0.28f, "2. SELECTION SORT", GLUT_BITMAP_HELVETICA_12);
    
    renderizarTextoHUD(-0.25f, -0.7f, "[Clique com o mouse para selecionar]", GLUT_BITMAP_HELVETICA_12);
}

// Renderiza a simulação dos blocos quando um algoritmo está rodando
void desenharRepresentacao() {
    // Textos informativos da tela
    glColor3f(1.0f, 1.0f, 1.0f);
    if (estado_atual == ESTADO_BUBBLE) renderizarTextoHUD(-0.95f, 0.88f, "Algoritmo Ativo: BUBBLE SORT", GLUT_BITMAP_HELVETICA_18);
    else renderizarTextoHUD(-0.95f, 0.88f, "Algoritmo Ativo: SELECTION SORT", GLUT_BITMAP_HELVETICA_18);

    if (!simulacao_ativa && !concluido) renderizarTextoHUD(-0.95f, 0.80f, "STATUS: PARADO. [Aperte ESPACO para mover / 'M' para Menu]", GLUT_BITMAP_HELVETICA_12);
    else if (concluido) renderizarTextoHUD(-0.95f, 0.80f, "STATUS: ORDENADO! ['M' para retornar ao Menu]", GLUT_BITMAP_HELVETICA_12);
    else renderizarTextoHUD(-0.95f, 0.80f, "STATUS: EM MOVIMENTO... [Aperte ESPACO para pausar]", GLUT_BITMAP_HELVETICA_12);

    // Define a posição estática da Câmera controlada pelas setas do teclado
    gluLookAt(0.0f, camera_angulo_Y, 12.0f,  0.0f, 1.5f, 0.0f,  0.0f, 1.0f, 0.0f);
    glRotatef(camera_angulo_X, 0.0f, 1.0f, 0.0f);

    // Desenha o Cenário (1 para Mesa, 2 para Grid Dinâmico)
    if (cenario_atual == 1) {
        glColor3f(0.4f, 0.23f, 0.08f); // Madeira da mesa
        glPushMatrix();
            glTranslatef(0.0f, -0.2f, 0.0f);
            glScalef(11.0f, 0.4f, 4.0f);
            glutSolidCube(1.0f);
        glPopMatrix();
    } else {
        glColor3f(0.1f, 0.5f, 0.8f); // Grid Futurista Neon
        glLineWidth(2.0f);
        for(float i = -6.0f; i <= 6.0f; i += 0.5f) {
            glBegin(GL_LINES);
                glVertex3f(i, -0.01f, -2.0f); glVertex3f(i, -0.01f, 2.0f);
                glVertex3f(-6.0f, -0.01f, i/3.0f); glVertex3f(6.0f, -0.01f, i/3.0f);
            glEnd();
        }
    }

    // Desenha os Blocos 3D com texturas procedimentais
    float largura_bloco = 9.0f / TAMANHO_VETOR;
    float inicio_x = -4.5f;

    for (int i = 0; i < TAMANHO_VETOR; i++) {
        float x_pos = inicio_x + (i * largura_bloco) + (largura_bloco / 2.0f);
        float escala_altura = ((float)vetor[i] / VALOR_MAXIMO) * 3.5f;

        glPushMatrix();
            glTranslatef(x_pos, escala_altura / 2.0f, 0.0f);
            glScalef(largura_bloco * 0.85f, escala_altura, 0.4f);

            if (i == comp_1 || i == comp_2) {
                glDisable(GL_TEXTURE_2D);
                glColor3f(1.0f, 0.2f, 0.2f); // Ativos em comparação (Vermelho)
            } else if (ordenado[i]) {
                glDisable(GL_TEXTURE_2D);
                glColor3f(0.2f, 0.8f, 0.3f); // Corretamente ordenados (Verde)
            } else {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, id_textura);
                if (cenario_atual == 1) glColor3f(0.8f, 0.65f, 0.5f);
                else glColor3f(0.3f, 0.6f, 1.0f);
            }

            glutSolidCube(1.0f);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}

void desenharCena() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (estado_atual == ESTADO_MENU) {
        desenharMenuPrincipal();
    } else {
        desenharRepresentacao();
    }

    glutSwapBuffers();
}

void rotinaTimer(int valor) {
    if (simulacao_ativa && !concluido) {
        if (estado_atual == ESTADO_BUBBLE) {
            passoBubbleSort(vetor, ordenado, TAMANHO_VETOR, i_bubble, j_bubble, comp_1, comp_2, concluido);
        } else if (estado_atual == ESTADO_SELECTION) {
            passoSelectionSort(vetor, ordenado, TAMANHO_VETOR, i_select, j_select, min_idx, comp_1, comp_2, concluido);
        }
    }
    glutPostRedisplay();
    glutTimerFunc(VELOCIDADE_MS, rotinaTimer, 0);
}


// Detecta os cliques nos Botões 3D do Menu Principal
void gerenciarCliqueMouse(int botao, int estado, int x, int y) {
    if (estado_atual == ESTADO_MENU && botao == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {
        float mouse_x = (float)x / largura_janela;
        float mouse_y = (float)(altura_janela - y) / altura_janela;

        // Clique no Botão do Bubble Sort
        if (mouse_x >= 0.3f && mouse_x <= 0.7f && mouse_y >= 0.52f && mouse_y <= 0.62f) {
            estado_atual = ESTADO_BUBBLE;
            resetarVetorEDados();
        }
        // Clique no Botão do Selection Sort
        else if (mouse_x >= 0.3f && mouse_x <= 0.7f && mouse_y >= 0.32f && mouse_y <= 0.42f) {
            estado_atual = ESTADO_SELECTION;
            resetarVetorEDados();
        }
    }
}

void gerenciarTeclado(unsigned char tecla, int x, int y) {
    switch (tecla) {
        case 32: // ESPAÇO: Liga/Desliga o movimento dos blocos
            if (estado_atual != ESTADO_MENU) simulacao_ativa = !simulacao_ativa;
            break;
        case 'm': case 'M': // M: Volta para a tela do Menu
            estado_atual = ESTADO_MENU;
            simulacao_ativa = false;
            break;
        case 'c': case 'C': // C: Troca de Cenário
            cenario_atual = (cenario_atual == 1) ? 2 : 1;
            break;
        case 'r': case 'R': // R: Reseta o algoritmo atual (Mantém parado)
            if (estado_atual != ESTADO_MENU) resetarVetorEDados();
            break;
        case 27: // ESC: Fecha o programa
            exit(0);
            break;
    }
}

// Permite movimentar a câmera 3D usando as setas direcionais do teclado
void gerenciarTeclasEspeciais(int tecla, int x, int y) {
    if (estado_atual != ESTADO_MENU) {
        switch (tecla) {
            case GLUT_KEY_LEFT:  camera_angulo_X -= 3.0f; break;
            case GLUT_KEY_RIGHT: camera_angulo_X += 3.0f; break;
            case GLUT_KEY_UP:    camera_angulo_Y += 0.4f; break;
            case GLUT_KEY_DOWN:  camera_angulo_Y -= 0.4f; break;
        }
    }
}

void reiniciarJanela(int largura, int altura) {
    largura_janela = largura;
    altura_janela = altura;
    if (altura == 0) altura = 1;
    glViewport(0, 0, largura, altura);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLdouble)largura / (GLdouble)altura, 1.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    srand(time(NULL));
    resetarVetorEDados();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(largura_janela, altura_janela);
    glutCreateWindow("Modelo de Ordenacao");

    glEnable(GL_DEPTH_TEST);
    configurarIluminacao();
    gerarTexturaProcedimental();

    // Associação de Callbacks do GLUT
    glutDisplayFunc(desenharCena);
    glutReshapeFunc(reiniciarJanela);
    glutKeyboardFunc(gerenciarTeclado);
    glutSpecialFunc(gerenciarTeclasEspeciais);
    glutMouseFunc(gerenciarCliqueMouse);
    glutTimerFunc(VELOCIDADE_MS, rotinaTimer, 0);

    glClearColor(0.06f, 0.07f, 0.1f, 1.0f);

    glutMainLoop();
    return 0;
}