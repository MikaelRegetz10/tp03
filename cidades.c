#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cidades.h"

Estrada *getEstrada(const char *nomeArquivo) {
    FILE *file = fopen(nomeArquivo, "r");
    if (!file) {
        perror("Erro ao abrir arquivo");
        return NULL;
    }

    Estrada *estrada = (Estrada *)malloc(sizeof(Estrada));
    if (!estrada) {
        perror("Erro de alocação de memória");
        fclose(file);
        return NULL;
    }

    fscanf(file, "%d", &estrada->T);
    fscanf(file, "%d", &estrada->N);

    estrada->C = (Cidade *)malloc(estrada->N * sizeof(Cidade));
    if (!estrada->C) {
        perror("Erro de alocação de memória");
        fclose(file);
        free(estrada);
        return NULL;
    }

    for (int i = 0; i < estrada->N; i++) {
        if (fscanf(file, "%d ", &estrada->C[i].Posicao) != 1) {
            fclose(file);
            free(estrada->C);
            free(estrada);
            return NULL;
        }
        fgets(estrada->C[i].Nome, 256, file); 
        estrada->C[i].Nome[strcspn(estrada->C[i].Nome, "\n")] = '\0'; 
    }

    fclose(file);
    return estrada;
}

double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada)
        return -1.0; 

    double menorVizinhanca = estrada->T;

    for (int i = 0; i < estrada->N - 1; i++) {
        double distancia = fabs(estrada->C[i].Posicao - estrada->C[i+1].Posicao);
        if (distancia < menorVizinhanca)
            menorVizinhanca = distancia;
    }

    free(estrada->C);
    free(estrada);

    return menorVizinhanca;
}

double calcularVizinhanca(Estrada *estrada, int indiceCidade) {
    double menorVizinhanca = estrada->T;

    for (int i = 0; i < estrada->N; i++) {
        if (i != indiceCidade) {
            double distancia = fabs(estrada->C[indiceCidade].Posicao - estrada->C[i].Posicao);
            if (distancia < menorVizinhanca)
                menorVizinhanca = distancia;
        }
    }

    return menorVizinhanca;
}

char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada)
        return NULL;

    double menorVizinhanca = estrada->T; 
    char *cidadeMenor = NULL;

    for (int i = 0; i < estrada->N; i++) {
        double vizinhanca = calcularVizinhanca(estrada, i);
        if (vizinhanca < menorVizinhanca) {
            menorVizinhanca = vizinhanca;
 
            cidadeMenor = malloc(strlen(estrada->C[i].Nome) + 1);
            if (!cidadeMenor) {
                perror("Erro de alocação de memória");
                free(estrada->C);
                free(estrada);
                return NULL;
            }
  
            strcpy(cidadeMenor, estrada->C[i].Nome);
        }
    }

    free(estrada->C);
    free(estrada);

    return cidadeMenor;
}


int main() {
    const char *nomeArquivo = "teste01.txt";

    double menorVizinhanca = calcularMenorVizinhanca(nomeArquivo);
    char *cidadeMenor = cidadeMenorVizinhanca(nomeArquivo);

    printf("Menor vizinhanca de estrada: %.2f\n", menorVizinhanca);
    printf("Cidade com menor vizinhanca: %s\n", cidadeMenor);

    return 0;
}