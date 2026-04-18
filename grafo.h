#ifndef GRAFO_H
#define GRAFO_H

/* 
    Cada vértice possui um índice (0 a n-1) e um rótulo (char).
    Formato de entrada:
    <num_vertices>
    <indice> <rotulo> : <lista_indices_destinos>;
    
    Exemplo:
    5
    0 A : 1, 3;
    1 B : 2;
    2 C : 0;
    3 D : 0, 1, 2, 4;
    4 E : 1, 3;
*/

typedef struct No {
    int indice;         // indice do vertice destino
    struct No *proximo; // ponteiro para o proximo vizinho na lista
}No;

typedef struct Grafo {
    int n;              // numero de vertices do grafo
    char *rotulos;      // array de rotulos, rotulos[i] = rotulo do vertice i
    No **lista_adj;     // cada indice i contem um ponteiro para lista de adjacencia do vertice i
}Grafo;

No* criar_no(int indice);
Grafo* criar_grafo(int num_vertices);
void adicionar_aresta(Grafo *grafo, int origem, int destino);
void imprimir_grafo(Grafo *grafo);
void liberar_grafo(Grafo *grafo);

#endif