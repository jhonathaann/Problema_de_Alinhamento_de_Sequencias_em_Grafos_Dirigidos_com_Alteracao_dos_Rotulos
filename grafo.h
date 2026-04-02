#ifndef GRAFO_H
#define GRAFO_h

/* 
    [v1, v2, v3, v4, v5]

    v1 --> [v2, v4],
    v2 --> [v3],
    v3 --> [v1],
    v4 --> [v1, v2, v3, v5]
    v5 --> [v2, v4]

*/

typedef struct No {
    char rotulo;        // simbolo do alfabeto Σ
    struct No *proximo; // ponteiro para o proximo vizinho na lista
}No;

typedef struct Grafo {
    int n;              // numero de vertices do grafo
    No **lista_adj;     // cada indice aqui contem um ponteiro que aponta para o inicio de uma lista
}Grafo;

No* criar_no(char rotulo);
Grafo* criar_grafo(int num_vertices);
void adicionar_aresta(Grafo *grafo, char origem, char destino);
void liberar_grafo(Grafo *grafo);

#endif