#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include <vector>

// Assinaturas exatas combinando com o escopo do motor
void passoBubbleSort(std::vector<int>& vetor, std::vector<bool>& ordenado, int tamanho, int& i, int& j, int& comp_1, int& comp_2, bool& concluido);
void passoSelectionSort(std::vector<int>& vetor, std::vector<bool>& ordenado, int tamanho, int& i, int& j, int& min_idx, int& comp_1, int& comp_2, bool& concluido);

#endif