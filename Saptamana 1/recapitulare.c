#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct Produs {
	int id;
	int stoc;
	char* denumire;
	float pret;
	char categorie;
};

struct Produs initializare(int id, int stoc, char* denumire, float pret, char categorie) {
	struct Produs p;
	p.id = id;
	p.stoc = stoc;
	p.pret = pret;
	p.denumire = (char*)malloc(sizeof(char)*(strlen(denumire)+1));
	strcpy(p.denumire, denumire);
	p.categorie = categorie;
	return p;
}

void afisare(struct Produs p) {
	printf("ID: %d\n", p.id);
	printf("Stoc: %d\n", p.stoc);
	printf("Pret: %5.2f\n", p.pret);
	printf("Denumire: %s\n", p.denumire);
	printf("Categorie: %c\n", p.categorie);
}

void modificaDenumire(struct Produs *p, char* nouaDenumire) {
	free(p->denumire);
	p->denumire = (char*)malloc(sizeof(char)*(strlen(nouaDenumire)+1));
	strcpy(p->denumire, nouaDenumire);
}

void dezalocare(struct Produs *p) {
	if (p->denumire != NULL) {
		free(p->denumire);
		p->denumire=NULL;
	}
}

float calculeazaValoare(struct Produs p) {
	return p.stoc * p.pret;
}

int main() {
	struct Produs p;
	p = initializare(1, 20, "Telefon", 2000, 'A');
	afisare(p);
	modificaDenumire(&p, "Mouse");
	afisare(p);
	printf("Valoarea totala pentru produsul %s este: %5.2f.\n", p.denumire, calculeazaValoare(p));
	struct Produs* ps = &p; // alocat pe stiva main
	struct Produs* pp = malloc(sizeof(struct Produs)); // alocat pe heap, trebuie dezalocat
	dezalocare(&p);
	return 0;
}