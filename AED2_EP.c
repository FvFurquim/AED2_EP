/*----------------------------------------------------------|
| NOMES DOS RESPONSÁVEIS: Felipe Furquim E Vitor Cairolli   |
|----------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
#include <malloc.h>
#include <string.h>

//ESCREVA O NROUSP DO PRIMEIRO INTEGRANTE AQUI
int nroUSP1() {
    return 11208030;
}

//ESCREVA O NROUSP DO SEGUNDO INTEGRANTE AQUI (OU DEIXE COM ZERO)
int  nroUSP2() {
    return 11276617;
}

//ESCREVA O NRO DO GRUPO CADASTRADO
int grupo() {
    return 6;
}

// AQUI COMEÇA O EP
typedef enum {false, true} bool;

typedef struct {
    int bot_lin;
    int bot_col;
    int exit_lin;
    int exit_col;
    int box_col;
    int key_col;
    bool haskey;
} ESTADO;

typedef struct aux{
    int v;
    bool fim;
    ESTADO* est;
    char* mov;
    char* cont;
    struct aux* prox;
}NO;

typedef struct {
    NO* inicio;
    ESTADO* estado;
    int flag;
    bool fim;
    char* movimento;
    char* contrario;
}VERTICE;

typedef struct {
    bool a;
}ACHOU;

/*Traducao de int para movimentos:
 0- left
 1- right
 2- up
 3- down
 4- push left
 5- push right
 6- grab
 7- exit
 */

bool movimentoPossivel(ESTADO* e, int movimento){
    
    switch(movimento){
        case 0:
            if(e->bot_col > 1 && e->bot_lin == 3)
                return true;
            return false;
            break;
        case 1:
            if(e->bot_col < 3 && e->bot_lin == 3)
                return true;
            return false;
            break;
        case 2:
            if(e->bot_lin == 3 && e->bot_col == e->box_col)
                return true;
            return false;
            break;
        case 3:
            if(e->bot_lin == 2 && e->bot_col == e->box_col)
                return true;
            return false;
            break;
        case 4:
            if(e->bot_lin == 3 && e->bot_col == e->box_col && e->bot_col > 1)
                return true;
            return false;
            break;
        case 5:
            if(e->bot_lin == 3 && e->bot_col == e->box_col && e->bot_col < 3)
                return true;
            return false;
            break;
        case 6:
            if(e->bot_lin == 2 && e->bot_col == e->key_col)
                return true;
            return false;
            break;
        case 7:
            if(e->bot_lin == e->exit_lin && e->bot_col == e->exit_col && e->haskey == true)
                return true;
            return false;
            break;
        default:
            printf("Parametro do movimentoPossivel invalido");
            return false;
    }
}

// Métodos que executam o movimento do bot

void left(ESTADO* e){
    e->bot_col--;
}

void right(ESTADO* e){
    e->bot_col++;
}

void up (ESTADO* e){
    e->bot_lin--;
}

void down (ESTADO* e){
    e->bot_lin = 3;
}

void pushLeft (ESTADO* e){
    e->bot_col--;
    e->box_col--;
}

void pushRight (ESTADO* e){
    e->bot_col++;
    e->box_col++;
}

void grab (ESTADO* e){
    e->haskey = true;
    e->key_col = 0;
}

void end (VERTICE* g, NO* n, int v){
    g[v].fim = true;
    n->fim = true;
    n->est->bot_lin = 0;
    n->est->bot_col = 0;
}

// Imprime o Estado
void printEstado(ESTADO* s){
    printf("bot_lin = %i\nbot_col = %i\nexit_lin = %i\nexit_col = %i\nbox_col = %i\nkey_col = %i\nhaskey = %i\n", s->bot_lin, s->bot_col, s->exit_lin, s->exit_col, s->box_col, s->key_col, s->haskey);
}

// Compara se dois estados são iguais
bool estadosIguais(ESTADO* e1, ESTADO* e2){
    if(e1->bot_col == e2->bot_col && e1->bot_lin == e2->bot_lin && e1->box_col == e2->box_col && e1->exit_col == e2->exit_col && e1->exit_lin == e2->exit_lin && e1->haskey == e2->haskey && e1->key_col == e2->key_col)
        return true;
    
    return false;
}

// Talvez alterar:
// Verifica se há uma aresta com o mesmo etado em um vertice
bool arestaExiste(VERTICE* g, int v1, ESTADO* novo){
    
    if(estadosIguais(g[v1].estado, novo)) return true;
    
    NO* ini = g[v1].inicio;
    while(ini){
        if(estadosIguais(ini->est, novo))return true;
        ini = ini->prox;
    }
    
    return false;
}

// Talvez alterar:
// Cria uma aresta
bool criarAresta(VERTICE* g, NO* novo, ESTADO* atual, int v1, int v2){
    if(arestaExiste(g, v1, novo->est))
        return false;
    
    NO* novo2 = (NO*) malloc(sizeof(NO));
    
    novo->v = v2;
    novo->prox = g[v1].inicio;
    g[v1].inicio = novo;
    
    novo2->v = v1;
    novo2->est = atual;
    novo2->prox = g[v2].inicio;
    g[v2].inicio = novo2;
    g[v2].estado = novo->est;
    g[v2].movimento = novo->mov;
    g[v2].contrario = novo->cont;
   
    return true;
}

// Copia os valors de um estado para o outro
void copiarEstados(ESTADO* e1, ESTADO* e2){
    e2->bot_col = e1->bot_col;
    e2->bot_lin = e1->bot_lin;
    e2->box_col = e1->box_col;
    e2->exit_col = e1->exit_col;
    e2->exit_lin = e1->exit_lin;
    e2->haskey = e1->haskey;
    e2->key_col = e1->key_col;
}

// Faz todo o trabalho de verificar se pode concluir o movimento para esquerda ou direita. Caso sim, ele faz isso
void mover(VERTICE* g, int i, int* indice, NO* no, int movi){
    
    ESTADO* estadoAlocado = (ESTADO*) malloc(sizeof(ESTADO));
    copiarEstados(g[i].estado, estadoAlocado);
    
    ESTADO* ant = (ESTADO*) malloc(sizeof(ESTADO));
    copiarEstados(g[i].estado, ant);
    
    no->est = estadoAlocado;
    
    switch(movi){
        case 0:
            left(no->est);
            if(!arestaExiste(g, i, no->est)){
                *indice = *indice + 1;
                no->mov = "l";
                no->cont = "r";
                criarAresta(g, no, ant, i, *indice);
            }
            break;
        case 1:
            right(no->est);
            if(!arestaExiste(g, i, no->est)){
                *indice = *indice + 1;
                no->mov = "r";
                no->cont = "l";
                criarAresta(g, no, ant, i, *indice);
            }
            break;
        case 2:
            up(no->est);
            if(!arestaExiste(g, i, no->est)){
                *indice = *indice + 1;
                no->mov = "u";
                no->cont = "d";
                criarAresta(g, no, ant, i, *indice);
            }
            break;
        case 3:
            down(no->est);
            if(!arestaExiste(g, i, no->est)){
                *indice = *indice + 1;
                no->mov = "d";
                no->cont = "u";
                criarAresta(g, no, ant, i, *indice);
            }
            break;
        case 4:
            pushLeft(no->est);
            if(!arestaExiste(g, i, no->est)){
                *indice = *indice + 1;
                no->mov = "L";
                no->cont = "R";
                criarAresta(g, no, ant, i, *indice);
            }
            break;
        case 5:
            pushRight(no->est);
            if(!arestaExiste(g, i, no->est)){
                *indice = *indice + 1;
                no->mov = "R";
                no->cont = "L";
                criarAresta(g, no, ant, i, *indice);
            }
            break;
        case 6:
            grab(no->est);
            if(!arestaExiste(g, i, no->est)){
                *indice = *indice + 1;
                no->mov = "g";
                no->cont = "";
                criarAresta(g, no, ant, i, *indice);
            }
            break;
        case 7:
            end(g, no, *indice);
            if(!arestaExiste(g, i, no->est)){
                *indice = *indice + 1;
                no->mov = "e";
                no->cont = "";
                criarAresta(g, no, ant, i, *indice);
            }
            break;
        default: printf("Variavel movi invlida no metodo mover()");
    }
}

// Esse método monta o grafo com todas as possibilidades de estado
VERTICE* construirGrafo(ESTADO* ini){
    
    int MAX;
    
    if(ini->exit_lin == 2)
        MAX = 25;
    else
        MAX = 27;
    
    VERTICE* g = (VERTICE*) malloc(sizeof(VERTICE) * (MAX + 1));
    
    // Incializacao:
    g[1].estado = ini;
    g[1].inicio = NULL;
    g[1].movimento = "";
    
    int j;
    for(j = 1; j <= MAX; j++){
        g[j].flag = 0;
        g[j].fim = false;
    }
    //Fim da inicializacao
    
    int indice = 1;
    int i;
    for(i = 1; i <= MAX; i++){
      
        if(movimentoPossivel(g[i].estado, 0)){
            NO* esq = (NO*) malloc(sizeof(NO));
            mover(g, i, &indice, esq, 0);
        }
        
        if(movimentoPossivel(g[i].estado, 1)){
            NO* dir = (NO*) malloc(sizeof(NO));
            mover(g, i, &indice, dir, 1);
        }
        
        if(movimentoPossivel(g[i].estado, 2)){
            NO* cima = (NO*) malloc(sizeof(NO));
            mover(g, i, &indice, cima, 2);
        }
        
        if(movimentoPossivel(g[i].estado, 3)){
            NO* baixo = (NO*) malloc(sizeof(NO));
            mover(g, i, &indice, baixo, 3);
        }
        
        if(movimentoPossivel(g[i].estado, 4)){
            NO* empEsq = (NO*) malloc(sizeof(NO));
            mover(g, i, &indice, empEsq, 4);
        }
        
        if(movimentoPossivel(g[i].estado, 5)){
            NO* empDir = (NO*) malloc(sizeof(NO));
            mover(g, i, &indice, empDir, 5);
        }
        
        if(movimentoPossivel(g[i].estado, 6)){
            NO* pegar = (NO*) malloc(sizeof(NO));
            mover(g, i, &indice, pegar, 6);
        }
        
        if(movimentoPossivel(g[i].estado, 7)){
            NO* fim = (NO*) malloc(sizeof(NO));
            mover(g, i, &indice, fim, 7);
        }
    }
    
    return g;
}

void buscaProfundaCaminho(VERTICE* g, int a, ACHOU* achou, char* caminho){
    if(achou->a == true) return;
    
    strcat(caminho, g[a].movimento);
    
    if(g[a].estado->bot_col == 0){
        achou->a = true;
    } 
    if(achou->a == true) return;
    
    g[a].flag = 1;
    NO* p = g[a].inicio;
    
    while(p){
        if(g[p->v].flag == 0){
            buscaProfundaCaminho(g, p->v, achou, caminho);
            if(achou->a == true) return;
            strcat(caminho, g[p->v].contrario);
        }
        p = p->prox;
    }
    
    g[a].flag = 2;
}

//------------------------------------------
// O EP consiste em implementar esta funcao
// e outras funcoes auxiliares que esta
// necessitar
//------------------------------------------
void caminhoValido(ESTADO* s, char* resp) {
    
    VERTICE* g = construirGrafo(s);
    
    ACHOU* achou = (ACHOU*) malloc(sizeof(ACHOU));
    achou->a = false;
    buscaProfundaCaminho(g, 1, achou, resp);
    
}

//APAGAR:
void exibir(VERTICE* g, int MAX){
    int i;
    for(i = 1; i <= MAX; i++){
        NO* p = g[i].inicio;
        
        printf("%d: ", i);
        
        while(p){
            printf("%d ", p->v);
            p = p->prox;
        }
        
        printf("\n");
    }
}


//---------------------------------------------------------
// use main() para fazer chamadas de teste ao seu programa
// mas nao entregue o codido de main() nem inclua nada
// abaixo deste ponto
//---------------------------------------------------------
int main() {
    
    // exemplo de teste do exemplo 1 da especificacao do EP
    char* resp = (char*) malloc(sizeof(char)*500);
    ESTADO* s = (ESTADO*) malloc(sizeof(ESTADO));
    
    s->bot_lin = 3;  
    s->bot_col = 3;
    s->exit_lin = 3; 
    s->exit_col = 3;
    s->box_col = 1;
    s->key_col = 2;
    s->haskey = false;
    caminhoValido(s, resp);
    
    return 0;
}