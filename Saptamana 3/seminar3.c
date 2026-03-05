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

void afisareMasina(Masina masina) {
	printf("Masina id: %d, numar usi: %d, pret: %5.2f, model: %s, nume sofer: %s, serie: %c\n\n", masina.id, masina.nrUsi, masina.pret, masina.model, masina.numeSofer, masina.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	for (int i = 0; i < nrMasini; i++)
	{
		afisareMasina(masini[i]);
	}
}

void adaugaMasinaInVector(Masina** masini, int * nrMasini, Masina masinaNoua) {
	Masina* vectorNou = (Masina*)malloc(sizeof(Masina) * ((*nrMasini) + 1));
	for (int i = 0; i < (*nrMasini); i++)
	{
		vectorNou[i] = (*masini)[i];
	}
	vectorNou[*nrMasini] = masinaNoua;
	free(*masini);
	*masini = vectorNou;
	(*nrMasini)++;
}

Masina citireMasinaFisier(FILE* file) {
	Masina m;
	char buffer[50];
	fgets(buffer, 49, file);
	char var[3]=",\n";
	m.id = atoi(strtok(buffer, var));
	m.nrUsi = atoi(strtok(NULL, var));
	m.pret = atof(strtok(NULL, var));
	char* aux = strtok(NULL, var);
	m.model = (char*)malloc(strlen(aux) + 1);
	strcpy(m.model, aux);
	aux = strtok(NULL, var);
	m.numeSofer = (char*)malloc(strlen(aux) + 1);
	strcpy(m.numeSofer, aux);
	m.serie = strtok(NULL, var)[0];
	return m;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	FILE *f = fopen(numeFisier, "r");
	Masina* masini = NULL;
	(*nrMasiniCitite) = 0;
	while (!feof(f)) {
		adaugaMasinaInVector(&masini, nrMasiniCitite, citireMasinaFisier(f));
	}
	fclose(f);
	return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	//este dezalocat intreg vectorul de masini
	for (int i = 0; i< (*nrMasini); i++) {
		free((*vector)[i].model);
		free((*vector)[i].numeSofer);
	}
	free(*vector);
	*vector = NULL;
	*nrMasini = 0;
}

int main()
{
	int nrMasini = 0;
	Masina* masini = NULL;
	masini = citireVectorMasiniFisier("masini.txt", &nrMasini);
	afisareVectorMasini(masini, nrMasini);
	return 0;
}