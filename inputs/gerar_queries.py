import string
import random
import os

def gerar_query(tamanho, alfabeto):
    """
    Gera uma string aleatória de tamanho 'm' usando os caracteres do alfabeto fornecido.
    """
    # random.choices retorna uma lista com 'tamanho' elementos escolhidos com reposição
    caracteres_escolhidos = random.choices(alfabeto, k=tamanho)
    
    # junsta os caracteres em uma unica string
    query = "".join(caracteres_escolhidos)
    return query


def salvar_query_txt(query, diretorio_saida="inputs/querys"):
    # Garante que a pasta de saída exista
    os.makedirs(diretorio_saida, exist_ok=True)
    
    tamanho = len(query)
    nome_arquivo = f"tamanho_{tamanho}.txt"
    caminho_completo = os.path.join(diretorio_saida, nome_arquivo)
    
    with open(caminho_completo, "w", encoding="utf-8") as arquivo:
        arquivo.write(f"{tamanho}\n")
        arquivo.write(f"{query}\n")
        
    print(f"Query de tamanho {tamanho} gerada e salva em: {caminho_completo}")

def main():
    alfabeto = list(string.ascii_uppercase)
    tamanho_da_query = 15
    
    query = gerar_query(tamanho_da_query, alfabeto)
    
    salvar_query_txt(query)

if __name__ == "__main__":
    main()