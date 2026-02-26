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
	//copiem intr-un vector nou pe care il vom returna primele nrElementeCopiate
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
	//dezalocam elementele din vector si vectorul
	for (int i = 0; i < *nrElemente; ++i) {
		free((*vector)[i].nume);


	}
	free((*vector));
	*vector = NULL;
	nrElemente = 0;


}

void copiazaBibcuSupMare(struct Biblioteca* vector, char nrElemente, float supMin, struct Biblioteca** vectorNou, int* dimensiune) {
	//parametrul prag poate fi modificat in functie de
	// tipul atributului ales pentru a indeplini o conditie
	//este creat un nou vector cu elementele care indeplinesc acea conditie

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

struct Biblioteca getPrimulElementConditionat(struct Biblioteca* vector, int nrElemente, const char* conditie) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.

	//DE CAUTAT BIBLIOTECA CU UN ANUMIT NUME CU STRCPMP.
	//ESTE O FUNCTIE CARE PRIMESTE DOI PARAMETRII DE TIP CHAR*, SI FACE DIFERENTA INTRE CODURILE ASCII SI RETURNEAZA O VALOARE, 0 DACA SUNT LA FEL


	struct Biblioteca s;
	s.id = 1;

	return s;
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
	printf("\nElementele vect cu primele %d elemente copiate", nrCopiate);
	afisareVector(copie, nrCopiate);

	dezalocare(&copie,&nrCopiate);

	struct Biblioteca* vBibcuSupMAre = NULL;
	int nrBibCuSupM = 0;
	copiazaBibcuSupMare(vectorBiblioteci, numarBiblioteci, 100,&vBibcuSupMAre,&nrBibCuSupM);
	afisareVector(vBibcuSupMAre,nrBibCuSupM);

	dezalocare(&vBibcuSupMAre,&nrBibCuSupM);

	return 0;
}