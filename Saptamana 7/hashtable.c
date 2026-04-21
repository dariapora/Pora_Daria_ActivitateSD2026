#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

typedef struct Nod Nod;
struct Nod {
	Masina info;
	Nod* next;
};

struct HashTable {
	int dim;
	Nod** vector;
};
typedef struct HashTable HashTable;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Nod* primul) {
	while (primul) {
		afisareMasina(primul->info);
		primul = primul->next;

	}
}

void adaugaMasinaInLista(Nod** lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	if (*lista) {
		Nod* aux = (*lista);
		while (aux->next != NULL) {
			aux = aux->next;

		}
		aux->next = nou;
	}
	else {
		(*lista) = nou;
	}
}


HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = (Nod**)malloc(dimensiune * sizeof(Nod*));
	for (int i = 0; i < dimensiune; i++)
	{
		ht.vector[i] = NULL;
	}
	return ht;
}

int calculeazaHash(int id, int dimensiune) {
	id = id * 23 + 123;
	return id % dimensiune;
}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
	//este folosit mecanismul CHAINING
	//este determinata pozitia si se realizeaza inserarea pe pozitia respectiva
	int poz = calculeazaHash(masina.id, hash.dim);
	if (hash.vector[poz]) {
		//avem coliziune
	}
	else {
		//nu avem coliziune
	}
	adaugaMasinaInLista(&(hash.vector[poz]), masina);

}

HashTable citireMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	HashTable ht = initializareHashTable(3);

	if (!f) {
		printf("Nu exista\n");
		return ht;
	}
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		inserareMasinaInTabela(ht, m);

	}
	fclose(f);
	return ht;

}

void afisareTabelaDeMasini(HashTable ht) {
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL) {
			printf("Cluster-ul: %d\n", i + 1);
			afisareListaMasini(ht.vector[i]);
			printf("=========================\n");
		}
	}
}

void dezalocareTabelaDeMasini(HashTable *ht) {
	for (int i = 0; i < (*ht).dim; i++)
	{
		if ((*ht).vector[i] != NULL) {
			Nod* cap = (*ht).vector[i];
			while (cap) {
						Nod* p = cap;
						(cap) = p->next;
						if (p->info.numeSofer) {
							free(p->info.numeSofer);
						}
						if (p->info.model) {
							free(p->info.model);
						}
						free(p);

					}

		}
	}
	(*ht).dim = 0;
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	*nrClustere = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL) {
			(*nrClustere)++;
		}
	}
	float* pretMediu = (float*)malloc(sizeof(float) * (*nrClustere));
	int k = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL) {
			float suma = 0;
			float numar = 0;
			Nod* aux = ht.vector[i];

			while (aux) {
				suma += aux->info.pret;
				numar++;
				aux = aux->next;

			}
			pretMediu[k++] = suma / numar;
		}
	}

	return pretMediu;
}

Masina getMasinaDupaCheie(HashTable ht, int id) {
	Masina m;
	m.id = -1;
	int hash = calculeazaHash(id, ht.dim);
	if (hash >= 0 && hash <= ht.dim) {
		Nod* aux = ht.vector[hash];
		while (aux) {
			if (aux->info.id == id) {
				m = aux->info;
				m.model = (char*)malloc((strlen(aux->info.model)+1) * sizeof(char));
				strcpy(m.model, aux->info.model);
				m.numeSofer = (char*)malloc((strlen(aux->info.numeSofer)+1) * sizeof(char));
				strcpy(m.numeSofer, aux->info.numeSofer);
				break;

			}
			aux = aux->next;
		}
	}

	return m;
}

int main() {

	HashTable ht =  citireMasiniDinFisier("masini.txt");
	afisareTabelaDeMasini(ht);
	//dezalocareTabelaDeMasini(&ht);
	//afisareTabelaDeMasini(ht);
	Masina m = getMasinaDupaCheie(ht, 5);
	afisareMasina(m);
	int nrClustere;
	float* vectorpreturi = calculeazaPreturiMediiPerClustere(ht, &nrClustere);
	for (int  i = 0; i < nrClustere; i++)
	{
		printf("Pentru clusterul cu indexul %d, pretul mediu este: %f\n", i, vectorpreturi[i]);
	}

	return 0;
}