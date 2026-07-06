# Modelo de Ordenação

## Contexto Acadêmico
[cite_start]Este sistema faz parte do **Projeto Interdisciplinar** que promove a integração curricular entre as disciplinas de **Programação Orientada a Objetos (POO)** e **Computação Gráfica (CG)** do Departamento de Ciência da Computação da **Universidade Federal de Rondônia (UNIR)**, sob a orientação do Prof. Dr. Lucas Marques da Cunha[cite: 1, 3, 4, 5, 6, 8].

[cite_start]O objetivo do ecossistema é simular uma arquitetura multicamadas através da separação estrita de responsabilidades: uma camada de controle lógico e um motor gráfico nativo de baixo nível focado em pipeline de renderização avançado[cite: 19, 20, 52].

---

## Funcionalidades do Motor Gráfico (CG)
O motor foi desenvolvido em **C++ utilizando OpenGL (FreeGLUT)** e implementa:
* **Menu 3D Interativo:** Tela de inicialização com botões tridimensionais selecionáveis via clique do mouse (Mapeamento de coordenadas de tela para o Espaço 3D).
* [cite_start]**Controle Estrito de Animação (FSM):** Os algoritmos iniciam em estado **estático (congelado)** para fins pedagógicos e avançam passo a passo por frame quando comandados[cite: 97].
* [cite_start]**Pipeline de Iluminação Fixo:** Configuração de luz ambiente e difusa de estúdio para sombreamento realista dos blocos tridimensionais[cite: 67].
* **Texturização Procedimental:** Geração matemática de mapa de bits (padrão xadrez/stripes) na memória de vídeo, mitigando dependências externas de arquivos de imagem.
* **Múltiplos Cenários:** Alternância em tempo real entre uma Mesa de Madeira Acadêmica e um Grid Abstrato Digital (Estilo *Tron*).
* **Câmera Interativa:** Controle manual de órbita e elevação do ponto de vista sobre o vetor tridimensional.

---

## Estrutura do Projeto
Visando a simplicidade de compilação e transporte para os computadores do laboratório, o motor e as lógicas de ordenação foram consolidados em um único módulo integrado:

* [cite_start]**`motor.cpp`**: Arquivo único contendo a lógica gráfica, interface 3D e os modelos de ordenação (Bubble Sort e Selection Sort)[cite: 65, 104].

---

## Controles da Janela

### No Menu Principal
* `Clique Esquerdo do Mouse` sobre os botões 3D para selecionar o algoritmo de ordenação (Bubble Sort ou Selection Sort).

### Na Tela de Representação (Visualização)
* [cite_start]`BARRA DE ESPAÇO` -> Inicia / Pausa o movimento físico e lógico de ordenação dos blocos[cite: 97, 108].
* `SETAS DIRECIONAIS` (⬅️ ➡️ ⬆️ ⬇️) -> Movem a câmera dinamicamente ao redor do cenário 3D.
* `Tecla C / c` -> Altera o cenário de fundo (Mesa de Madeira vs Grid Neon).
* `Tecla R / r` -> Reseta o vetor com novos valores aleatórios (o sistema volta ao estado Parado).
* `Tecla M / m` -> Limpa os buffers e retorna para o Menu Principal 3D.
* `Tecla ESC` -> Encerra o programa limpando a memória de vídeo de forma segura.

---

## Como Compilar e Executar (Linux)

Certifique-se de possuir o compilador `g++` e as bibliotecas de desenvolvimento do OpenGL instaladas na sua distribuição Linux (Ubuntu/Debian):

```bash
# 1. Instalar dependências de Computação Gráfica (se necessário)
sudo apt-get update
sudo apt-get install freeglut3-dev mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev

# 2. Navegar até a pasta do projeto
cd ~/OpenGL_Project/projetoFinal

# 3. Compilar o arquivo único unificado
g++ -o motor motor.cpp -lglut -lGL -lGLU

# 4. Executar a aplicação
./motor