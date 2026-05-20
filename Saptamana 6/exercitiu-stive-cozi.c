#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Comanda {
    int cod;
    char* titlu;
    char* autor;
    char* gen;
    float pret;
};

typedef struct Comanda Carte;
typedef struct Nod Nod;
typedef struct Queue Queue;

struct Nod {
    Nod* next;
    Nod* prev;
    Carte info;
};

struct Queue {
    Nod* prim;
    Nod* last;
};

Queue initializareCoada() {
    Queue coada;
    coada.prim = NULL;
    coada.last = NULL;
    return coada;
}

void afisareCarte(Carte c) {
    printf("Cod: %d\nTitlu: %s\nAutor: %s\nGen: %s\nPret: %f\n\n", c.cod, c.titlu, c.autor, c.gen, c.pret);
}

void afisareCoada(Queue coada) {
    if (coada.prim!=NULL) {
        Nod* aux = coada.prim;
        while (aux) {
            afisareCarte(aux->info);
            aux=aux->next;
        }
    }
}

void inserareCoada(Queue* coada, Carte carte){
    Nod* nou = malloc(sizeof(Nod));
    nou->info = carte;
    nou->prev = NULL;
    nou->next = NULL;
    if (coada->last!=NULL) {
        nou->prev = coada->last;
        coada->last->next = nou;
        coada->last = nou;
    }
    else {
      coada->prim = nou;
      coada->last = nou;
    }
}

Carte extragereCarte(Queue* queue, int cod) {
    Carte c;
    c.cod = -1;
    if (queue->prim!=NULL) {
        Nod* aux = queue->prim;
        while (aux) {
            if (aux->info.cod == cod) {
                c = aux->info;
                Nod* deSters = aux;
                if (deSters->prev) {
                    deSters->prev->next = deSters->next;
                }
                else {
                    queue->prim = deSters->next;
                }
                if (deSters->next)
                    deSters->next->prev = deSters->prev;
                else {
                    queue->last = deSters -> prev;
                }
                aux = aux->next;
                free(deSters);
            }
            else aux=aux->next;
        }
    }
    return c;
}

Carte* cartiHorror(Queue coada) {
    int contor = 0;
    if (coada.prim!=NULL) {
        Nod* aux = coada.prim;
        while (aux) {
            if (strcmp(aux->info.gen, "Horror") == 0) {
                contor++;
            }
            aux=aux->next;
        }
        Carte* cartiHorror = malloc(sizeof(Carte) * contor);
        int k = 0;
        aux = coada.prim;
        while (aux) {
            if (strcmp(aux->info.gen, "Horror") == 0) {
                cartiHorror[k++] = aux->info;
            }
            aux=aux->next;
        }
        printf("Carti din genul Horror:\n");
        for (int i=0; i<contor; i++) {
            afisareCarte(cartiHorror[i]);
        }
        return cartiHorror;
    }
    printf("Coada este goala.\n");
    return NULL;
}

int main() {
    Queue coada = initializareCoada();
    Carte c1 = {1, "Viata e frumoasa", "Mircea Gunes", "Poezie", 30.4};
    Carte c2 = {2, "Biologie", "Maria Hublov", "Stiinte Exacte", 12};
    Carte c3 = {3, "IT", "Stephen King", "Horror", 33};
    Carte c4 = {4, "Horror Story", "Mike Bike", "Horror", 27};

    inserareCoada(&coada, c1);
    inserareCoada(&coada, c2);
    inserareCoada(&coada, c3);
    inserareCoada(&coada, c4);

    printf("Inainte de extragere:\n");
    afisareCoada(coada);
    printf("\n");
    Carte c = extragereCarte(&coada, 1);
    printf("Carte extrasa:\n");
    afisareCarte(c);
    printf("\n");
    printf("Dupa extragere:\n");
    afisareCoada(coada);
    Carte* horror = cartiHorror(coada);
    free(horror);
}
