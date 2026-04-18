import string
import random

''' Cada vértice do grafo possui um índice único e um rótulo.
    O rótulo pode se repetir entre vértices diferentes.
    Formato: <num_vertices>
             <indice> <rotulo> : <lista_indices_destinos>;
'''

def criar_grafo(num_vertices, grafo):
    for origem in range(num_vertices):
        # gera conexoes aletatorias com base no numero de vertices do grafo
        num_destinos = random.randint(0, min(7, num_vertices - 1))
        destinos_aleatorios = random.sample([v for v in range(num_vertices) if v != origem], num_destinos)
        grafo[origem] = destinos_aleatorios

def gerar_rotulos(num_vertices, alfabeto):
    # gera rotulo para cada vertice (podem se repetir)
    rotulos = {}
    for i in range(num_vertices):
        rotulos[i] = random.choice(alfabeto)
    return rotulos

def salvar_grafo_txt(grafo, rotulos, nome_arquivo):
    with open(nome_arquivo, "w", encoding="utf-8") as arquivo:
        num_vertices = len(grafo)
        arquivo.write(f"{num_vertices}\n")

        for indice in range(num_vertices):
            rotulo = rotulos[indice]
            destinos = grafo[indice]
            
            if destinos:
                lista_destinos = ', '.join(map(str, destinos))
                linha = f"{indice} {rotulo} : {lista_destinos};\n"
            else:
                linha = f"{indice} {rotulo} : -1;\n"
            arquivo.write(linha)

def main():
    alfabeto = list(string.ascii_uppercase)
    num_vertices = 26
    
    grafo = {}
    criar_grafo(num_vertices, grafo)
    
    rotulos = gerar_rotulos(num_vertices, alfabeto)

    salvar_grafo_txt(grafo, rotulos, "inputs\exemplo.txt")

if __name__ == "__main__":
    main()