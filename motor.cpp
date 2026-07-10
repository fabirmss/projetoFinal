#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/freeglut.h>
#endif
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm> 
#include <thread>
#include "algoritmos.h"
#include "model_MotorOrdenacao.h"

#define TAMANHO_VETOR 40
#define VALOR_MAXIMO 50

int velocidade_atual_ms = 40;
enum TipoVelocidade { LENTO, MEDIO, RAPIDO };
TipoVelocidade modo_velocidade = MEDIO;

enum EstadoSistema { ESTADO_MENU, ESTADO_BUBBLE, ESTADO_SELECTION, ESTADO_INSERTION };
EstadoSistema estado_atual = ESTADO_MENU;

std::vector<int> vetor(TAMANHO_VETOR);
std::vector<bool> ordenado(TAMANHO_VETOR, false);

int cenario_atual = 1;    
bool simulacao_ativa = false; 
bool concluido = false;

int i_bubble = 0, j_bubble = 0;
int i_select = 0, j_select = 0, min_idx = 0;
int i_insert = 1, j_insert = 0, chave_insert = 0;
bool pegar_chave = true;
int comp_1 = -1, comp_2 = -1;

float camera_angulo_X = 0.0f;
float camera_angulo_Y = 5.0f;
int largura_janela = 1024;
int altura_janela = 768;

#define TEXTURA_LARGURA 64
#define TEXTURA_ALTURA 64
GLubyte imagem_textura[TEXTURA_ALTURA][TEXTURA_LARGURA][4];
GLuint id_textura;

std::thread thread_glut;
bool glut_inicializado = false;

void passoBubbleSort() {
    if (i_bubble < TAMANHO_VETOR - 1) {
        if (j_bubble < TAMANHO_VETOR - i_bubble - 1) {
            comp_1 = j_bubble; comp_2 = j_bubble + 1;
            if (vetor[j_bubble] > vetor[j_bubble + 1]) std::swap(vetor[j_bubble], vetor[j_bubble + 1]);
            j_bubble++; 
        } else {
            ordenado[TAMANHO_VETOR - i_bubble - 1] = true;
            j_bubble = 0; i_bubble++;    
        }
    } else {
        ordenado[0] = true; comp_1 = -1; comp_2 = -1; concluido = true; simulacao_ativa = false;
    }
}

void passoSelectionSort() {
    if (i_select < TAMANHO_VETOR - 1) {
        if (j_select == 0) { min_idx = i_select; j_select = i_select + 1; }
        if (j_select < TAMANHO_VETOR) {
            comp_1 = j_select; comp_2 = min_idx;
            if (vetor[j_select] < vetor[min_idx]) min_idx = j_select;
            j_select++; 
        } else {
            if (min_idx != i_select) std::swap(vetor[i_select], vetor[min_idx]);
            ordenado[i_select] = true; i_select++; j_select = 0;              
        }
    } else {
        ordenado[TAMANHO_VETOR - 1] = true; comp_1 = -1; comp_2 = -1; concluido = true; simulacao_ativa = false;
    }
}

void passoInsertionSort() {
    passoInsertionSort(vetor, ordenado, TAMANHO_VETOR, i_insert, j_insert, chave_insert, pegar_chave, concluido, simulacao_ativa);
}

void resetarVetorEDados() {
    i_bubble = 0; j_bubble = 0;
    i_select = 0; j_select = 0; min_idx = 0;
    i_insert = 1; j_insert = 0; pegar_chave = true;
    comp_1 = -1;  comp_2 = -1;
    concluido = false;
    simulacao_ativa = false;
}

void gerarTexturaProcedimental() {
    for (int i = 0; i < TEXTURA_ALTURA; i++) {
        for (int j = 0; j < TEXTURA_LARGURA; j++) {
            GLubyte c = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) ? 240 : 180;
            imagem_textura[i][j][0] = c; imagem_textura[i][j][1] = c; imagem_textura[i][j][2] = c; imagem_textura[i][j][3] = 255; 
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
    glEnable(GL_LIGHTING); glEnable(GL_LIGHT0); glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    GLfloat posicao_luz[] = { 0.0f, 12.0f, 6.0f, 1.0f };
    GLfloat luz_ambiente[] = { 0.25f, 0.25f, 0.25f, 1.0f };
    GLfloat luz_difusa[]   = { 0.85f, 0.85f, 0.85f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);
    glLightfv(GL_LIGHT0, GL_AMBIENT, luz_ambiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa);
}

void renderizarTextoHUD(float x, float y, const char* texto, void* fonte) {
    glDisable(GL_LIGHTING); glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glRasterPos2f(x, y);
    while (*texto) { glutBitmapCharacter(fonte, *texto); texto++; }
    glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW); glEnable(GL_LIGHTING);
}

void desenharMenuPrincipal() {
    glColor3f(1.0f, 1.0f, 1.0f);
    renderizarTextoHUD(-0.35f, 0.6f, "Modelo de Ordenacao", GLUT_BITMAP_TIMES_ROMAN_24);
    renderizarTextoHUD(-0.22f, 0.45f, "Selecione a Representacao:", GLUT_BITMAP_HELVETICA_18);
    gluLookAt(0.0f, 0.0f, 8.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    glPushMatrix(); glTranslatef(0.0f, 0.8f, 0.0f); glScalef(4.0f, 0.7f, 0.5f); glColor3f(0.2f, 0.4f, 0.7f); glutSolidCube(1.0f); glPopMatrix();
    renderizarTextoHUD(-0.11f, 0.17f, "1. BUBBLE SORT", GLUT_BITMAP_HELVETICA_12);
    glPushMatrix(); glTranslatef(0.0f, -0.4f, 0.0f); glScalef(4.0f, 0.7f, 0.5f); glColor3f(0.2f, 0.6f, 0.4f); glutSolidCube(1.0f); glPopMatrix();
    renderizarTextoHUD(-0.14f, -0.12f, "2. SELECTION SORT", GLUT_BITMAP_HELVETICA_12);
    glPushMatrix(); glTranslatef(0.0f, -1.6f, 0.0f); glScalef(4.0f, 0.7f, 0.5f); glColor3f(0.6f, 0.3f, 0.7f); glutSolidCube(1.0f); glPopMatrix();
    renderizarTextoHUD(-0.13f, -0.42f, "3. INSERTION SORT", GLUT_BITMAP_HELVETICA_12);
    renderizarTextoHUD(-0.25f, -0.85f, "[Clique com o Mouse para selecionar]", GLUT_BITMAP_HELVETICA_12);
}

void desenharRepresentacao() {
    glColor3f(1.0f, 1.0f, 1.0f);
    if (estado_atual == ESTADO_BUBBLE) renderizarTextoHUD(-0.95f, 0.88f, "Algoritmo Ativo: BUBBLE SORT", GLUT_BITMAP_HELVETICA_18);
    else if (estado_atual == ESTADO_SELECTION) renderizarTextoHUD(-0.95f, 0.88f, "Algoritmo Ativo: SELECTION SORT", GLUT_BITMAP_HELVETICA_18);
    else renderizarTextoHUD(-0.95f, 0.88f, "Algoritmo Ativo: INSERTION SORT", GLUT_BITMAP_HELVETICA_18);

    if (!simulacao_ativa && !concluido) renderizarTextoHUD(-0.95f, 0.80f, "STATUS: PARADO. [Aperte ESPACO para mover / 'M' para Menu]", GLUT_BITMAP_HELVETICA_12);
    else if (concluido) renderizarTextoHUD(-0.95f, 0.80f, "STATUS: ORDENADO! ['M' para retornar ao Menu]", GLUT_BITMAP_HELVETICA_12);
    else renderizarTextoHUD(-0.95f, 0.80f, "STATUS: EM MOVIMENTO... [Aperte ESPACO para pausar]", GLUT_BITMAP_HELVETICA_12);

    gluLookAt(0.0f, camera_angulo_Y, 12.0f,  0.0f, 1.5f, 0.0f,  0.0f, 1.0f, 0.0f);
    glRotatef(camera_angulo_X, 0.0f, 1.0f, 0.0f);

    if (cenario_atual == 1) {
        glColor3f(0.4f, 0.23f, 0.08f); glPushMatrix(); glTranslatef(0.0f, -0.2f, 0.0f); glScalef(11.0f, 0.4f, 4.0f); glutSolidCube(1.0f); glPopMatrix();
    } else {
        glColor3f(0.1f, 0.5f, 0.8f); glLineWidth(2.0f);
        for(float i = -6.0f; i <= 6.0f; i += 0.5f) {
            glBegin(GL_LINES); glVertex3f(i, -0.01f, -2.0f); glVertex3f(i, -0.01f, 2.0f); glVertex3f(-6.0f, -0.01f, i/3.0f); glVertex3f(6.0f, -0.01f, i/3.0f); glEnd();
        }
    }

    float largura_bloco = 9.0f / TAMANHO_VETOR;
    float inicio_x = -4.5f;

    for (int i = 0; i < TAMANHO_VETOR; i++) {
        float x_pos = inicio_x + (i * largura_bloco) + (largura_bloco / 2.0f);
        float escala_altura = ((float)vetor[i] / VALOR_MAXIMO) * 3.5f;
        glPushMatrix();
            glTranslatef(x_pos, escala_altura / 2.0f, 0.0f);
            glScalef(largura_bloco * 0.85f, escala_altura, 0.4f);
            if (i == comp_1 || i == comp_2) { glDisable(GL_TEXTURE_2D); glColor3f(1.0f, 0.2f, 0.2f); } 
            else if (ordenado[i]) { glDisable(GL_TEXTURE_2D); glColor3f(0.2f, 0.8f, 0.3f); } 
            else { glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, id_textura); if (cenario_atual == 1) glColor3f(0.8f, 0.65f, 0.5f); else glColor3f(0.3f, 0.6f, 1.0f); }
            glutSolidCube(1.0f); glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }

    glPushMatrix();
        glTranslatef(4.0f, 4.5f, -1.0f); glRotatef(25.0f, 1.0f, 0.0f, 0.0f); glRotatef(-20.0f, 0.0f, 1.0f, 0.0f);
        glPushMatrix(); glScalef(2.2f, 0.7f, 0.2f); glColor3f(0.15f, 0.15f, 0.18f); glutSolidCube(1.0f); glPopMatrix();
        glPushMatrix(); glTranslatef(0.0f, 0.0f, 0.15f); glScalef(1.9f, 0.5f, 0.2f);
            if (modo_velocidade == LENTO) glColor3f(0.9f, 0.6f, 0.1f);      
            else if (modo_velocidade == MEDIO) glColor3f(0.1f, 0.6f, 0.9f);      
            else glColor3f(0.9f, 0.1f, 0.2f);      
            glutSolidCube(1.0f); 
        glPopMatrix();
    glPopMatrix();

    renderizarTextoHUD(0.55f, 0.90f, "CONTROLE DE VELOCIDADE", GLUT_BITMAP_HELVETICA_10);
    if (modo_velocidade == LENTO) renderizarTextoHUD(0.55f, 0.83f, "STATUS: >> LENTO (150ms)", GLUT_BITMAP_HELVETICA_12);
    else if (modo_velocidade == MEDIO) renderizarTextoHUD(0.55f, 0.83f, "STATUS: >> MEDIO (40ms)", GLUT_BITMAP_HELVETICA_12);
    else renderizarTextoHUD(0.55f, 0.83f, "STATUS: >> RAPIDO (5ms)", GLUT_BITMAP_HELVETICA_12);
    renderizarTextoHUD(0.55f, 0.77f, "[Clique no Botao Flutuante p/ Alterar]", GLUT_BITMAP_HELVETICA_10);
}

void desenharCena() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); glLoadIdentity();
    if (estado_atual == ESTADO_MENU) desenharMenuPrincipal(); else desenharRepresentacao();
    glutSwapBuffers();
}

void rotinaTimer(int valor) {
    if (simulacao_ativa && !concluido) {
        if (estado_atual == ESTADO_BUBBLE) passoBubbleSort(); 
        else if (estado_atual == ESTADO_SELECTION) passoSelectionSort(); 
        else if (estado_atual == ESTADO_INSERTION) passoInsertionSort();
    }
    glutPostRedisplay();
    glutTimerFunc(velocidade_atual_ms, rotinaTimer, 0); 
}

void gerenciarCliqueMouse(int botao, int estado, int x, int y) {
    float mouse_x = (float)x / largura_janela; float mouse_y = (float)(altura_janela - y) / altura_janela;
    if (estado_atual == ESTADO_MENU && botao == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {
        if (mouse_x >= 0.3f && mouse_x <= 0.7f && mouse_y >= 0.56f && mouse_y <= 0.65f) { estado_atual = ESTADO_BUBBLE; resetarVetorEDados(); }
        else if (mouse_x >= 0.3f && mouse_x <= 0.7f && mouse_y >= 0.40f && mouse_y <= 0.49f) { estado_atual = ESTADO_SELECTION; resetarVetorEDados(); }
        else if (mouse_x >= 0.3f && mouse_x <= 0.7f && mouse_y >= 0.24f && mouse_y <= 0.33f) { estado_atual = ESTADO_INSERTION; resetarVetorEDados(); }
    } 
    else if (estado_atual != ESTADO_MENU && botao == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {
        if (mouse_x >= 0.60f && mouse_x <= 0.98f && mouse_y >= 0.70f && mouse_y <= 0.96f) {
            if (modo_velocidade == LENTO) { modo_velocidade = MEDIO; velocidade_atual_ms = 40; } 
            else if (modo_velocidade == MEDIO) { modo_velocidade = RAPIDO; velocidade_atual_ms = 5; } 
            else { modo_velocidade = LENTO; velocidade_atual_ms = 150; }
            glutPostRedisplay(); 
        }
    }
}

void gerenciarTeclado(unsigned char tecla, int x, int y) {
    switch (tecla) {
        case 32: if (estado_atual != ESTADO_MENU) simulacao_ativa = !simulacao_ativa; break;
        case 'm': case 'M': estado_atual = ESTADO_MENU; simulacao_ativa = false; break;
        case 'c': case 'C': cenario_atual = (cenario_atual == 1) ? 2 : 1; break;
        case 'r': case 'R': if (estado_atual != ESTADO_MENU) resetarVetorEDados(); break;
        case 27: exit(0); break;
    }
}

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
    largura_janela = largura; altura_janela = altura; if (altura == 0) altura = 1;
    glViewport(0, 0, largura, altura); glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(45.0, (GLdouble)largura / (GLdouble)altura, 1.0, 30.0); glMatrixMode(GL_MODELVIEW);
}

void rodarCicloJanelaNativa() {
    int argc = 1;
    char* argv[] = {(char*)"motor"};
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(largura_janela, altura_janela);
    glutCreateWindow("Modelos de Ordenacao - CG 3D");

    glEnable(GL_DEPTH_TEST);
    configurarIluminacao();
    gerarTexturaProcedimental();

    glutDisplayFunc(desenharCena);
    glutReshapeFunc(reiniciarJanela);
    glutKeyboardFunc(gerenciarTeclado);
    glutSpecialFunc(gerenciarTeclasEspeciais);
    glutMouseFunc(gerenciarCliqueMouse);
    glutTimerFunc(velocidade_atual_ms, rotinaTimer, 0);

    glClearColor(0.06f, 0.07f, 0.1f, 1.0f);
    glutMainLoop();
}


JNIEXPORT void JNICALL Java_model_MotorOrdenacao_init
  (JNIEnv *env, jobject obj) {
    if (!glut_inicializado) {
        glut_inicializado = true;
        thread_glut = std::thread(rodarCicloJanelaNativa);
        thread_glut.detach(); 
    }
}

JNIEXPORT void JNICALL Java_model_MotorOrdenacao_ordenar
  (JNIEnv *env, jobject obj, jintArray vetorJava, jstring algoritmoJava) {
    
    jsize tamanho = env->GetArrayLength(vetorJava);
    jint* dados = env->GetIntArrayElements(vetorJava, nullptr);
    
    for (int i = 0; i < TAMANHO_VETOR && i < tamanho; i++) {
        vetor[i] = dados[i];
        ordenado[i] = false;
    }
    env->ReleaseIntArrayElements(vetorJava, dados, JNI_ABORT);
    
    const char* algoritmoStr = env->GetStringUTFChars(algoritmoJava, nullptr);
    std::string algo(algoritmoStr);
    
    resetarVetorEDados();
    if (algo == "Bubble Sort") {
        estado_atual = ESTADO_BUBBLE;
    } else if (algo == "Selection Sort") {
        estado_atual = ESTADO_SELECTION;
    } else if (algo == "Insertion Sort") {
        estado_atual = ESTADO_INSERTION;
    }
    
    env->ReleaseStringUTFChars(algoritmoJava, algoritmoStr);
    simulacao_ativa = true; 
}

JNIEXPORT void JNICALL Java_model_MotorOrdenacao_cleanup
  (JNIEnv *env, jobject obj) {
    exit(0);
}