# Modelo de Ordenação

## Contexto Acadêmico

Este sistema faz parte do **Projeto Interdisciplinar** que promove a integração curricular entre as disciplinas de **Programação Orientada a Objetos (POO)** e **Computação Gráfica (CG)** do Departamento de Ciência da Computação da **Universidade Federal de Rondônia (UNIR)**, sob a orientação do Prof. Dr. Lucas Marques da Cunha.

O objetivo do ecossistema é simular uma arquitetura multicamadas através da separação estrita de responsabilidades: uma camada de controle lógico e um motor gráfico nativo de baixo nível focado em pipeline de renderização avançado.

---

# Funcionalidades do Motor Gráfico (CG)

O motor foi desenvolvido em **C++ utilizando OpenGL (FreeGLUT)** e implementa:

- **Menu 3D Interativo:** Tela de inicialização com botões tridimensionais selecionáveis via clique do mouse (mapeamento de coordenadas de tela para o espaço 3D).
- **Controle Estrito de Animação (FSM):** Os algoritmos iniciam em estado **estático (congelado)** para fins pedagógicos e avançam passo a passo por frame quando comandados.
- **Pipeline de Iluminação Fixo:** Configuração de luz ambiente e difusa para sombreamento dos blocos tridimensionais.
- **Texturização Procedimental:** Geração matemática de texturas (xadrez e listras) diretamente na memória de vídeo, eliminando dependências externas de imagens.
- **Múltiplos Cenários:** Alternância em tempo real entre uma mesa de madeira acadêmica e um grid digital em estilo *Tron*.
- **Câmera Interativa:** Controle manual de órbita e elevação da câmera ao redor do vetor tridimensional.

---

# Estrutura do Projeto

Visando facilitar a compilação e execução nos computadores do laboratório, o motor gráfico e os algoritmos de ordenação foram consolidados em um único módulo integrado.

### Arquivo principal

- **`motor.cpp`**
  - Interface gráfica 3D.
  - Motor de renderização.
  - Algoritmos de ordenação (Bubble Sort e Selection Sort).
  - Controle de animações e interação do usuário.

---

# Controles da Aplicação

## Menu Principal

- **Clique esquerdo do mouse**
  - Seleciona o algoritmo de ordenação (Bubble Sort ou Selection Sort).

## Tela de Visualização

- **Barra de Espaço**
  - Inicia/Pausa a animação da ordenação.

- **Setas direcionais (⬅️ ➡️ ⬆️ ⬇️)**
  - Movimentam a câmera.

- **Tecla C**
  - Alterna o cenário (Mesa de Madeira ↔ Grid Neon).

- **Tecla R**
  - Gera um novo vetor aleatório e retorna ao estado parado.

- **Tecla M**
  - Retorna ao Menu Principal.

- **Tecla ESC**
  - Fecha a aplicação.

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
g++ -o motor motor.cpp -lglut -lGL -lGLU

# 4. Executar
./motor
```

---

# Como Compilar e Executar (macOS)

🍏 **Ambiente de Desenvolvimento do Grupo**

Certifique-se de possuir o gerenciador de pacotes **Homebrew** instalado no macOS para gerenciar as dependências do projeto.

```bash
# 1. Instalar o FreeGLUT via Homebrew
brew install freeglut

# 2. Navegar até a pasta do projeto
cd "/CG-TRABALHO FINAL/projetoFinal"

# 3. Compilar
g++ motor.cpp \
-I/opt/homebrew/include \
-L/opt/homebrew/lib \
-framework OpenGL \
-framework GLUT \
-o motor

# 4. Executar
./motor
```

> **Observação:** Em Macs com processador Intel, o Homebrew normalmente é instalado em `/usr/local`, enquanto nos Macs com Apple Silicon (M1, M2, M3 e posteriores) o caminho padrão é `/opt/homebrew`. Caso necessário, ajuste os parâmetros `-I` e `-L` conforme a instalação do Homebrew.