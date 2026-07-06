#include "algoritmos.h"
#include <cstdlib>

void passoBubbleSort(std::vector<int>& vetor, std::vector<bool>& ordenado, int tamanho, int& i, int& j, int& comp_1, int& comp_2, bool& concluido) {
    if (j < tamanho - i - 1) {
        comp_1 = j;
        comp_2 = j + 1;
        if (vetor[j] > vetor[j + 1]) {
            std::swap(vetor[j], vetor[j + 1]);
        }
        j++;
    } else {
        ordenado[tamanho - i - 1] = true;
        j = 0;
        i++;
        if (i >= tamanho - 1) {
            ordenado[0] = true;
            concluido = true;
        }
    }
}