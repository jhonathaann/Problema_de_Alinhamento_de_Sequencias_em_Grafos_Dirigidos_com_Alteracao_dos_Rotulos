#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"


No* criar_no(int indice){
    No* novo_no = (No*)malloc(sizeof(No));
    novo_no->indice = indice;
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
    
    // Aloca array de rotulos
    grafo->rotulos = (char*)malloc(num_vertices * sizeof(char));
    if(grafo->rotulos == NULL){
        printf("Erro ao alocar o array de rotulos na memória!\n");
        exit(1);
    }
    
    // Aloca lista de adjacencia
    grafo->lista_adj = (No**)malloc(num_vertices * sizeof(No*));
    if(grafo->lista_adj == NULL){
        printf("Erro ao alocar a lista de adj na memória!\n");
        exit(1);
    }

    for(int i = 0; i < num_vertices; i++){
        grafo->lista_adj[i] = NULL;
        grafo->rotulos[i] = '\0';  // Inicializa rotulos
    }

    return grafo;
}

void adicionar_aresta(Grafo *grafo, int origem, int destino){
    No* novo_no = criar_no(destino);
    novo_no->proximo = grafo->lista_adj[origem];     // inserindo o novo nó no inicio da lista
    grafo->lista_adj[origem] = novo_no;              // atualizando o inicio da lista
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

    free(grafo->rotulos);
    free(grafo->lista_adj);
    free(grafo);
}

void imprimir_grafo(Grafo* grafo) {
    printf("Grafo orientado\n");
    for (int i = 0; i < grafo->n; i++) {
        No* atual = grafo->lista_adj[i];
        
        printf("%d %c : ", i, grafo->rotulos[i]);

        if (atual == NULL) {
            printf("-1\n");
        } else {
            while (atual != NULL) {
                printf("%d", atual->indice);
                if (atual->proximo != NULL) {
                    printf(", ");
                }
                atual = atual->proximo;
            }
            printf("\n");
        }
    }
}
