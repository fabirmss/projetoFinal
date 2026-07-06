#include "algoritmos.h"
#include <vector>

void passoInsertionSort(std::vector<int>& vetor, std::vector<bool>& ordenado, int tamanho, int& i, int& j, int& chave, bool& pegar_chave, bool& concluido, bool& simulacao_ativa) {
    if (i < tamanho) {
        if (pegar_chave) {
            chave = vetor[i];
            j = i - 1;
            pegar_chave = false;
        }

        if (j >= 0 && vetor[j] > chave) {
            vetor[j + 1] = vetor[j];
            j--;
        } else {
            vetor[j + 1] = chave;
            ordenado[i] = true; 
            i++;
            pegar_chave = true;
        }
    } else {
        for(int k = 0; k < tamanho; k++) {
            ordenado[k] = true;
        }
        concluido = true;
        simulacao_ativa = false;
    }
}