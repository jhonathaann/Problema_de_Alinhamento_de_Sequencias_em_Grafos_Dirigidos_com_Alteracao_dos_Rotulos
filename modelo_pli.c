#include <stdio.h>
#include <stdlib.h>
#include "gurobi_c.h"
#include "modelo_pli.h"

#define TAM_ALFABETO 26

int resolver_alinhamento(Grafo* grafo, const char *q, int m){
    GRBenv      *env = NULL;
    GRBmodel    *model = NULL;
    int         erro = 0, contador_vars = 0, k = 0;
    int         n = grafo->n;

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

        idx_S[v] = contador_vars++;

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

            idx_Y[v*m + i] = contador_vars++;
            
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

            idx_L[v * TAM_ALFABETO + c] = contador_vars++;

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
            int v = atual->indice; // pega o indice do vertice destino
            for(int i = 0; i < m-1; i++){
                char nome_var[25];
                sprintf(nome_var, "X_%d_%d_%d", u, v, i);

                erro = GRBaddvar(model, 0, NULL, NULL, 0.0, 0.0, 1.0, GRB_BINARY, nome_var);
                if (erro) goto TRATA_ERRO;

                idx_X[u * n * (m-1) + v * (m-1) + i] = contador_vars++;     // salva o indice da var no Gurobi na posicao correta no nosso vetor 1D
                k++;
            }
            atual = atual->proximo;
        }
    }

    // --- Adicionando as Restrições ao Modelo ---

    // (2) Unicidade de visita no passo i: garantir que estamos em exatamente um vértice durante a consulta da nossa string de entrada
    // Somatório de Y_{v_i} = 1 para todo vertice v, para cada i
    for(int i = 0; i < m; i++){
        // alocando espaço para o pior caso: todos os n vertices participam da soma
        int *ind = (int*)malloc(n * sizeof(int));
        double *val = (double*)malloc(n * sizeof(double));
        int nao_nulos = 0;      // contator de elementos nao nulos na equacao

        for(int v = 0; v < n; v++){
            ind[nao_nulos] = idx_Y[v*m + i];    // pegando o indice da var Y_{v,i}
            val[nao_nulos] = 1.0;               // coeficiente dela é 1.0;
            nao_nulos++;
        }

        char nome_restricao[50];
        sprintf(nome_restricao, "Unicidade_Passo_%d", i);

        // GRBaddconstr(modelo, num_elementos_equacao, indices, coeficientes, sentido, valor_direito (RHS), nome)
        erro = GRBaddconstr(model, nao_nulos, ind, val, GRB_EQUAL, 1.0, nome_restricao);
        if (erro) goto TRATA_ERRO;

        free(ind);
        free(val);
    }

    // (5) Unicidade do rótulo final: garantir que um vertice tenha apenas um único rótulo
    // somatorio de L_{v,c} = 1 para todo caracter c, para cada vertice v
    for(int v = 0; v < n; v++){
        // alocando espaço para o pior caso: todos os n vertices participam da soma
        int *ind = (int*)malloc(TAM_ALFABETO * sizeof(int));
        double *val = (double*)malloc(TAM_ALFABETO * sizeof(double));
        int nao_nulos = 0;                                  // contator de elementos nao nulos na equacao

        for(int c = 0; c < TAM_ALFABETO; c++){
            ind[nao_nulos] = idx_L[v * TAM_ALFABETO + c];   // pegando o indice da var L_{v,c}
            val[nao_nulos] = 1.0;                           // coeficiente dela é 1.0;
            nao_nulos++;
        }

        char nome_restricao[50];
        sprintf(nome_restricao, "Unicidade_Rotulo_%d", v);

        erro = GRBaddconstr(model, nao_nulos, ind, val, GRB_EQUAL, 1.0, nome_restricao);
        if (erro) goto TRATA_ERRO;

        free(ind);
        free(val);
    }

    // (3) Equação de fluxo de saida: ela vai tratar da transição de um passo i para um passo i+1.
    // ela vai garantir que se voce visitar o vértice u no passo i, voce vai ser obrigado
    // a escolher exatamente uma aresta saindo de u para algum vizinho v
    for(int u = 0; u < n; u++){
        // i vai ate m-2, pois não tem fluxo de saida em m-1 (ultimo passo)
        for(int i = 0; i < m-1; i++){
            // pior caso de memoria: 1 (variavel y) + n-1 (no maximo n-1 vertices vizinhos de u)
            int *ind = (int*)malloc(1+(n-1) * sizeof(int));
            double *val = (double*)malloc(1+(n-1) * sizeof(double));
            int nao_nulos = 0;

            // 1° elemento da equacao: a var y_{u_i} (coeficiente positivo 1.0)
            ind[nao_nulos] = idx_Y[u*m + i];
            val[nao_nulos] = 1.0;
            nao_nulos++;
            
            // 2° elemento da equacao: o somatorio negativo de x_{u,v,i}
            No *atual = grafo->lista_adj[u];    // ponteiro para o inicio da lista de vizinhos do vertice u
            while(atual != NULL){
                int v = atual->indice;          // pega o indice do vertice atual

                // pega o indice da aresta que foi ativada
                int id_x = idx_X[u * n * (m-1) + v * (m-1) + i];

                // verificando se de fato ela foi ativada
                if(id_x != -1){
                    ind[nao_nulos] = id_x;
                    val[nao_nulos] = -1.0;      // coeficiente negativo pois passamos para o lado esquerdo
                    nao_nulos++;
                }

                atual = atual->proximo;
            }

            char nome_restricao[25];
            sprintf(nome_restricao, "Fluxo_Saida_u%d_i%d", u, i);

            // GRBaddconstr: y_{u,i} - sum x_{u,v,i} == 0
            erro = GRBaddconstr(model, nao_nulos, ind, val, GRB_EQUAL, 0.0, nome_restricao);
            if (erro) goto TRATA_ERRO;

            free(ind);
            free(val);
        }
    }

    // (4) Equação de fluxo de entrada: ela vai garantir que se voce visitou o vertice "v"
    // no passo i+1, obrigatoriamente alguma aresta que chega no vertice "v" partindo de algum
    // vizinho "u" deve ter sido ativada no passo i
    for(int v = 0; v < n; v++){
        for(int i = 0; i < m-1; i++){
            // pior caso de memoria: 1 variavel (y_{v,i}) + n-1 (no maximo n-1 arestas chegando em v)
            int *ind = (int*)malloc(1+(n-1) * sizeof(int));
            double *val = (double*)malloc(1+(n-1) * sizeof(double));
            int nao_nulos = 0;

            // 1° elemento da equação: y_{v_i} (coeficiente 1.0)
            // a transicao "i" chega no destino no passo "i+1"
            ind[nao_nulos] = idx_Y[v * m + (i+1)];
            val[nao_nulos] = 1.0;
            nao_nulos++;

            // 2° elemento da equacao: o somatorio negativo de x_{u,v,i}
            // precisamos varrer o grafo para saber que aponta para o vertice "v"
            for(int u = 0; u < n; u++){
                No *atual = grafo->lista_adj[u];
                while(atual != NULL){
                    // verificando se o vizinho do vertice "u" é o nosso vertice de interesse "v"
                    if(atual->indice == v){
                        // pega o indice da aretas u -> v
                        int id_x = idx_X[u * n * (m - 1) + v * (m - 1) + i];
                        
                        // verificando se a aresta foi de fato ativada
                        if(id_x != -1){
                            ind[nao_nulos] = id_x;
                            val[nao_nulos] = -1.0;
                            nao_nulos;
                        }
                        break;      // o grafo tratado é orientado e simples: ou seja, não temos casos onde u <--> v
                    }

                    atual = atual->proximo;
                }
            }
            char nome_variavel[25];
            sprintf(nome_variavel, "Fluxo_Entrada_v%d_i%d", v, i);

            // GRBaddconstr: y_{v, i+1} - sum x_{u,v,i} == 0
            erro = GRBaddconstr(model, nao_nulos, ind, val, GRB_EQUAL, 0.0, nome_variavel);
            if (erro) goto TRATA_ERRO;

            free(ind);
            free(val);
        }
    }

    // (6) garante que se o vertice v foi visitado no passo i , então o seu rótulo final 
    // tem que ser o mesmo simbolo contido na string de entrada q na posicao i (q[i])
    // y_{v,i} - L_{v, q[i]} <= 0
    for(int v = 0; v < n; v++){
        for(int i = 0; i < m; i++){
            int ind[2];
            double val[2];

            // variavel y_{v,i} (coenficiente é 1.0)
            ind[0] = idx_Y[v * m + i];
            val[0] = 1.0;

            // variavel L_{v, q[i]} (coeficiente é -1.0)
            int posicao = q[i] - 'A';
            ind[1] = idx_L[v * TAM_ALFABETO + posicao];
            val[1] = -1.0;

            char nome_restricao[25];
            sprintf(nome_restricao, "Emissao_v%d_i%d", v, i);

            erro = GRBaddconstr(model, 2, ind, val, GRB_LESS_EQUAL, 0.0, nome_restricao);
            if (erro) goto TRATA_ERRO;
        }
    }

    // (7) Penalidade de substituição do rótulo original do vertice v
    // Equação: S_v + L_{v, sigma(v)} >= 1
    for(int v = 0; v < n; v++){
        int ind[2];
        double val[2];

        // 1° variavel: S_{v} (coeficiente é 1.0)
        ind[0] = idx_S[v];
        val[0] = 1.0;

        // 2° variavel: L{v,sigma(v)} (coeficiente é 1.0)
        int posicao = grafo->rotulos[v] - 'A';
        ind[1] = idx_L[v * TAM_ALFABETO + posicao];
        val[1] = 1.0;

        char nome_restricao[25];
        sprintf(nome_restricao, "Penalidade_v%d", v);

        erro = GRBaddconstr(model, 2, ind, val, GRB_GREATER_EQUAL, 1.0, nome_restricao);
        if (erro) goto TRATA_ERRO;
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
