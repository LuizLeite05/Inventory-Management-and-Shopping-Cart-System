#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Catálogo de produtos
typedef struct produtos
{
	int codigo;
	char descricao[40];
	float preco;
	struct produtos *prox;
} tProdutos;

// Item de estoque de uma filial
typedef struct itemEstoque
{
	int codigo_produto;
	int quantidade;
	struct itemEstoque *prox;
} tEstoque;

// Filial
typedef struct filial
{
	int id_filial;
	char nome[50];
	char cidade[50];
	tEstoque *estoque;
	struct filial *prox;
} tFilial;

// Item no carrinho
typedef struct itemCarrinho
{
	int codigo_produto;
	int quantidade;
	float preco_unitario;
	struct itemCarrinho *prox;
} tItemCarrinho;

// Carrinho
typedef struct carrinho
{
	tItemCarrinho *itens;
	float total;
} tCarrinho;

/*==== Escopo das Funções ====*/

tProdutos *lerProdutos(char *documentoTXT);
tProdutos *pesquisarProdutos(tProdutos *lista);
tProdutos *buscarProduto(tProdutos *catalogo, int codigo);
tFilial *lerFiliais(char *documentoTXT);
tFilial *buscarFilial(tFilial *filiais, int id);
void lerEstoques(tFilial *listaFiliais, char *documentoTXT);
void iniciarCarrinho(tCarrinho *carrinho);
void inserirCarrinho(tCarrinho *carrinho, tProdutos *catalogo, int cod, int qtd);
void removerDoCarrinho(tCarrinho *carrinho, int cod);
void removerQuantiaCarrinho(tCarrinho *carrinho, int cod, int quantia);
void listarCarrinho(tCarrinho *carrinho);
void listarFaltas(tCarrinho *carrinho, tFilial *filial, tProdutos *catalogo);
void verificaDisponibilidade(tCarrinho *carrinho, tFilial *filiais, tProdutos *catalogo);
void finalizarCompra(tCarrinho *carrinho, tFilial *filiais, tProdutos *catalogo, int id_filial);
void comandos(char *documentoTXT, tProdutos *catalogo, tFilial *filiais);
void liberarProdutos(tProdutos *lista);
void liberarEstoques(tEstoque *estoque);
void liberarFiliais(tFilial *lista);
void liberarCarrinho(tCarrinho *carrinho);
int ajustarCarrinhoParaFilial(tCarrinho *carrinho, tFilial *filial);
int filialAtendeTotal(tCarrinho *carrinho, tFilial *filial);

// Arquivo Main

int main()
{
	int opcao;
	tProdutos *catalogo = NULL;
	tFilial *filiais = NULL;
	tCarrinho carrinho;
	iniciarCarrinho(&carrinho);

	do
	{
		printf("\n======================= MENU MINIMO =============\n");
		printf("\n1 - Carregar Dados\n2 - Pesquisar Produto\n3 - Carrinho\n4 - Verificar Disponibilidade por Filial\n5 - Escolher Filial e Finalizar\n6 - Carregar Carrinho\n0 - Sair\n> ");

		if (scanf("%d", &opcao) != 1)
		{
			while (getchar() != '\n');

			printf("\nEntrada inválida! Encerrando programa...\n");

			liberarProdutos(catalogo);
			liberarFiliais(filiais);
			liberarCarrinho(&carrinho);
			printf("Memória liberada com sucesso!\n");

			return 0;
		}
		while (getchar() != '\n');

		switch (opcao)
		{
		case 1:
			catalogo = lerProdutos("produtos_v2.txt");
			filiais = lerFiliais("filiais_v2.txt");
			lerEstoques(filiais, "estoques_v2.txt");
			break;

		case 2:
			if (catalogo == NULL || filiais == NULL)
			{
				printf("\nCarregue os dados primeiro!!\n");
			}
			else
			{
				pesquisarProdutos(catalogo);
			}
			break;

		case 3:
			if (catalogo == NULL || filiais == NULL)
			{
				printf("\nCarregue os dados primeiro!\n");
				break;
			}

			int opCar;
			int codigo;
			int qtd;

			do
			{
				printf("\n===== MENU DO CARRINHO =====\n");
				printf("1 - Inserir item\n");
				printf("2 - Remover item\n");
				printf("3 - Listar carrinho\n");
				printf("4 - Remover p/Quantidade\n");
				printf("0 - Voltar\n> ");
				scanf("%d", &opCar);

				switch (opCar)
				{
				case 1:
					printf("Codigo do produto: ");
					scanf("%d", &codigo);
					printf("Quantidade: ");
					scanf("%d", &qtd);
					inserirCarrinho(&carrinho, catalogo, codigo, qtd);
					break;

				case 2:
					printf("Codigo do produto para remover: ");
					scanf("%d", &codigo);
					removerDoCarrinho(&carrinho, codigo);
					break;

				case 3:
					listarCarrinho(&carrinho);
					break;

				case 4:
					printf("Codigo do produto: ");
					scanf("%d", &codigo);
					printf("Quantidade: ");
					scanf("%d", &qtd);
					removerQuantiaCarrinho(&carrinho, codigo, qtd);
					break;
				}

			} while (opCar != 0);

			break;

		case 4:
			if (catalogo == NULL || filiais == NULL)
			{
				printf("\nCarregue os dados primeiro!!\n");
			}
			else
			{
				verificaDisponibilidade(&carrinho, filiais, catalogo);
			}
			break;

		case 5:
			if (catalogo == NULL || filiais == NULL)
			{
				printf("\nCarregue os dados primeiro!!\n");
			}
			else
			{
				if (carrinho.itens == NULL)
				{
					printf("\nCarrinho vazio! Adicione itens primeiro.\n");
				}
				else
				{
					int id_fil;
					printf("\nDigite o ID da filial para finalizar: ");
					scanf("%d", &id_fil);
					finalizarCompra(&carrinho, filiais, catalogo, id_fil);
				}
			}
			break;

		case 6:
			if (catalogo == NULL || filiais == NULL)
			{
				printf("\nCarregue os dados primeiro!!\n");
			}
			else
			{
				comandos("carrinhos_clientes_v2.txt", catalogo, filiais);
			}
			break;

		case 0:
			printf("\nLiberando memória...\n");
			liberarProdutos(catalogo);
			liberarFiliais(filiais);
			liberarCarrinho(&carrinho);
			printf("Memória liberada com sucesso!\n");

			printf("\nSaindo...\n");
			break;

		default:
			printf("\nOpcao Invalida - Escolha uma opcao valida\n");
			break;
		}

	} while (opcao != 0);
	return 0;
}
/*
As seguintes funções abrem o arquivo que será passado para ela, após abrir o arquivo, lê e
adiciona os dados vindos do arquivo .txt a sua respectiva lista encadeada;

Caso aconteça de o documento estar vazio, tem uma verificação informando.
O mesmo se aplica caso tenha algum erro ao abrir o arquivo .txt
*/
tProdutos *lerProdutos(char *documentoTXT)
{
	FILE *arquivo = fopen(documentoTXT, "r");
	if (arquivo == NULL)
	{
		printf("\nErro Arquivo vazio %s", documentoTXT);
		return NULL;
	}
	tProdutos *lista = NULL;
	tProdutos *novo = NULL;
	tProdutos *ultimo = NULL;
	int codigo;
	char descricao[40];
	float valor;

	while (fscanf(arquivo, "%d %s %f", &codigo, descricao, &valor) == 3)
	{
		novo = (tProdutos *)malloc(sizeof(tProdutos));
		if (novo == NULL)
		{
			printf("\nErro de Alocacao");
			fclose(arquivo);
			return lista;
		}

		novo->codigo = codigo;
		strcpy(novo->descricao, descricao);
		novo->preco = valor;
		novo->prox = NULL;

		// Caso a lsita esteja vazia, adiciona o primeiro elemento
		if (lista == NULL)
		{
			lista = novo;
			ultimo = novo;
		}
		else
		{
			ultimo->prox = novo;
			ultimo = novo;
		}
	}
	fclose(arquivo);
	printf("\nDados dos Produtos carregados com sucesso!");
	return lista;
}

tFilial *lerFiliais(char *documentoTXT)
{
	FILE *arquivo = fopen(documentoTXT, "r");
	if (arquivo == NULL)
	{
		printf("\nErro arquivo Vazio %s", documentoTXT);
		return NULL;
	}
	tFilial *novo = NULL;
	tFilial *lista = NULL;
	tFilial *ultimo = NULL;
	int id_filial;
	char nome[50];
	char cidade[50];
	while (fscanf(arquivo, "%d %s %s", &id_filial, nome, cidade) == 3)
	{
		novo = (tFilial *)malloc(sizeof(tFilial));
		if (novo == NULL)
		{
			printf("\nErro de Alocacao");
			fclose(arquivo);
			return lista;
		}

		novo->id_filial = id_filial;
		strcpy(novo->nome, nome);
		strcpy(novo->cidade, cidade);
		novo->estoque = NULL;
		novo->prox = NULL;

		if (lista == NULL)
		{
			lista = novo;
			ultimo = novo;
		}
		else
		{
			ultimo->prox = novo;
			ultimo = novo;
		}
	}

	fclose(arquivo);
	printf("\nDados das Filiais carregados com sucesso!");
	return lista;
}

void lerEstoques(tFilial *listaFiliais, char *documentoTXT)
{
	FILE *arquivo = fopen(documentoTXT, "r");
	if (arquivo == NULL)
	{
		printf("\nErro Arquivo vazio %s", documentoTXT);
		return;
	}
	tEstoque *novo = NULL;
	tFilial *aux = NULL;
	tEstoque *ultimo = NULL;
	int id_filial;
	int codigo_produto;
	int quantidade;

	while (fscanf(arquivo, "%d %d %d", &id_filial, &codigo_produto, &quantidade) == 3)
	{
		aux = listaFiliais;
		while (aux && aux->id_filial != id_filial)
			aux = aux->prox;

		if (aux == NULL)
		{
			printf("\nAVISO: Filial %d nao encontrada", id_filial);
			continue;
		}

		novo = (tEstoque *)malloc(sizeof(tEstoque));
		if (novo == NULL)
		{
			printf("\nErro de alocacao");
			fclose(arquivo);
			return;
		}

		novo->codigo_produto = codigo_produto;
		novo->quantidade = quantidade;
		novo->prox = NULL;

		if (aux->estoque == NULL)
		{
			aux->estoque = novo;
		}
		else
		{
			ultimo = aux->estoque;
			while (ultimo->prox)
			{
				ultimo = ultimo->prox;
			}
			ultimo->prox = novo;
		}
	}
	fclose(arquivo);
	printf("\nDados dos Estoques carregados com sucesso!");
}

/*
A função pesquisarProdutos, recebe como parametro uma lista do tipo Produtos
caso essa lista esteja vazia, informa ao usuário que nenhum produto foi carregado
Caso a lista não esteja vazia entrega duas opções de pesquisa ao usuario
pesquisar pelo código do produto, ou pesquisar pela descrição do produto
Utiliza um ponteiro aux para percorrer a lista até encontrar o codigo ou encontrar
a descrição.
Caso o produto não seja encontrado informa ao usuário, se for encontrado, informa para ele:
seu código, descrição e valor
*/
tProdutos *pesquisarProdutos(tProdutos *lista)
{
	if (lista == NULL)
	{
		printf("\nNenhum produto carregado!\n");
		return NULL;
	}

	int opcao;
	printf("\nPesquisar por: \n1 - Codigo\n2 - Descricao\n");
	scanf("%d", &opcao);

	if (opcao == 1)
	{
		int codigo;
		printf("\nInforme o codigo do produto: ");
		scanf("%d", &codigo);

		tProdutos *aux = lista;

		while (aux)
		{
			if (aux->codigo == codigo)
			{
				printf("\nProduto encontrado\n");
				printf("\nCodigo: %d\nDescricao: %s\nPreco: %.2f\n", aux->codigo, aux->descricao, aux->preco);
				return aux;
			}
			aux = aux->prox;
		}
		printf("\nProduto NAO encontrado\n");
		return NULL;
	}
	else if (opcao == 2)
	{
		char texto[40];

		printf("\nInforme a descricao do produto: ");
		scanf("%39s", texto);

		tProdutos *aux = lista;
		int achou = 0;

		printf("\n========== RESULTADOS =================\n");
		while (aux)
		{
			if (strstr(aux->descricao, texto))
			{
				printf("\nCodigo: %d | %s | %.2f\n", aux->codigo, aux->descricao, aux->preco);
				achou = 1;
			}
			aux = aux->prox;
		}
		if (!achou)
		{
			printf("\nProduto não encontrado.\n");
		}
		return NULL;
	}
	else
	{
		printf("\nOpcao Invalida\n");
		return NULL;
	}
}

void iniciarCarrinho(tCarrinho *carrinho)
{
	carrinho->itens = NULL;
	carrinho->total = 0;
}

/*
Realiza a inserção de itens no carrinho, utilizando como parametro o seu codigo
e após o programa encontrar o produto, soliciata ao usuario sua respectiva quantidade
*/
void inserirCarrinho(tCarrinho *carrinho, tProdutos *catalogo, int cod, int qtd)
{
	if (qtd <= 0)
	{
		printf("\nInsira uma quantidade valida!");
		return;
	}
	tProdutos *auxProdutos = catalogo;
	while (auxProdutos && auxProdutos->codigo != cod)
		auxProdutos = auxProdutos->prox;

	if (auxProdutos == NULL)
	{
		printf("\nProduto nao encontrado no catalogo!");
		return;
	}

	tItemCarrinho *aux = carrinho->itens;
	while (aux)
	{
		if (aux->codigo_produto == cod)
		{
			aux->quantidade += qtd;
			carrinho->total += (aux->preco_unitario) * qtd;
			printf("\nQuantidade atualizada no carrinho.\n");
			return;
		}
		aux = aux->prox;
	}

	tItemCarrinho *novo = (tItemCarrinho *)malloc(sizeof(tItemCarrinho));
	novo->codigo_produto = cod;
	novo->quantidade = qtd;
	novo->preco_unitario = auxProdutos->preco;
	novo->prox = carrinho->itens;

	carrinho->itens = novo;
	carrinho->total += (novo->preco_unitario) * qtd;

	printf("\nItem adicionado ao carrinho!");
}

void removerDoCarrinho(tCarrinho *carrinho, int cod)
{
	if (carrinho->itens == NULL)
	{
		printf("\nCarrinho vazio!");
		return;
	}

	tItemCarrinho *aux = carrinho->itens;
	tItemCarrinho *anterior = NULL;

	while (aux && aux->codigo_produto != cod)
	{
		anterior = aux;
		aux = aux->prox;
	}
	if (aux == NULL)
	{
		printf("\nItem nao encontrado no carrinho!");
		return;
	}

	carrinho->total -= aux->preco_unitario * aux->quantidade;

	if (anterior == NULL)
	{
		carrinho->itens = aux->prox;
	}
	else
	{
		anterior->prox = aux->prox;
	}
	free(aux); // CORRIGIDO: libera o item correto
	printf("\nItem removido com sucesso do Carrinho!");
}

/*
Verifica se o carrinho está vazio, remove uma quantidade especifica do item que o
código foi informado pelo usuário, e faz verificações, caso o item fique zerado no carrinho
*/
void removerQuantiaCarrinho(tCarrinho *carrinho, int cod, int quantia)
{
	if (carrinho->itens == NULL)
	{
		printf("\nCarrinho Vazio");
		return;
	}

	tItemCarrinho *aux = carrinho->itens;
	tItemCarrinho *anterior = NULL;

	while (aux && aux->codigo_produto != cod)
	{
		anterior = aux;
		aux = aux->prox;
	}
	if (aux == NULL)
	{
		printf("\nItem não encontrado no carrinho!");
		return;
	}

	if (quantia >= aux->quantidade)
	{
		carrinho->total -= aux->preco_unitario * aux->quantidade;

		if (anterior == NULL)
		{
			carrinho->itens = aux->prox;
		}
		else
		{
			anterior->prox = aux->prox;
		}
		free(aux);
		printf("\nItem removido completamente do carrinho");
	}
	else
	{
		aux->quantidade -= quantia;
		carrinho->total -= aux->preco_unitario * quantia;
		printf("\nQuantidade reduzida. Nova quantia do produto: %d", aux->quantidade);
	}
}

void listarCarrinho(tCarrinho *carrinho)
{
	if (carrinho == NULL || carrinho->itens == NULL)
	{
		printf("\nCarrinho vazio");
		return;
	}
	tItemCarrinho *aux = carrinho->itens;

	printf("\n===================== Carrinho Atual ============\n");
	while (aux)
	{
		float subtotal = aux->quantidade * aux->preco_unitario;
		printf("\nCodigo: %d | Qtd: %d | Valor Unitario: %.2f | Subtotal: %.2f",
					 aux->codigo_produto, aux->quantidade, aux->preco_unitario, subtotal);

		aux = aux->prox;
	}
	printf("\n\nTOTAL: R$ %.2f", carrinho->total);
	printf("\n============ ++++++++++++++++++++++++++ ================\n");
}

/*
Verifica quais filiais conseguem atender totalmente o pedido realiado pelo usuario
Caso a filial atenda 100% altera o "econtrei" para 1;
*/
int filialAtendeTotal(tCarrinho *carrinho, tFilial *filial)
{
	tItemCarrinho *aux = carrinho->itens;

	while (aux)
	{
		tEstoque *itemEstoque = filial->estoque;
		int encontrei = 0;
		while (itemEstoque)
		{
			if (itemEstoque->codigo_produto == aux->codigo_produto)
			{
				encontrei = 1;
				if (itemEstoque->quantidade < aux->quantidade)
				{
					return 0;
				}
				break;
			}
			itemEstoque = itemEstoque->prox;
		}

		if (encontrei == 0)
		{
			return 0;
		}
		aux = aux->prox;
	}
	return 1;
}

/*
Verifica qual produto está em falta na filial, e mostra ao usuario qual produto está em falta
sua descrição e a quantidade em falta
*/
void listarFaltas(tCarrinho *carrinho, tFilial *filial, tProdutos *catalogo)
{
	tItemCarrinho *aux = carrinho->itens;

	while (aux)
	{
		tEstoque *itemEst = filial->estoque;
		int disponivel = 0;

		while (itemEst)
		{
			if (itemEst->codigo_produto == aux->codigo_produto)
			{
				disponivel = itemEst->quantidade;
				break;
			}
			itemEst = itemEst->prox;
		}

		int falta = aux->quantidade - disponivel;

		if (falta > 0)
		{
			tProdutos *prod = catalogo;
			while (prod && prod->codigo != aux->codigo_produto)
			{
				prod = prod->prox;
			}

			if (prod)
			{
				printf("      Falta: %d (%s) - %d unidades\n",
							 aux->codigo_produto,
							 prod->descricao,
							 falta);
			}
		}

		aux = aux->prox;
	}
}

void verificaDisponibilidade(tCarrinho *carrinho, tFilial *filiais, tProdutos *catalogo)
{
	if (carrinho->itens == NULL)
	{
		printf("\nCarrinho vazio\n");
		return;
	}

	printf("\n========== VERIFICACAO DE ATENDIMENTO ==========\n");

	printf("\nFILIAIS QUE ATENDEM 100%%:\n");

	tFilial *filial = filiais;
	int temApta = 0;

	while (filial != NULL)
	{
		if (filialAtendeTotal(carrinho, filial) == 1)
		{
			printf("  - Filial %d: %s\n", filial->id_filial, filial->nome);
			temApta = 1;
		}
		filial = filial->prox;
	}

	if (temApta == 0)
	{
		printf("  (Nenhuma filial atende 100%%)\n");
	}

	printf("\nFILIAIS QUE NAO ATENDEM:\n");

	filial = filiais;
	int temNaoApta = 0;

	while (filial != NULL)
	{
		if (filialAtendeTotal(carrinho, filial) == 0)
		{
			printf("  - Filial %d: %s\n", filial->id_filial, filial->nome);
			listarFaltas(carrinho, filial, catalogo);
			temNaoApta = 1;
		}
		filial = filial->prox;
	}

	if (temNaoApta == 0)
	{
		printf("\n(Todas as filiais atendem 100)\n");
	}

	printf("\n===============================================\n");
}

tFilial *buscarFilial(tFilial *filiais, int id)
{
	while (filiais && filiais->id_filial != id)
		filiais = filiais->prox;
	return filiais;
}

tProdutos *buscarProduto(tProdutos *catalogo, int cod)
{
	while (catalogo && catalogo->codigo != cod)
		catalogo = catalogo->prox;

	return catalogo;
}

/*
Caso o usuario deseje finalizar o pedido em uma filial que não atende 100% do pedido
reduz do carrinho do usuario até que a filial possa atender o pedido.
Caso os itens fiquem zerados, eles são removidos
Exemplo:
	Quero 5 dipironas, mas na filial 101 só tem 4
	reduz do carrinho de 5 dipironas para 4 dipironas
	também reduz o valor final, claro.
*/
int ajustarCarrinhoParaFilial(tCarrinho *carrinho, tFilial *filial)
{
	if (carrinho->itens == NULL)
	{
		printf("\nCarrinho vazio!\n");
		return 0;
	}

	tItemCarrinho *item = carrinho->itens;
	tItemCarrinho *anterior = NULL;
	carrinho->total = 0;

	printf("\n--- AJUSTANDO CARRINHO PARA A FILIAL %d - %s ---\n",
				 filial->id_filial, filial->nome);

	while (item != NULL)
	{
		// Buscar estoque correspondente
		tEstoque *est = filial->estoque;
		int qtdEstoque = 0;

		while (est != NULL)
		{
			if (est->codigo_produto == item->codigo_produto)
			{
				qtdEstoque = est->quantidade;
				break;
			}
			est = est->prox;
		}

		int faltante = item->quantidade - qtdEstoque;

		if (faltante > 0)
		{
			printf("Produto %d: faltam %d unidades. Ajustando...\n",
						 item->codigo_produto, faltante);

			item->quantidade -= faltante;

			if (item->quantidade <= 0)
			{
				// remover do carrinho
				printf("-> Produto %d removido do carrinho (zerado).\n", item->codigo_produto);

				if (anterior == NULL)
				{
					carrinho->itens = item->prox;
				}
				else
				{
					anterior->prox = item->prox;
				}

				tItemCarrinho *temp = item;
				item = item->prox;
				free(temp);
				continue;
			}
		}

		// subtotal recalculado
		carrinho->total += item->quantidade * item->preco_unitario;

		anterior = item;
		item = item->prox;
	}

	// Caso o carrinho tenha ficado vazio
	if (carrinho->itens == NULL)
	{
		printf("\nNenhum item restante para retirada nesta filial.\n");
		return 0;
	}

	// Verificação final — agora deve estar 100% apta
	if (filialAtendeTotal(carrinho, filial) == 1)
	{
		printf("\nCarrinho ajustado com sucesso. Filial agora atende 100%%.\n");
		return 1;
	}
	else
	{
		printf("\nERRO: Após ajuste, ainda há itens sem estoque!\n");
		return 0;
	}
}

void finalizarCompra(tCarrinho *carrinho, tFilial *filiais, tProdutos *catalogo, int id_filial)
{
	tFilial *filial = buscarFilial(filiais, id_filial);

	if (!filial)
	{
		printf("\nFilial invalida!\n");
		return;
	}

	// Salva a ultima compra realizada no programa em um novo arquivo txt
	FILE *relCompra = fopen("ultima_compra.txt", "w");
	if (relCompra != NULL)
	{
		fprintf(relCompra, "===========================================\n");
		fprintf(relCompra, "   COMPROVANTE - FILIAL %d\n", filial->id_filial);
		fprintf(relCompra, "===========================================\n\n");

		tItemCarrinho *itens = carrinho->itens;
		while (itens)
		{
			tProdutos *aux = buscarProduto(catalogo, itens->codigo_produto);
			if (aux)
			{
				fprintf(relCompra, "%s x%d - R$ %.2f\n",
								aux->descricao, itens->quantidade,
								itens->quantidade * itens->preco_unitario);
			}
			itens = itens->prox;
		}
		fprintf(relCompra, "\nTOTAL: R$ %.2f\n", carrinho->total);
		fclose(relCompra);
	}
	if (filialAtendeTotal(carrinho, filial) == 0)
	{
		printf("\nFilial nao atende totalmente. Ajustando carrinho...\n");

		int ok = ajustarCarrinhoParaFilial(carrinho, filial);

		if (ok == 0)
		{
			printf("\nVenda cancelada. Carrinho vazio apos ajuste.\n");

			// Salva cancelamento no histórico
			FILE *rel = fopen("relatorio_geral.txt", "a");
			if (rel)
			{
				fprintf(rel, "\n>>> VENDA CANCELADA - Filial %d (%s)\n",
								filial->id_filial, filial->nome);
				fprintf(rel, "Motivo: Carrinho ficou vazio apos ajuste\n\n");
				fclose(rel);
			}
			return;
		}

		printf("\nCarrinho ajustado. Continuando venda...\n");
	}

	// ========== ABATE ESTOQUE ==========
	tItemCarrinho *itemCarrinho = carrinho->itens;
	while (itemCarrinho)
	{
		tEstoque *est = filial->estoque;
		while (est && est->codigo_produto != itemCarrinho->codigo_produto)
			est = est->prox;
		if (est)
			est->quantidade -= itemCarrinho->quantidade;
		itemCarrinho = itemCarrinho->prox;
	}

	// ========== SALVAR NO HISTÓRICO GERAL ==========
	FILE *rel = fopen("relatorio_geral.txt", "a");
	if (rel != NULL)
	{
		fprintf(rel, "\n===========================================\n");
		fprintf(rel, "     COMPRA FINALIZADA - FILIAL %d\n", filial->id_filial);
		fprintf(rel, "===========================================\n");
		fprintf(rel, "Filial: %s (%s)\n\n", filial->nome, filial->cidade);

		fprintf(rel, "Itens comprados:\n");
		tItemCarrinho *itens = carrinho->itens;
		while (itens)
		{
			tProdutos *aux = buscarProduto(catalogo, itens->codigo_produto);
			if (aux)
				fprintf(rel, "  %d x %s (cod %d) = R$ %.2f\n",
								itens->quantidade, aux->descricao, aux->codigo,
								itens->quantidade * itens->preco_unitario);
			itens = itens->prox;
		}

		fprintf(rel, "\nTOTAL: R$ %.2f\n", carrinho->total);
		fprintf(rel, "===========================================\n\n");

		fflush(rel); // Força a gravação no arquivo
		fclose(rel);
		printf("Historico atualizado!\n");
	}
	else
	{
		printf("ERRO ao abrir relatorio_geral.txt\n");
	}

	// ========== Atualiza o estoque ==========
	FILE *arqEst = fopen("estoques_v2.txt", "w");
	if (arqEst != NULL)
	{
		tFilial *f = filiais;
		while (f)
		{
			tEstoque *e = f->estoque;
			while (e)
			{
				fprintf(arqEst, "%d %d %d\n", f->id_filial, e->codigo_produto, e->quantidade);
				e = e->prox;
			}
			f = f->prox;
		}
		fclose(arqEst);
	}

	// ========== RESUMO NO TERMINAL ==========
	printf("\n========= COMPRA FINALIZADA =========\n");
	printf("Filial: %d - %s\n", filial->id_filial, filial->nome);
	printf("TOTAL: R$ %.2f\n", carrinho->total);
	printf("=====================================\n");

	// ========== LIMPAR CARRINHO ==========
	tItemCarrinho *aux = carrinho->itens;
	while (aux)
	{
		tItemCarrinho *tmp = aux;
		aux = aux->prox;
		free(tmp);
	}
	carrinho->itens = NULL;
	carrinho->total = 0;
}

/*
Função de comandos
Faz a leitura do documento que normalmente é o carrinhos_clientes_v2
e executa os comandos especificados no arquivo (ADD, REMOVE, VERIFICAR_ATENDIMENTO, FINALIZAR)

*/
void comandos(char *documentoTXT, tProdutos *catalogo, tFilial *filiais)
{
	FILE *arquivo = fopen(documentoTXT, "r");
	if (arquivo == NULL)
	{
		printf("\nErro ao abrir o arquivo ---- %s", documentoTXT);
		return;
	}

	tCarrinho carrinho;
	iniciarCarrinho(&carrinho);

	char comando[40];
	int id_carrinho, codigo, quantidade, id_filial;

	while (fscanf(arquivo, "%s", comando) == 1)
	{
		if (strcmp(comando, "NOVO_CARRINHO") == 0)
		{
			fscanf(arquivo, "%d", &id_carrinho);
			printf("\n[COMANDO] NOVO_CARRINHO %d\n", id_carrinho);

			iniciarCarrinho(&carrinho);
			printf("Carrinho %d criado.\n", id_carrinho);
		}
		else if (strcmp(comando, "ADD") == 0)
		{
			fscanf(arquivo, "%d %d", &codigo, &quantidade);
			printf("\n[COMANDO] ADD %d %d\n", codigo, quantidade);
			inserirCarrinho(&carrinho, catalogo, codigo, quantidade);
		}
		else if (strcmp(comando, "REMOVE") == 0)
		{
			fscanf(arquivo, "%d %d", &codigo, &quantidade);
			printf("\n[COMANDO] REMOVE %d %d\n", codigo, quantidade);
			removerQuantiaCarrinho(&carrinho, codigo, quantidade);
		}
		else if (strcmp(comando, "VERIFICAR_ATENDIMENTO") == 0)
		{
			printf("\n[COMANDO] VERIFICAR_ATENDIMENTO\n");
			verificaDisponibilidade(&carrinho, filiais, catalogo);
		}
		else if (strcmp(comando, "FINALIZAR") == 0)
		{
			fscanf(arquivo, "%d", &id_filial);
			printf("\n[COMANDO] FINALIZAR %d\n", id_filial);
			finalizarCompra(&carrinho, filiais, catalogo, id_filial);
		}
		else if (strcmp(comando, "FIM") == 0)
		{
			printf("\n[COMANDO] FIM\n");
		}
		else
		{
			printf("\n[COMANDO] Comando desconhecido: %s\n", comando);
		}
	}

	fclose(arquivo);
	printf("\n========== FIM ==========\n");
}

/*
Funções para Liberar a memoria alocada das structs
*/
void liberarProdutos(tProdutos *lista)
{
	tProdutos *aux;
	while (lista)
	{
		aux = lista;
		lista = lista->prox;
		free(aux);
	}
}

void liberarEstoques(tEstoque *estoque)
{
	tEstoque *aux;
	while (estoque)
	{
		aux = estoque;
		estoque = estoque->prox;
		free(aux);
	}
}

void liberarFiliais(tFilial *lista)
{
	tFilial *aux;
	while (lista)
	{
		liberarEstoques(lista->estoque);
		aux = lista;
		lista = lista->prox;
		free(aux);
	}
}

void liberarCarrinho(tCarrinho *carrinho)
{
	tItemCarrinho *aux = carrinho->itens;
	tItemCarrinho *tmp = NULL;
	while (aux)
	{
		tmp = aux;
		aux = aux->prox;
		free(tmp);
	}
	carrinho->itens = NULL;
	carrinho->total = 0;
}
