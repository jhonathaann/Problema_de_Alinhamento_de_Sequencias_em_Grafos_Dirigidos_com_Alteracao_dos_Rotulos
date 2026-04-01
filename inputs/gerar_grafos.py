import string
import random

def criar_grafo(vertices, grafo, num_conexoes=3):
    for origem in vertices:
        destinos_aleatorios = random.sample(vertices, num_conexoes)
        for destino in destinos_aleatorios:
            if origem != destino and destino not in grafo[origem]:
                grafo[origem].append(destino)

def salvar_grafo_txt(grafo, nome_arquivo):
    with open(nome_arquivo, "w", encoding="utf-8") as arquivo:
        arquivo.write("Grafo orientado (Lista de Adjacência)\n")
        arquivo.write("Formato: Vertice de Origem --> Vertice de Destino\n")

        for origem, destino in grafo.items():
            if destino:
                linha = f"{origem} -> {', '.join(destino)}\n"
            else:
                linha = f"{origem} -> [vazio]\n"
            arquivo.write(linha)

def main():
    vertices = list(string.ascii_uppercase)
    # dicionario: {A: [], B: [], ...,}
    grafo = {vertice: [] for vertice in vertices}

    criar_grafo(vertices, grafo, num_conexoes=3)

    salvar_grafo_txt(grafo, "grafo.txt")

if __name__ == "__main__":
    main()