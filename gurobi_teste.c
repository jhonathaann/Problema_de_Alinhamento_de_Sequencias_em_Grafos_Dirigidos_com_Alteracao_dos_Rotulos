#include <stdlib.h>
#include <stdio.h>
#include "gurobi_c.h"

int main(void) {
    GRBenv   *env   = NULL;
    GRBmodel *model = NULL;
    int       error = 0;

    error = GRBloadenv(&env, "mip1.log");
    
    error = GRBnewmodel(env, &model, "exemplo", 0, NULL, NULL, NULL, NULL, NULL);

    error = GRBoptimize(model);

    GRBfreemodel(model);
    GRBfreeenv(env);

    return 0;
}