#include <stdio.h>
#include <stdlib.h>
#include "gurobi_c.h"
#include "modelo_pli.h"

int resolver_alinhamento(Grafo* grafo, const char *q, int m){
    GRBenv      *env = NULL;
    GRBmodel    *model = NULL;
    int         erro = 0;
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
        char nome_var[50];
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
            char nome_var[50];
            sprintf(nome_var, "V_%d_%d", v, i);

            // coef_funcao_objetivo é 0 pois essa var não entra na FO
            erro = GRBaddvar(model, 0, NULL, NULL, 0.0, 0.0, 1.0, GRB_BINARY, nome_var);
            if (erro) goto TRATA_ERRO;

            int num_cols;
            GRBgetintattr(model, GRB_INT_ATTR_NUMVARS, &num_cols);
            idx_Y[v*m + i] = num_cols-1;
            
        }
    }

    erro = GRBupdatemodel(model);
    if (erro) goto TRATA_ERRO;

    printf("Modelo criado com sucesso!\n");
    printf("Variaveis S: %d\n", n);
    printf("Variaveis Y: %d\n", n*m);

    free(idx_S);
    free(idx_Y);
    GRBfreemodel(model);
    GRBfreeenv(env);
    return 0;

TRATA_ERRO:
    printf("Erro no Gurobi: %s\n", GRBgeterrormsg(env));
    if (model) GRBfreemodel(model);
    if (env) GRBfreeenv(env);
    return 1;
}
