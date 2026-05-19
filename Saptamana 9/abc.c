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

//creare structura pentru un nod dintr-un arbore binar de cautare
struct Nod {
	Masina info;
	Nod* dreapta;
	Nod* stanga;
};

typedef struct Nod Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
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


void adaugaMasinaInArbore(Nod** radacina, Masina masinaNoua) {
	//adauga o noua masina pe care o primim ca parametru in arbore,
	//astfel incat sa respecte princiippile de arbore binar de cautare
	//dupa o anumita cheie pe care o decideti - poate fi ID
	if (*radacina) {
		if ((*radacina)->info.id < masinaNoua.id) {
			adaugaMasinaInArbore((*radacina)->dreapta, masinaNoua);
		}
		else if ((*radacina)->info.id < masinaNoua.id)
			adaugaMasinaInArbore((*radacina)->stanga, masinaNoua);
	}
	else {
		Nod* nou = malloc(sizeof(Nod));
		nou->info = masinaNoua;
		nou->stanga = NULL;
		nou->dreapta = NULL;
		(*radacina) = nou;
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	Nod* radacina = NULL;
	while (!feof(f)) {
		adaugaMasinaInArbore(&radacina, citireMasinaDinFisier(f));
	}
	fclose(f);
	return radacina;
}

void afisareMasiniDinArbore(Nod* radacina) {
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere
	if (radacina) {
		afisareMasina(radacina->info);
		afisareMasiniDinArbore(radacina->stanga);
		afisareMasiniDinArbore(radacina->dreapta);
	}
}

void dezalocareArboreDeMasini(/*arbore de masini*/) {
	//sunt dezalocate toate masinile si arborele de elemente
}

Masina getMasinaByID(Nod* root, int id) {
	Masina m;
	m.id = -1;
	if (root) {
		if (root->info.id == id) {
			m = root->info;
			m.model = (char*)malloc(strlen(root->info.model) + 1);
			strcpy_s(m.model, strlen(root->info.model) + 1, root->info.model);
			m.numeSofer = (char*)malloc(strlen(root->info.numeSofer) + 1);
			strcpy_s(m.numeSofer, strlen(root->info.numeSofer) + 1, root->info.numeSofer);
		}
		else if (root->info.id > id) {
			return getMasinaByID(root->stanga, id);
		}
		else {
			return getMasinaByID(root->dreapta, id);
		}
	}
	return m;
}

int determinaNumarNoduri(Nod* radacina) {
	if (radacina) {
		int numarDreapta = determinaNumarNoduri(radacina->dreapta);
		int numarStanga = determinaNumarNoduri(radacina->stanga);
		return numarDreapta+numarStanga+1;
	}
	return 0;
}

int calculeazaInaltimeArbore(Nod* radacina) {
	//calculeaza inaltimea arborelui care este data de
	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza
	if (radacina) {
		int lungimeDreapta = calculeazaInaltimeArbore(radacina->dreapta);
		int lungimeStanga = calculeazaInaltimeArbore(radacina->stanga);
		if (lungimeDreapta>lungimeStanga) return lungimeDreapta+1;
		return lungimeStanga+1;
	}
	return 0;
}

float calculeazaPretTotal(Nod* radacina) {
	if (radacina) {
		float ps= calculeazaPretTotal(radacina->stanga);
		float pd = calculeazaPretTotal(radacina->dreapta);
		return radacina->info.pret + pd + ps;
	}
	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* radacina, const char* numeSofer) {
	if (radacina) {
		float ps = calculeazaPretulMasinilorUnuiSofer(radacina->stanga, numeSofer);
		float pd = calculeazaPretulMasinilorUnuiSofer(radacina->dreapta, numeSofer);
		if (strcmp(radacina->info.numeSofer, numeSofer) == 0) {
			
			return radacina->info.pret + pd + ps;
		}
		return pd + ps;
	}
	return 0;
}

int main() {
	return 0;
}