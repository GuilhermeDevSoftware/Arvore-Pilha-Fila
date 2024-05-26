#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <malloc.h>

// Estruturas Basicas : Pessoa e Veículo
typedef struct {
    char placa[20];
    char tipo; // m=moto; a=automovel
    char marca[20];
    char modelo[20];
    short int ano;
} Veiculo;

typedef struct NoDeFilaVeic {
    Veiculo v;
    struct NoDeFilaVeic* proximo; // fila simplesmente ligada
} NoDeFilaVeic; // uma pessoa tem zero ou mais veiculos

void mostrarVeiculo(Veiculo v) {
    printf("\nPlaca: %s", v.placa);
    printf("\nTipo: %s", (v.tipo == 'm' ? "moto" : "automÃ³vel"));
    printf("\nMarca: %s", v.marca);
    printf("\nModelo: %s", v.modelo);
    printf("\nAno: %d\n", v.ano);
}

typedef struct {
    char cpf[20];
    char nome[40];
    NoDeFilaVeic* primeiro;
} Pessoa;

void mostrarPessoa(Pessoa p) {
    printf("\nCPF: %s", p.cpf);
    printf("\nNome: %s", p.nome);
    NoDeFilaVeic* possante = p.primeiro;
    while (possante != NULL) {
        mostrarVeiculo(possante->v);
        possante = possante->proximo;
    }
}

// Estruturas auxiliares : pilha, filas e Ã¡rvores

// Árvore de Pessoas
typedef struct NoDeArvore {
    Pessoa p;
    struct NoDeArvore* esq;
    struct NoDeArvore* dir;
} NoDeArvore; // Nó de arvore de Pessoas

typedef NoDeArvore* Arvore;

// Fila de Pessoas
typedef struct NoDeFilaPessoas {
    Pessoa p;
    struct NoDeFilaPessoas* proximo;
} NoDeFilaPessoas;

typedef struct {
    NoDeFilaPessoas* primeiro;
    NoDeFilaPessoas* ultimo;
} FilaPessoas;

// Pilha de Pessoas
typedef struct NoDePilhaPessoas {
    Pessoa p;
    struct NoDePilhaPessoas* inferior;
} NoDePilhaPessoas;

typedef struct {
    NoDePilhaPessoas* topo;
} PilhaPessoas;

// Funções para Fila de Pessoas
void initFila(FilaPessoas* f) {
    f->primeiro = NULL;
    f->ultimo = NULL;
}

void enfileirar(FilaPessoas* f, Pessoa novaPessoa) {
    NoDeFilaPessoas* novoNo = (NoDeFilaPessoas*)malloc(sizeof(NoDeFilaPessoas));
    novoNo->p = novaPessoa;
    novoNo->proximo = NULL;
    if (f->ultimo != NULL) {
        f->ultimo->proximo = novoNo;
    }
    f->ultimo = novoNo;
    if (f->primeiro == NULL) {
        f->primeiro = novoNo;
    }
}

Pessoa desenfileirar(FilaPessoas* f) {
    if (f->primeiro == NULL) {
        Pessoa p;
        strcpy(p.cpf, "");
        return p; // Fila vazia
    }
    NoDeFilaPessoas* temp = f->primeiro;
    f->primeiro = f->primeiro->proximo;
    if (f->primeiro == NULL) {
        f->ultimo = NULL;
    }
    Pessoa p = temp->p;
    free(temp);
    return p;
}

void mostrarFila(FilaPessoas* f) {
    NoDeFilaPessoas* atual = f->primeiro;
    while (atual != NULL) {
        mostrarPessoa(atual->p);
        atual = atual->proximo;
    }
}

// Funções para Pilha de Pessoas
void initPilha(PilhaPessoas* p) {
    p->topo = NULL;
}

void empilhar(PilhaPessoas* p, Pessoa novaPessoa) {
    NoDePilhaPessoas* novoNo = (NoDePilhaPessoas*)malloc(sizeof(NoDePilhaPessoas));
    novoNo->p = novaPessoa;
    novoNo->inferior = p->topo;
    p->topo = novoNo;
}

Pessoa desempilhar(PilhaPessoas* p) {
    if (p->topo == NULL) {
        Pessoa p;
        strcpy(p.cpf, "");
        return p; // Pilha vazia
    }
    NoDePilhaPessoas* temp = p->topo;
    p->topo = p->topo->inferior;
    Pessoa pessoa = temp->p;
    free(temp);
    return pessoa;
}

void mostrarPilha(PilhaPessoas* p) {
    NoDePilhaPessoas* atual = p->topo;
    while (atual != NULL) {
        mostrarPessoa(atual->p);
        atual = atual->inferior;
    }
}

// Funções para Árvore de Pessoas
void inserirNaArvore(Arvore* a, Pessoa novaPessoa) {
    if (*a == NULL) {
        *a = (NoDeArvore*)malloc(sizeof(NoDeArvore));
        (*a)->p = novaPessoa;
        (*a)->esq = NULL;
        (*a)->dir = NULL;
    } else {
        if (strcmp(novaPessoa.cpf, (*a)->p.cpf) < 0) {
            inserirNaArvore(&((*a)->esq), novaPessoa);
        } else {
            inserirNaArvore(&((*a)->dir), novaPessoa);
        }
    }
}

void mostrarArvore(Arvore a) {
    if (a != NULL) {
        mostrarArvore(a->esq);
        mostrarPessoa(a->p);
        mostrarArvore(a->dir);
    }
}

// Funções para carregar e salvar dados
void carregarDados(FilaPessoas* f) {
    FILE* fp = fopen("pessoas.dat", "rb");
    if (fp != NULL) {
        Pessoa p;
        while (fread(&p, sizeof(Pessoa), 1, fp)) {
            enfileirar(f, p);
        }
        fclose(fp);
    }
}

void salvarDadosRecursivo(Arvore a, FILE* fp) {
    if (a != NULL) {
        fwrite(&(a->p), sizeof(Pessoa), 1, fp);
        salvarDadosRecursivo(a->esq, fp);
        salvarDadosRecursivo(a->dir, fp);
    }
}

void salvarDados(Arvore a) {
    FILE* fp = fopen("pessoas.dat", "wb");
    if (fp != NULL) {
        salvarDadosRecursivo(a, fp);
        fclose(fp);
    }
}



// Funções para manipular veiculos
void inserirVeiculo(Pessoa* p) {
    Veiculo novoVeiculo;
    printf("\nPlaca: ");
    scanf("%s", novoVeiculo.placa);
    printf("Tipo (m=a moto, a=automovel): ");
    scanf(" %c", &novoVeiculo.tipo); 
    printf("Marca: ");
    scanf("%s", novoVeiculo.marca);
    printf("Modelo: ");
    scanf("%s", novoVeiculo.modelo);
    printf("Ano: ");
    scanf("%hd", &novoVeiculo.ano);

    NoDeFilaVeic* novoNo = (NoDeFilaVeic*)malloc(sizeof(NoDeFilaVeic));
    novoNo->v = novoVeiculo;
    novoNo->proximo = p->primeiro;
    p->primeiro = novoNo;
}

Pessoa* buscarPessoaNaFila(FilaPessoas* f, char* cpf) {
    NoDeFilaPessoas* atual = f->primeiro;
    while (atual != NULL) {
        if (strcmp(atual->p.cpf, cpf) == 0) {
            return &(atual->p);
        }
        atual = atual->proximo;
    }
    return NULL;
}

Pessoa* buscarPessoaNaPilha(PilhaPessoas* p, char* cpf) {
    NoDePilhaPessoas* atual = p->topo;
    while (atual != NULL) {
        if (strcmp(atual->p.cpf, cpf) == 0) {
            return &(atual->p);
        }
        atual = atual->inferior;
    }
    return NULL;
}

Pessoa* buscarPessoaNaArvore(Arvore a, char* cpf) {
    if (a == NULL) {
        return NULL;
    }
    if (strcmp(cpf, a->p.cpf) == 0) {
        return &(a->p);
    } else if (strcmp(cpf, a->p.cpf) < 0) {
        return buscarPessoaNaArvore(a->esq, cpf);
    } else {
        return buscarPessoaNaArvore(a->dir, cpf);
    }
}

// Funções principais do programa
void inserirPessoa(FilaPessoas* f) {
    Pessoa novaPessoa;
    printf("\nCPF: ");
    scanf("%s", novaPessoa.cpf);
    printf("Nome: ");
    scanf("%s", novaPessoa.nome);
    novaPessoa.primeiro = NULL;
    enfileirar(f, novaPessoa);
}

void dePilhaParaArvore(PilhaPessoas* p, Arvore* a) {
    Pessoa pessoa = desempilhar(p);
    if (strcmp(pessoa.cpf, "") != 0) { // Verifica se a pessoa desempilhada são validas
        inserirNaArvore(a, pessoa);
    }
}

void dePilhaParaFila(PilhaPessoas* p, FilaPessoas* f) {
    Pessoa pessoa = desempilhar(p);
    if (strcmp(pessoa.cpf, "") != 0) { // Verifica se a pessoa desempilhada são validas
        enfileirar(f, pessoa);
    }
}

void deFilaParaArvore(FilaPessoas* f, Arvore* a) {
    Pessoa pessoa = desenfileirar(f);
    if (strcmp(pessoa.cpf, "") != 0) { // Verifica se a pessoa desenfileirada são validas
        inserirNaArvore(a, pessoa);
    }
}

void deFilaParaPilha(FilaPessoas* f, PilhaPessoas* p) {
    Pessoa pessoa = desenfileirar(f);
    if (strcmp(pessoa.cpf, "") != 0) { // Verifica se a pessoa desenfileirada são validas
        empilhar(p, pessoa);
    }
}

int mostraOpcoes() {
    int escolha;
    printf("\n --- O menu ---\n");
    printf("\n1. Inserir nova pessoa no final da Fila");
    printf("\n2. Retirar pessoa da Pilha e inserir na Árvore");
    printf("\n3. Retirar pessoa da Pilha e inserir na Fila");
    printf("\n4. Retirar pessoa da Fila e inserir na Árvore");
    printf("\n5. Retirar pessoa da Fila e empilhar na Pilha");
    printf("\n6. Mostrar as pessoas da Fila (sem desenfileirar)");
    printf("\n7. Mostrar as pessoas da Pilha (sem desempilhar)");
    printf("\n8. Mostrar as pessoas da Árvore (visitação em ordem)");
    printf("\n9. Inserir veículo para uma pessoa");
    printf("\n10. Sair do Programa e salvar os dados");
    printf("\n\nDigite o número da opção e tecle <ENTER>: ");
    scanf("%d", &escolha);
    return escolha;
}

void menu() {
    FilaPessoas fila;
    PilhaPessoas pilha;
    Arvore arvore = NULL;

    initFila(&fila);
    initPilha(&pilha);

    carregarDados(&fila);

    int opcao;
    do {
        opcao = mostraOpcoes();
        switch (opcao) {
            case 1:
                inserirPessoa(&fila);
                break;
            case 2:
                dePilhaParaArvore(&pilha, &arvore);
                break;
            case 3:
                dePilhaParaFila(&pilha, &fila);
                break;
            case 4:
                deFilaParaArvore(&fila, &arvore);
                break;
            case 5:
                deFilaParaPilha(&fila, &pilha);
                break;
            case 6:
                mostrarFila(&fila);
                break;
            case 7:
                mostrarPilha(&pilha);
                break;
            case 8:
                mostrarArvore(arvore);
                break;
            case 9:
            {
                char cpf[20];
                printf("\nDigite o CPF da pessoa para adicionar um veículo: ");
                scanf("%s", cpf);
                Pessoa* p = buscarPessoaNaFila(&fila, cpf);
                if (p == NULL) {
                    p = buscarPessoaNaPilha(&pilha, cpf);
                    if (p == NULL) {
                        p = buscarPessoaNaArvore(arvore, cpf);
                    }
                }
                if (p != NULL) {
                    inserirVeiculo(p);
                } else {
                    printf("\nPessoa não encontrada!\n");
                }
                break;
            }
            case 10:
                salvarDados(arvore);
                break;
        }
    } while (opcao != 10);
}



int main() {
    setlocale(LC_ALL, "Portuguese");
    menu();
    return 0;
}
