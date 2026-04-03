

1. **Primeira Linha (Quantidade de Vértices):** A primeira linha contém um único número inteiro representando a quantidade de vértices que o grafo possui.

2. **Linhas Subsequentes (Arestas):**
   Cada linha a partir da segunda representa um vértice de origem e a lista de vértices de destino alcançáveis a partir dele.
   * O formato padrão é: `Origem : Destino1, Destino2, ..., DestinoN;`

   * **`-1` (Menos um):** Indica que o vértice de origem **não possui** arestas saindo dele (lista vazia).

---

## Exemplo

Abaixo está um exemplo de como o arquivo `.txt` deve se parecer:

```txt
26
A : F, P, V, R, K, N;
B : M, J, F, E, P, C, X;
C : -1;
D : Z, I, G, J;
E : L;
F : I, D, C, S, X, Q, G;
G : X, M, D;
H : F;
I : D, O, U, C, A, P, T;
J : Q;
K : V, L, U, I, X;
L : X, D, N;
M : C, U, Q, P, G;
N : K, L, S, B, Q, M;
O : C, I, L, P, M, Q, K;
P : K, E, S, U, W, V;
Q : -1;
R : D, C, O, B, H, E;
S : N, V, E, T, R, H, G;
T : K, E;
U : J, H, C, A, S, Y;
V : C, Y, J, G, K, M, T;
W : D, S, Y, J, Z;
X : S, P, V, G, R;
Y : X;
Z : K, B, S;