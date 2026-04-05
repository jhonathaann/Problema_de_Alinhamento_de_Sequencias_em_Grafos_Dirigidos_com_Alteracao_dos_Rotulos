#include <stdio.h>
#include <stdlib.h>
#include "gurobi_c.h"
#include "modelo_pli.h"

#define TAM_ALFABETO 26

int resolver_alinhamento(Grafo* grafo, const char *q, int m){
    GRBenv      *env = NULL;
    GRBmodel    *model = NULL;
    int         erro = 0;
    int         n = grafo->n;
    int k = 0;

    // Criando o ambiente
    erro = GRBloadenv(&env, "alinhamento.log");
    if (erro) goto TRATA_ERRO;

    // Criando um modelo vazio
    erro = GRBnewmodel(env, &model, "AlinhamentoGrafo", 0, NULL, NULL, NULL, NULL, NULL);
    if (erro) goto TRATA_ERRO;

    // --- Adicionando as Variáveis ao Modelo ---

    // S_v: indica se o vertice v teve ou nao o seu rotulo alterado
    int *idx_S = (int *)malloc(n * sizeof(int));
    for(int v = 0; v < n; v++){
        char nome_var[25];
        sprintf(nome_var, "S_%d", v);

        // GRBaddvar(modelo, num_elementos_coluna, indices_restricoes, coeficientes,
        //           coef_funcao_objetivo, limite_inferior, limite_superior, tipo_variavel, nome)
        // Como o objetivo é MINIMIZAR o somatório de S_v, o coef_funcao_objetivo é 1.0
        erro = GRBaddvar(model, 0, NULL, NULL, 1.0, 0.0, 1.0, GRB_CONTINUOUS, nome_var);
        if (erro) goto TRATA_ERRO;

        // O Gurobi sempre adiciona variáveis sequencialmente.
        // Se o modelo estava vazio, o primeiro S_0 recebe o índice 0, S_1 o índice 1, etc.
        // Mas para manter a robustez, pegamos o número atual de colunas.
        int num_cols;
        GRBgetintattr(model, GRB_INT_ATTR_NUMVARS, &num_cols);
        idx_S[v] = num_cols-1;

    }

    // Y_[v][i]: indica se o vertice v foi visitado no passo "i" da string "q"
    // Y_[v][i] sera comprimida para y[v*m + i]
    int *idx_Y = (int *)malloc(n*m * sizeof(int));
    for(int v = 0; v < n; v++){
        for(int i = 0; i < m; i++){
            char nome_var[25];
            sprintf(nome_var, "V_%d_%d", v, i);

            // coef_funcao_objetivo é 0 pois essa var não entra na FO
            erro = GRBaddvar(model, 0, NULL, NULL, 0.0, 0.0, 1.0, GRB_BINARY, nome_var);
            if (erro) goto TRATA_ERRO;

            int num_cols;
            GRBgetintattr(model, GRB_INT_ATTR_NUMVARS, &num_cols);
            idx_Y[v*m + i] = num_cols-1;
            
        }
    }

    // L_[v][c]: indica se o vertice "v" recebe o caracter "c"
    int *idx_L = (int *)malloc(n * TAM_ALFABETO * sizeof(int));
    for(int v = 0; v < n; v++){
        for(int c = 0; c < TAM_ALFABETO; c++){
            char nome_var[25];
            sprintf(nome_var, "L_%d_%d", v, c + 'A');

            erro = GRBaddvar(model, 0, NULL, NULL, 0.0, 0.0, 1.0, GRB_BINARY, nome_var);
            if (erro) goto TRATA_ERRO;

            int num_cols;
            GRBgetintattr(model, GRB_INT_ATTR_NUMVARS, &num_cols);
            idx_L[v * TAM_ALFABETO + c] = num_cols - 1;

        }
    }

    // X_[u][v][i]: indica se a aresta que liga "u" com "v" foi ativada no i-ésimo passo da consulta
    int total_x_vars = n*n *(m-1);
    int *idx_X = (int *)malloc(total_x_vars * sizeof(int));

    // inicializa tudo com -1 (indicando que a var nao existe no Gurobi)
    for(int i = 0; i < total_x_vars; i++){
        idx_X[i] = -1;
    }

    // itera apenas sobre as arestas que de fato existem no grafo
    for(int u = 0; u < n; u++){
        No *atual = grafo->lista_adj[u];  // pegando o ponteiro que aponta para todos os vertices vizinhos do vertice "u"
        while(atual != NULL){
            int v = atual->rotulo - 'A'; // converte a letra de destino para o indice

            for(int i = 0; i < m-1; i++){
                char nome_var[25];
                sprintf(nome_var, "X_%d_%d_%d", u, v, i);

                erro = GRBaddvar(model, 0, NULL, NULL, 0.0, 0.0, 1.0, GRB_BINARY, nome_var);
                if (erro) goto TRATA_ERRO;

                int num_cols;
                GRBgetintattr(model, GRB_INT_ATTR_NUMVARS, &num_cols);
                idx_X[u * n * (m-1) + v * (m-1) + i] = num_cols -1;     // salva o indice da var no Gurobi na posicao correta no nosso vetor 1D
                k++;
            }
            atual = atual->proximo;
        }
    }


    erro = GRBupdatemodel(model);
    if (erro) goto TRATA_ERRO;

    printf("Modelo criado com sucesso!\n");
    printf("Variaveis S: %d\n", n);
    printf("Variaveis Y: %d\n", n*m);
    printf("Variaveis L: %d\n", n*TAM_ALFABETO);
    printf("Variaveis X: %d\n", k);

    free(idx_S);
    free(idx_Y);
    free(idx_L);
    free(idx_X);
    GRBfreemodel(model);
    GRBfreeenv(env);
    return 0;

TRATA_ERRO:
    printf("Erro no Gurobi: %s\n", GRBgeterrormsg(env));
    if (model) GRBfreemodel(model);
    if (env) GRBfreeenv(env);
    return 1;
}
