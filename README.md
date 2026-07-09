# Modelo de Ordenação

## Contexto Acadêmico

Este sistema faz parte do **Projeto Interdisciplinar** que promove a integração curricular entre as disciplinas de **Programação Orientada a Objetos (POO)** e **Computação Gráfica (CG)** do Departamento de Ciência da Computação da **Universidade Federal de Rondônia (UNIR)**, sob a orientação do **Prof. Dr. Lucas Marques da Cunha**.

O objetivo do ecossistema é simular uma arquitetura multicamadas através da separação estrita de responsabilidades: uma camada de controle lógico e um motor gráfico nativo de baixo nível focado em visualização tridimensional e no ensino de algoritmos de ordenação.

---

# Funcionalidades do Motor Gráfico (CG)

O motor foi desenvolvido em **C++ utilizando OpenGL (FreeGLUT)** e implementa:

- **Menu 3D Interativo:** Tela inicial com seleção dos algoritmos através de botões tridimensionais clicáveis.
- **Visualização Tridimensional dos Algoritmos:** Representação gráfica da execução dos algoritmos de ordenação em tempo real.
- **Controle Estrito de Animação (FSM):** A execução permanece pausada até que o usuário inicie a simulação.
- **Pipeline de Iluminação:** Utilização de iluminação ambiente e difusa para melhor percepção dos elementos 3D.
- **Texturização Procedimental:** Geração de texturas sem dependência de arquivos externos.
- **Múltiplos Cenários:** Alternância entre diferentes ambientes de visualização.
- **Controle de Velocidade:** Permite alternar entre três velocidades de execução da simulação.
- **Câmera Interativa:** Movimentação da câmera para diferentes ângulos de observação.
- **Execução Passo a Passo** dos algoritmos:
  - Bubble Sort
  - Selection Sort
  - Insertion Sort

---

# Tecnologias Utilizadas

- C++
- OpenGL
- FreeGLUT
- GLUT
- g++
- Homebrew (macOS)

---

# Estrutura do Projeto

O projeto está organizado da seguinte forma:

```text
projetoFinal/
│
├── motor.cpp
├── bubble.cpp
├── selection.cpp
├── insertion.cpp
├── algoritmos.h
└── README.md
```

### Descrição dos Arquivos

#### `motor.cpp`

- Motor gráfico 3D.
- Interface do usuário.
- Renderização.
- Controle da câmera.
- Controle dos cenários.
- Entrada do teclado e mouse.
- Gerenciamento da execução dos algoritmos.

#### `bubble.cpp`

Implementação do algoritmo **Bubble Sort** em execução passo a passo.

#### `selection.cpp`

Implementação do algoritmo **Selection Sort** em execução passo a passo.

#### `insertion.cpp`

Implementação do algoritmo **Insertion Sort** em execução passo a passo.

#### `algoritmos.h`

Arquivo de cabeçalho contendo as assinaturas das funções utilizadas pelos algoritmos.

---

# Algoritmos Implementados

Atualmente o projeto possui três algoritmos de ordenação:

- Bubble Sort
- Selection Sort
- Insertion Sort

Cada algoritmo pode ser selecionado através do menu principal e executado de forma visual, permitindo acompanhar cada etapa da ordenação.

---

# Controles da Aplicação

## Menu Principal

- **Clique esquerdo do mouse**
  - Seleciona o algoritmo desejado:
    - Bubble Sort
    - Selection Sort
    - Insertion Sort

## Tela de Visualização

### Barra de Espaço

Inicia ou pausa a simulação.

### Setas Direcionais

- ⬅️ Rotaciona a câmera para a esquerda.
- ➡️ Rotaciona a câmera para a direita.
- ⬆️ Eleva a câmera.
- ⬇️ Abaixa a câmera.

### Tecla C

Alterna entre os cenários disponíveis.

### Tecla R

Gera um novo conjunto de valores aleatórios e reinicia a simulação.

### Tecla M

Retorna ao Menu Principal.

### Clique no botão de velocidade

Alterna entre:

- Lento
- Médio
- Rápido

### Tecla ESC

Fecha a aplicação.

---

# Como Compilar e Executar (Linux)

Certifique-se de possuir o compilador **g++** e as bibliotecas de desenvolvimento do OpenGL instaladas.

```bash
# 1. Instalar dependências
sudo apt-get update
sudo apt-get install freeglut3-dev mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev

# 2. Navegar até a pasta do projeto
cd ~/OpenGL_Project/projetoFinal

# 3. Compilar
g++ motor.cpp bubble.cpp selection.cpp insertion.cpp \
-o motor \
-lglut \
-lGL \
-lGLU

# 4. Executar
./motor
```

---

# Como Compilar e Executar (macOS)

🍏 **Ambiente de Desenvolvimento**

Certifique-se de possuir o **Homebrew** instalado.

```bash
# 1. Instalar o FreeGLUT
brew install freeglut

# 2. Navegar até a pasta do projeto
cd "/CAMINHO/DO/SEU/projetoFinal"

# Exemplo:
# cd "/Users/SEU_USUARIO/Documents/projetoFinal"

# 3. Compilar
g++ motor.cpp bubble.cpp selection.cpp insertion.cpp \
-I/opt/homebrew/include \
-L/opt/homebrew/lib \
-framework OpenGL \
-framework GLUT \
-o motor

# 4. Executar
./motor
```

> **Observação:** Em Macs Intel o Homebrew normalmente é instalado em `/usr/local`, enquanto em Macs Apple Silicon (M1, M2, M3 e posteriores) o caminho padrão é `/opt/homebrew`. Caso necessário, ajuste os parâmetros `-I` e `-L`.

---

# Como Utilizar

1. Execute a aplicação.
2. No menu principal, escolha um algoritmo.
3. Pressione **Barra de Espaço** para iniciar a animação.
4. Utilize as **setas direcionais** para movimentar a câmera.
5. Utilize a tecla **C** para alterar o cenário.
6. Utilize o botão de velocidade para alternar entre os modos **Lento**, **Médio** e **Rápido**.
7. Pressione **R** para gerar um novo conjunto de dados.
8. Pressione **M** para retornar ao menu principal.
9. Pressione **ESC** para encerrar o programa.

---

# Demonstração da Ferramenta

## Vídeo de Demonstração

> **Link do vídeo:**  
> **https://_______________________________**

---

# Repositório do Projeto

> **Link do GitHub:**  
> **https://github.com/fabirmss/projetoFinal**

---

# Autores

- _Fabiana Ramos____________
- _Gabriel Barros__________
- _Giulia Correa____________
- _Higor______

---

# Licença

Este projeto foi desenvolvido para fins acadêmicos como parte do Projeto Interdisciplinar do Curso de Ciência da Computação da **Universidade Federal de Rondônia (UNIR)**.
