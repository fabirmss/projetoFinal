#include "algoritmos.h"
#include <cstdlib>

void passoSelectionSort(std::vector<int>& vetor, std::vector<bool>& ordenado, int tamanho, int& i, int& j, int& min_idx, int& comp_1, int& comp_2, bool& concluido) {
    if (i < tamanho - 1) {
        if (j == 0) {
            min_idx = i;
            j = i + 1;
        }

        if (j < tamanho) {
            comp_1 = j;
            comp_2 = min_idx;
            if (vetor[j] < vetor[min_idx]) {
                min_idx = j;
            }
            j++;
        } else {
            std::swap(vetor[i], vetor[min_idx]);
            ordenado[i] = true;
            i++;
            j = 0;
        }
    } else {
        ordenado[tamanho - 1] = true;
        concluido = true;
    }
}