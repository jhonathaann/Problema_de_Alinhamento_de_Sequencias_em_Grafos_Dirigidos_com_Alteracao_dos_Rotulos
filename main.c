#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "grafo.h"
#include "modelo_pli.h"

Grafo* ler_grafo(const char* nome_arquivo){
    Grafo *grafo;
    FILE *arquivo = fopen(nome_arquivo, "r");
    int n;
    char linhas[256];

    if(arquivo == NULL){
        printf("Nao foi possivel abrir o arquivo: %s\n", nome_arquivo);
        return NULL;
    }

    fscanf(arquivo, "%d\n", &n);

    grafo = criar_grafo(n);

    // Le cada linha do grafo
    while(fgets(linhas, sizeof(linhas), arquivo) != NULL){
        char *p = linhas;
        
        // Pula espacos iniciais
        while(isspace((unsigned char)*p)) p++;
        if(*p == '\0' || *p == '\n') continue;

        // Le o indice do vertice origem
        char *endptr;
        long origem = strtol(p, &endptr, 10);
        if(endptr == p) continue;
        p = endptr;

        // Le o rotulo do vertice
        while(isspace((unsigned char)*p)) p++;
        if(!isalpha((unsigned char)*p)) continue;
        char rotulo_origem = *p;
        p++;

        // Procura os dois pontos
        char *dois_pontos = strchr(p, ':');
        if(dois_pontos == NULL) continue;
        p = dois_pontos + 1;

        // Valida o indice e armazena o rotulo
        if(origem < 0 || origem >= n) continue;
        grafo->rotulos[origem] = rotulo_origem;

        // Pula espacos apos os dois pontos
        while(isspace((unsigned char)*p)) p++;
        
        // Verifica se nao tem vizinhos
        if(strncmp(p, "-1", 2) == 0) continue;

        // Le a lista de destinos
        while(*p != '\0'){
            while(isspace((unsigned char)*p) || *p == ',') p++;
            if(*p == ';' || *p == '\n' || *p == '\0') break;
            if(*p == '-') break;

            long destino = strtol(p, &endptr, 10);
            if(endptr == p) break;
            if(destino >= 0 && destino < n){
                adicionar_aresta(grafo, (int)origem, (int)destino);
            }
            p = endptr;
        }
    }
    
    fclose(arquivo);
    return grafo;
}

int main(int argc, char *argv[]){
    Grafo *grafo;
    const char *q= "AAB";
    int m = 3;
    
    if(argc < 2){
        printf("Uso: %s <arquivo_grafo>\n", argv[0]);
        printf("Exemplo: %s inputs/exemplo.txt\n", argv[0]);
        return 1;
    }
    
    grafo = ler_grafo(argv[1]);
    
    if(grafo == NULL){
        printf("Erro ao carregar o grafo.\n");
        return 1;
    }

    imprimir_grafo(grafo);

    // resolver_alinhamento(grafo, q, m);
   
    liberar_grafo(grafo);
    return 0;
}