//Busca de normas da UEFS//
/*****************************************************************************************
Autor: William Oliveira Soares
Componente Curricular: MI Algoritmos I
Concluido em: 30/12/2017
Declaro que este código foi elaborado por mim de forma individual e não contém nenhum
trecho de código de outro colega ou de outro autor, tais como provindos de livros e
apostilas, e páginas ou documentos eletrônicos da Internet. Qualquer trecho de código
de outra autoria que não a minha está destacado com uma citação para o autor e a fonte
do código, e estou ciente que estes trechos não serão considerados para fins de avaliação.
******************************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define c 50

typedef struct lista{
    char palavra[c]; //Palavra encontrada no texto
    int quantidade; //Vezes em que a palavra foi encontrada no texto
    char arquivo[c]; //Nome do arquivo onde contém as palavras
    struct lista *prox; //Endereço da próxima célula
}lista;

int Verificando_Palavra(); 
lista* Adicionando_Palavra();  
void Procurando_palavra();
void salvando_lista();
lista* Atribuindo_indice();
lista* Adicionando_indice();
void Numero_arquivos();

int main(int argc, char *argv[]){
    SetConsoleOutputCP(65001); //Faz com que o windows reconheça os acentos da lingua portuguesa
    if (argv[3]!=NULL && argv[2]!=NULL && argv[1]!=NULL){ //Caso aconteça de ter mais de 2 argumentos
        printf("\aERROR 400\nMá solicitação da execução do programa!\nO programa será executado com apenas 2 argumentos\n");
    }
    if(argv[1] == NULL){ //Se estiver apenas com o executável do programa
        printf("\aERROR 403\nProibido a execução do programa!\n");
        return 0;
    }
    int debug; //Variavel que determina como verdadeiro ou falso para uma determinada ação
    lista *i; //Lista do indice invertido
    i=(lista*)malloc(sizeof(lista));//Alocação dinamica
    FILE *file;//Ponteiro do arquivo
    i = Atribuindo_indice(i, file);//Pegando o que tem na lista.txt
    strcat(argv[1], ".txt");
    file=fopen(argv[1], "r");
    if(file == NULL){ //Caso o arquivo não exista
        printf("\aERROR 404\nArquivo não encontrado!\n");
        free(i);
        return 0;
    }
    /***************************************************
    Neste if verifica se a lista.txt existe, se existir,
    verifica se o arquivo desejado ja está na lista, se
    estiver, ele procura pela palavra desejada na lista,
    retornando a quantidade de vezes que a palavra foi
    encontrada e o numero de arquivos que foi procurado
    ****************************************************/
    if(i!=NULL){
        lista *aux;
        aux=i;
        while(aux!=NULL){
            if(strcmp(aux->arquivo, argv[1])==0){
                Procurando_palavra(i, argv[2], debug);
                Numero_arquivos(i);
                free(i);
                return 0;
            }
            aux=aux->prox;
        }
    }
    lista *l;//Lista generica, caso o arquivo pedido não estiver na lista.txt
    l=(lista*)malloc(sizeof(lista));
    l=NULL;
    char conteudo[c]; //Palavra que está no arquivo
    char *arg; 
    /**************************************************
    Assume a palavra que está no arquivo sem o espaço, 
    ',', '.', '()', '!', '?', são os caracteres que mais
    aparece em textos.
    ***************************************************/
    while(!feof(file)){//Enquanto não chegar no fim do arquivo
        fscanf(file, "%s", conteudo);//Pega palavra por palavra
        arg = strtok(conteudo," ,.():!?");//Retira os caracteres
        debug = Verificando_Palavra(l,arg);//Verifica se a palavra ja esta na lista
        if (debug==0){
            l=Adicionando_Palavra(l, arg, argv[1]);//Se não está na lista ela é adicionada
        }
        arg = strtok(NULL, " ,.():!?");
    }
    if(argv[2]==NULL){//Se só estiver o nome do arquivo, é retornado essa mensagem
        printf("Arquivo adicionado a lista para pesquisas futuras\n");
        salvando_lista(l, file);
        free(l);
        free(i);
        return 0;
    }
    else{
        Procurando_palavra(l, argv[2], debug);
    }
    salvando_lista(l, file);//Salvando a lista generica na lista.txt
    free(l);//Liberando a memoria alocada
    free(i);
    return 0;
} 

int Verificando_Palavra(lista *l, char conteudo[c]){
    /**********************************************
    Nesta função verifica se a palavra recebida do
    arquivio ja está na lista, se sim, modifica o
    debug para 1, e acrescenta +1 na variavel
    quantidade da celula onde a palavra é igual
    ***********************************************/
    int debug=0;
    lista *aux;
    aux=l;
    while(aux!=NULL){
        if (strcmp(aux->palavra, conteudo)==0){
            debug=1;
            aux->quantidade++;
        }
        aux=aux->prox;
    }
    return debug;
}

lista* Adicionando_Palavra(lista* l, char conteudo[c], char arq[c]){
    /*******************************************************************
    Nesta função só é executada se o debug da função Verificando_Palavra
    ser 0, então essa função adiciona a palavra do arquivo na lista.
    Não foi preciso desalocar a lista porque quando uma fução acaba, 
    todo conteudo é descartado.
    *******************************************************************/
    lista *aux;
    aux=(lista*)malloc(sizeof(lista));
    strcpy(aux->palavra, conteudo);
    strcpy(aux->arquivo, arq);
    aux->quantidade=1;
    aux->prox=l;
    l=aux;
    return l;
}

void Procurando_palavra(lista *l, char nome[c], int debug){
    /*******************************************************************
    Esta função procura tanto na lista generica como na lista do indice
    invertido, por isso a ideia da lista generica, para deixar o codigo
    mais limpo e mais rapido. A variavel debug retorna para indicar se
    a palavra foi encontrada (1) ou não (0), e também a um tratamendo de
    plural ou singular pela quantidade de vezes do arquivo.
    *******************************************************************/
    lista *aux;
    aux=l;
    debug=0;
    while(aux!=NULL){
        if (strcmp(aux->palavra, nome)==0){
            if (aux->quantidade==1){
                printf("Palavra foi encontrada %d vez no arquivo '%s'\n", aux->quantidade, aux->arquivo);
            }
            else{
                printf("Palavra foi encontrada %d vezes no arquivo '%s'\n", aux->quantidade, aux->arquivo);
            }
            debug=1;
        }
        aux=aux->prox;
    }
    if (debug==0){
        printf("Palavra não encontrada\n");
    }
}

void Numero_arquivos(lista *l){
    /**********************************************
    Nesta função verifica a quantidade de arquivos 
    na lista invertida pela a verificação do nome 
    do arquivo, toda vez que há uma diferença o 
    numero de arquivos é aumentado.
    ***********************************************/
    lista *aux, *auxil;
    int x=1;
    aux=l;
    auxil=aux->prox;
    while(aux!=NULL && auxil!=NULL){
        if(strcmp(aux->arquivo,auxil->arquivo)!=0){
            x++;
        }
        aux=aux->prox;
        auxil=auxil->prox;
    }
    printf("Quantidade total de arquivos: %d\n", x);
}

void salvando_lista(lista *l, FILE *file){
    /*****************************************************
    Esta função salva a lista generica na lista invertida.
    *****************************************************/
    lista *aux;
    aux=l;
    file = fopen("lista.txt","a");
    while(aux!=NULL){
        fprintf(file, "%s %i %s\n", aux->palavra, aux->quantidade, aux->arquivo);
        aux=aux->prox;
    }
    fprintf(file, "\n");
    fclose(file);
}

lista* Atribuindo_indice(lista *l, FILE *file){
    /************************************************************
    Esta função salva a lista.txt na lista invertida do programa.
    ************************************************************/
    l=NULL;
    file=fopen("lista.txt", "r");
    if (file==NULL){
        return l;
    }
    lista *aux;
    char info[c];
    char *depurar;
    int i=0;
    while(!feof(file)){
        l=Adicionando_indice(aux, l, depurar, i, info, file);
    }
    return l;
}

lista* Adicionando_indice(lista *aux, lista *l, char depurar[c], int i, char info[100], FILE *file){
    /*****************************************************
    Esta função é de atribuição dos argumentos da lista.txt
    na lista 'i' do programa, como sabemos como é escrito a
    lista.txt, foi usado a variavel 'i' para indicar qual é 
    o arquivo que está sendo adicionado na célula, sendo 0
    a palavra, 1 a quantidade e 2 o nome do arquivo.
    *****************************************************/
    aux=(lista*)malloc(sizeof(lista));
    for(i=0;i<3;i++){
        fscanf(file, "%s", info);
        depurar=strtok(info, " ");
        while(depurar != NULL){
            if(i==0){
                strcpy(aux->palavra, depurar);
                depurar=strtok(NULL, " ");
            }
            else if(i==1){
                int x;
                x=atoi(depurar);
                aux->quantidade=x;
                depurar=strtok(NULL, " ");
            }
            else if(i==2){
                strcpy(aux->arquivo,depurar);
                depurar=strtok(NULL, " ");
            }
        }
    }
    aux->prox=l;
    l=aux;
    return l;
}
// FIM ;)