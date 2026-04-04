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

    while(fgets(linhas, sizeof(linhas), arquivo) != NULL){
        char *token = strtok(linhas, " :");
        if(token == NULL) continue;

        char origem = token[0];

        token = strtok(NULL, " ");

        while(token != NULL){
            // Remove espaços em branco do início
            while(*token == ' ' || *token == '\t') token++;
            
            if(strcmp(token, "-1;") == 0 || strcmp(token, "-1") == 0) break;
            
            // Verifica se o token tem pelo menos um caractere e é uma letra
            if(strlen(token) > 0 && isalpha(token[0])){
                adicionar_aresta(grafo, origem, token[0]);
            }

            token = strtok(NULL, " ,;\n");
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

    // imprimir_grafo(grafo);

    resolver_alinhamento(grafo, q, m);
   
    liberar_grafo(grafo);
    return 0;
}