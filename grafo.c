#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"


No* criar_no(char rotulo){
    No* novo_no = (No*)malloc(sizeof(No));
    novo_no->rotulo = rotulo;
    novo_no->proximo = NULL;
    return novo_no;
}

Grafo* criar_grafo(int num_vertices){
    Grafo *grafo = (Grafo*)malloc(sizeof(Grafo));
    if(grafo == NULL){
        printf("Erro ao alocar o Grafo na memória!\n");
        exit(1);
    }

    grafo->n = num_vertices;
    grafo->lista_adj = (No**)malloc(num_vertices * sizeof(No*));
    if(grafo->lista_adj == NULL){
        printf("Erro ao alocar a lista de adj na memória!\n");
    }

    for(int i = 0; i < num_vertices; i++){
        grafo->lista_adj[i] = NULL;
    }

    return grafo;

}

void adicionar_aresta(Grafo *grafo, char origem, char destino){
    int indice_origem = origem - 'A';                       // converte a letra para um indice de 0 a 25

    No* novo_no = criar_no(destino);

    novo_no->proximo = grafo->lista_adj[indice_origem];     // inserindo o novo nó no inicio da lista
    grafo->lista_adj[indice_origem] = novo_no;              // atualizando o inicio da lista na posicao "indice_origem"
}

void liberar_grafo(Grafo *grafo){
    for(int i = 0 ; i < grafo->n; i++){
        No *atual = grafo->lista_adj[i];
        while(atual != NULL){
            No *temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }

    free(grafo->lista_adj);
    free(grafo);
}

void imprimir_grafo(Grafo* grafo) {
    printf("Grafo orientado\n");
    for (int i = 0; i < grafo->n; i++) {
        No* atual = grafo->lista_adj[i];
        
        // Converte o índice de volta para letra
        char rotulo_origem = i + 'A';

        printf("%c : ", rotulo_origem);

        if (atual == NULL) {
            printf("-1\n");
        } else {
            while (atual != NULL) {
                printf("%c", atual->rotulo);
                if (atual->proximo != NULL) {
                    printf(", ");
                }
                atual = atual->proximo;
            }
            printf("\n");
        }
    }
}
