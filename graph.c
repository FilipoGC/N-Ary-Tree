#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <deque>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\e[0;32m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define STYLE_BOLD "\033[1m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"

using namespace std;

struct node{
    int nivel, type;
    char *name;
    vector<struct node*> *filhos;
    struct node* pai;
};
typedef struct node Node;

Node* criaNode(int nv, char *nam, int t, Node* father){
    Node *p = (Node*) malloc (sizeof(Node));
    p->name = (char*) malloc (100 * sizeof(char));
    p->nivel = nv;
    if(nam!=NULL)
        strcpy(p->name, nam);
    else p->name = NULL;
    p->filhos = new vector<Node*>();
    p->type = t;
    p->pai = father;
    
    return p;
}

//Limpa o buffer do teclado
void flush_in(){ 
    int ch;
    while( (ch = fgetc(stdin)) != EOF && ch != '\n' ){} 
}

void ls(Node* noAtual){
    for(int i=0;i<noAtual->filhos->size();i++) {
        if(noAtual->filhos->at(i)->type == 1)
            printf(ANSI_COLOR_BLUE STYLE_BOLD  "%s ", noAtual->filhos->at(i)->name);
        else
            printf(ANSI_COLOR_RESET "%s ", noAtual->filhos->at(i)->name);
    }
    printf(ANSI_COLOR_RESET "\n");
}

int rm(Node* noAtual, char* arquivo){
	for(int i = 0; i < noAtual->filhos->size(); i++) {
		if( strcmp(noAtual->filhos->at(i)->name, arquivo)==0) {
			noAtual->filhos->erase(noAtual->filhos->begin()+i);
			return 1;
		}
	}
	printf("rm: não foi possível remover '%s': Arquivo ou diretório inexistente\n", arquivo);
	return 0;

}

Node* cd(Node* noAtual, char* arquivo) {
	for(int i = 0; i < noAtual->filhos->size(); i++) {
		if( strcmp(noAtual->filhos->at(i)->name, arquivo)==0)
			return noAtual->filhos->at(i);
	}
	printf("bash: cd: %s: Arquivo ou diretório inexistente\n", arquivo);

	return NULL;
}

void printCaminho(Node* noAtual, Node* raiz) {
	deque<char*> *caminho = new deque<char*> ();
	Node* p = noAtual;
	while(p != NULL){
		caminho->push_front(p->name);
		p = p->pai;
	
	}
	for(int i = 0; i < caminho->size() - 1; i++){
		printf(ANSI_COLOR_BLUE STYLE_BOLD "%s/" ANSI_COLOR_RESET, caminho->at(i));
	}
	printf(ANSI_COLOR_BLUE STYLE_BOLD "%s$" ANSI_COLOR_RESET, caminho->at(caminho->size()-1));



}

void pwd(Node* noAtual) {
	deque<char*> *caminho = new deque<char*> ();
	Node* p = noAtual;
	while(p != NULL){
		caminho->push_front(p->name);
		p = p->pai;
	
	}
	for(int i = 0; i < caminho->size() - 1; i++){
		if(i != 0)
			printf("%s/" ANSI_COLOR_RESET, caminho->at(i));
		else
			printf("/" ANSI_COLOR_RESET);
	}
	printf( "%s\n" ANSI_COLOR_RESET, caminho->at(caminho->size()-1));



}

void mv(Node* noAtual, char* origem, char* destino){
	Node* aux = NULL;
	for(int i = 0; i < noAtual->filhos->size(); i++) {
		if( strcmp(noAtual->filhos->at(i)->name, origem)==0){
			aux = noAtual->filhos->at(i);
			noAtual->filhos->erase(noAtual->filhos->begin()+i);
		}	
	
	}
	if(aux == NULL){
		printf("mv: não foi possível obter estado de '%s': Arquivo ou diretório inexistente\n", origem);
		return;
	}
	for(int i = 0; i < noAtual->filhos->size(); i++) {
		if( strcmp(noAtual->filhos->at(i)->name, destino)==0){
			noAtual->filhos->at(i)->filhos->push_back(aux);
			return;
		}
	}
	printf("mv: não foi possível obter estado de '%s': Arquivo ou diretório inexistente\n", destino);
}

int main (){
    char opa[10]= "~";
    char a[30] = "Verao2008";
    char b[30] = "FigurinhaArthurPlagio.png";
    char c[30] = "Fotos_Paulin_Pinho";
    char d[30] = "Trabalhos_Giusepe_Cagroça";
    char e[30] = "ThieloVSLuizMarenco.gif";

    Node *raiz = criaNode(1, opa, 0, NULL);
    
    printf(ANSI_COLOR_RED STYLE_BOLD "Comandos disponíveis: ls, mkdir, exit, cd.., cd, pwd, mv, rm\n" ANSI_COLOR_RESET);
    Node* noAtual = raiz;
    raiz->filhos->push_back(criaNode(raiz->nivel+1, a, 1, raiz));
    raiz->filhos->push_back(criaNode(raiz->nivel+1, b, 0, raiz));
    raiz->filhos->push_back(criaNode(raiz->nivel+1, c, 1, raiz));
    raiz->filhos->push_back(criaNode(raiz->nivel+1, d, 1, raiz));
    raiz->filhos->push_back(criaNode(raiz->nivel+1, e, 0, raiz));
    while(true){
    	char command[100], command1[100], command2[100], command3[100];
    	for(int i = 0; i < 100; i++){
        	command[i] = '\0';
        	command1[i] = '\0';
        	command2[i] = '\0';
        	command3[i] = '\0';
        }
    	printCaminho(noAtual, raiz);
    	//printf("%s$", noAtual->name);
    	//flush_in();
        for(int i = 0; i < 100; i++) command[i] = '*';
		scanf("%[^\n]", command);
		flush_in();	
		int cont = 0;
		int cont2 = 0;
		int cont3 = 0;
		//For pra poder separar strings com espaco ate 3 comandos
		for(int i = 0; i < 100; i++){
			if(command[i] == ' ') {
				cont++;
				i++;
			}
			if(cont == 0) { 
				if(command[i] != '*')
					command1[i] = command[i];
				else {
					command1[i] = '\0';
					break;
				}	
			}	
			if(cont == 1) {
				command1[i] = '\0';
				if(command[i] != '*') {
					command2[cont2] = command[i];
					cont2++;
				}
				else {
					command2[cont2] = '\0';
					break;
				}
			}
			if(cont == 2) {
				command2[cont2] = '\0';
				if(command[i] != '*') {
					command3[cont3] = command[i];
					cont3++;
				}
				else {
					command3[cont3] = '\0';		
					break;
				}
			}
		}
		if(strcmp(command1, "exit")==0) break;
        else if(strcmp(command1, "ls")==0) ls(noAtual);
        else if(strcmp(command1, "mkdir")==0) {
        	Node* aux = criaNode(noAtual->nivel+1, command2, 1, noAtual);
        	noAtual->filhos->push_back(aux);
        }
        else if(strcmp(command1, "rm")==0){ if(rm(noAtual, command2));;}
        else if(strcmp(command1, "cd")==0) {
        	Node* aux = cd(noAtual, command2);
        	if(aux != NULL)
        		noAtual = aux;
        }
        else if(strcmp(command1, "cd..")==0) {
        	Node* aux = noAtual->pai;
        	if(aux != NULL)
        		noAtual = aux;
        }
        else if(strcmp(command1, "pwd")==0) pwd(noAtual);
        else if(strcmp(command1, "mv")==0) {
        	mv(noAtual, command2, command3);	
        }
        for(int i = 0; i < 100; i++){
        	command[i] = '\0';
        	command1[i] = '\0';
        	command2[i] = '\0';
        	command3[i] = '\0';
        }
        //flush_in();
    }
    //printf("%s\n", command);

    //ls(raiz);
    return 1;;
}
