

1. **Primeira Linha (Quantidade de Vértices):** A primeira linha contém um único número inteiro representando a quantidade de vértices que o grafo possui.

2. **Linhas Subsequentes (Arestas):**
   Cada linha a partir da segunda representa um vértice de origem, no qual o formato é:
   `
   * 
<num_vertices>
    <indice> <rotulo> : <lista_indices_destinos>;`


---

## Exemplo

Abaixo está um exemplo de como o arquivo `.txt` deve se parecer:

```txt
26
0 Q : -1;
1 C : 18;
2 I : 1, 9, 10, 5;
3 Q : 4, 23, 24, 12, 18;
4 C : -1;
5 G : 15, 22, 25;
6 I : 12, 10, 13, 15;
7 A : -1;
8 Y : 17, 18;
9 K : 12, 20, 13, 0, 19;
10 H : 2;
11 T : 16, 8, 19, 0, 22;
12 E : 1, 18;
13 K : 23, 0, 10, 25, 19, 17;
14 S : 9, 17, 2;
15 I : 5;
16 F : 24, 20, 8, 10, 17, 22, 12;
17 I : 14, 18, 9, 7;
18 N : 4, 19, 5, 13, 25, 10;
19 H : 12, 14, 0;
20 B : 2;
21 O : 25, 18, 9, 3, 8, 4;
22 C : 8;
23 L : 14;
24 F : 6, 4, 13, 23, 9, 17, 3;
25 E : 2, 24, 8, 16, 10, 22;
