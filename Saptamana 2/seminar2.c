#include<stdio.h>
#include<stdlib.h>
#include <string.h>

// [] indexare = deplasare + dereferentiere
// -> = dereferentiere + accesare

struct Biblioteca {
	int id;
	int nrCarti;
	char* nume;
	float suprafata;
	char sector;
};
struct Biblioteca initializare(int id, int nrCarti, char* nume, float suprafata, char sector) {
	struct Biblioteca s;
	s.id = id;
	s.nrCarti = nrCarti;
	s.nume = (char*)malloc(sizeof(char)*(strlen(nume) + 1));
	strcpy(s.nume, nume);
	s.suprafata = suprafata;
	s.sector = sector;
	return s;
}

struct Biblioteca copiazaBiblioteca(struct Biblioteca sursa) {
	return initializare(sursa.id, sursa.nrCarti, sursa.nume, sursa.suprafata, sursa.sector);
}

void afisare(struct Biblioteca s) {
	printf("ID-ul: %d\t\n Numarul de carti: %d\n Denumire: %s\n Suprafata: %5.2f\n Sector: %d\n", s.id, s.nrCarti,s.nume, s.suprafata, s.sector);
}

void afisareVector(struct Biblioteca* vector, int nrElemente) {
	for (int i=0; i<nrElemente; i++) {
		afisare(vector[i]);
	}
}

struct Biblioteca* copiazaPrimeleNElemente(struct Biblioteca* vector, int nrElemente, int nrElementeCopiate) {
	struct Biblioteca *vectorNou=NULL;
	if (nrElementeCopiate < nrElemente) {
		nrElementeCopiate = nrElemente;
	}

	vectorNou = (struct Biblioteca*)malloc(sizeof(struct Biblioteca)*nrElementeCopiate);
	for (int i = 0; i< nrElementeCopiate; i++) {
		vectorNou[i] = copiazaBiblioteca(vector[i]);
	}
	return vectorNou;
}

void dezalocare(struct Biblioteca** vector, int* nrElemente) {
	for (int i = 0; i < *nrElemente; ++i) {
		free((*vector)[i].nume);
	}
	free((*vector));
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaBibliotecaCuSuprafataMare(struct Biblioteca* vector, char nrElemente, float supMin, struct Biblioteca** vectorNou, int* dimensiune) {
	*dimensiune=0;
	for (int i = 0; i < nrElemente; ++i) {
		if (vector->suprafata>supMin) {
			(*dimensiune)++;
		}
	}
	*vectorNou = (struct Biblioteca*)malloc(sizeof( struct Biblioteca)* (*dimensiune));
	int contor=0;
	for (int i = 0; i < nrElemente; ++i) {
		if (vector->suprafata>supMin) {
			(*vectorNou)[contor++] = copiazaBiblioteca(vector[i]);
		}
	}
}

struct Biblioteca getPrimulElementConditionat(struct Biblioteca* vector, int nrElemente, const char* denumire) {
	struct Biblioteca s;
	for(int i=0; i<nrElemente; i++) {
		if (strcmp(vector[i].nume, denumire)==0) {
			s = copiazaBiblioteca(vector[i]);
			return s;
		}
	}
	return initializare(0, 0, "N/A", 0, 0);
}

int main() {
	struct Biblioteca b;
	b = initializare(1, 431, "Biblioteca Mea", 482.95, 5);

	int numarBiblioteci = 5;
	struct Biblioteca* vectorBiblioteci = (struct Biblioteca*)malloc(sizeof(struct Biblioteca)*numarBiblioteci);

	vectorBiblioteci[0] = b; // shallow copy
	vectorBiblioteci[1] = initializare(2, 6, "Apropo", 13, 1);
	vectorBiblioteci[2] = initializare(3, 127, "Creanga", 938, 2);
	vectorBiblioteci[3] = initializare(4, 921, "Slavici", 210, 5);
	vectorBiblioteci[4] = initializare(5, 73, "Mihai Viteazu", 97, 2);

	afisareVector(vectorBiblioteci, numarBiblioteci);
	int nrCopiate = 3;
	struct Biblioteca* copie= copiazaPrimeleNElemente(vectorBiblioteci, numarBiblioteci, nrCopiate);
	printf("\nElementele vectorului cu primele %d elemente copiate: \n", nrCopiate);
	afisareVector(copie, nrCopiate);

	dezalocare(&copie,&nrCopiate);

	struct Biblioteca* vBiblioteciCuSuprafataMare = NULL;
	int nrBibliotecaCuSuprafataMare = 0;
	copiazaBibliotecaCuSuprafataMare(vectorBiblioteci, numarBiblioteci, 100,&vBiblioteciCuSuprafataMare,&nrBibliotecaCuSuprafataMare);

	printf("\nElementele vectorului cu biblioteci mari: \n");
	afisareVector(vBiblioteciCuSuprafataMare,nrBibliotecaCuSuprafataMare);
	dezalocare(&vBiblioteciCuSuprafataMare,&nrBibliotecaCuSuprafataMare);

	printf("\nPrimul element care indeplineste conditia: \n");
	afisare(getPrimulElementConditionat(vectorBiblioteci, numarBiblioteci, "Slavici"));

	return 0;
}