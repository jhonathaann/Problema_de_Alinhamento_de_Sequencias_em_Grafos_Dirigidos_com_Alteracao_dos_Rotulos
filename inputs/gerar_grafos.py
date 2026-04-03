import string
import random

''' Cada vértice do grafo possui um rótulo único, no qual esse rótulo pertence a um 
    Alfabeto Σ previamente definido'''

def criar_grafo(alfabeto, grafo):
    for origem in alfabeto:
        destinos_aleatorios = random.sample(alfabeto, random.randint(0, 7))
        for destino in destinos_aleatorios:
            if origem != destino and destino not in grafo[origem]:
                grafo[origem].append(destino)

def salvar_grafo_txt(grafo, nome_arquivo):
    with open(nome_arquivo, "w", encoding="utf-8") as arquivo:
        arquivo.write("Grafo orientado (Lista de Adjacência)\n")

        for origem, destino in grafo.items():
            if destino:
                linha = f"{origem} : {', '.join(destino)};\n"
            else:
                linha = f"{origem} : -1;\n"
            arquivo.write(linha)

def main():
    alfabeto = list(string.ascii_uppercase)
    # dicionario: {A: [], B: [], ...,}
    grafo = {vertice: [] for vertice in alfabeto}

    criar_grafo(alfabeto, grafo)

    salvar_grafo_txt(grafo, "inputs\exemplo.txt")

if __name__ == "__main__":
    main()