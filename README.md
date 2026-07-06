# Modelo de Ordenação

## Contexto Acadêmico
Este sistema faz parte do **Projeto Interdisciplinar** que promove a integração curricular entre as disciplinas de **Programação Orientada a Objetos (POO)** (3º período) e **Computação Gráfica (CG)** (7º período) do Departamento de Ciência da Computação da **Universidade Federal de Rondônia (UNIR)**, sob a orientação do Prof. Dr. Lucas Marques da Cunha.

O objetivo do ecossistema é simular uma arquitetura multicamadas profissional através da separação estrita de responsabilidades: uma camada de aplicação/FSM estruturada e um motor gráfico nativo de baixo nível focado em pipeline de renderização avançado.

---

## Funcionalidades do Motor Gráfico (CG)
O motor foi desenvolvido em **C++ utilizando OpenGL (FreeGLUT)** e implementa:
* **Menu 3D Interativo:** Tela de inicialização com botões tridimensionais selecionáveis via clique do mouse (Mapeamento de Raycast 2D para Espaço 3D).
* **Controle Estrito de Animação (FSM):** Os algoritmos iniciam em estado **estático (congelado)** para fins pedagógicos e avançam passo a passo por frame quando comandados.
* **Pipeline de Iluminação Fixo:** Configuração de luz ambiente e difusa de estúdio para sombreamento realista dos blocos tridimensionais.
* **Texturização Procedimental:** Geração matemática de mapa de bits (padrão xadrez/stripes) na memória de vídeo, mitigando dependências externas de arquivos `.bmp`.
* **Múltiplos Cenários:** Alternância em tempo real entre uma Mesa de Madeira Acadêmica e um Grid Abstrato Digital (Estilo *Tron*).
* **Câmera Interativa:** Controle manual de órbita e elevação do ponto de vista sobre o vetor tridimensional.

---

## Arquitetura de Arquivos (Modularização)
O projeto foi estruturado seguindo boas práticas de engenharia de software para garantir baixo acoplamento e alta coesão:

```text
/projetoFinal
├── algoritmos.h     # Contrato e assinaturas globais das Máquinas de Estado
├── bubble.cpp       # Implementação isolada do passo a passo do Bubble Sort
├── selection.cpp    # Implementação isolada do passo a passo do Selection Sort
└── motor.cpp        # Núcleo gráfico, gerenciamento de buffers, luz, texturas e GLUT