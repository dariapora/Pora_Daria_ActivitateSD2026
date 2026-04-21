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
	Nod* urmator;
};

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
	strlcpy(m1.model, aux, strlen(aux) + 1); // folosesc strlcpy (clang)

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strlcpy(m1.numeSofer, aux, strlen(aux) + 1);

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
		primul = primul->urmator;
	}
}

void adaugaMasinaInLista(Nod** primul, Masina masinaNoua) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = masinaNoua;
	nod->urmator = NULL;
	if (*primul == NULL) *primul = nod;
	else {
		Nod* aux = *primul;
		while (aux->urmator) {
			aux=aux->urmator;
		}
		aux->urmator = nod;
	}
}

void adaugaLaInceputInLista(Nod** primul, Masina masinaNoua) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = masinaNoua;
	nod->urmator = (*primul);
	(*primul) = nod;
}

void* citireListaMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	if (file) {
		Nod* nod = NULL;
		while (!feof(file)) {
			adaugaMasinaInLista(&nod, citireMasinaDinFisier(file));
		}
		fclose(file);
		return nod;
	}
	return NULL;
}

void dezalocareListaMasini(Nod** primul) {
	Nod* aux = NULL;
	while ((*primul)) {
		aux = (*primul);
		(*primul) = (*primul)->urmator;
		if (aux->info.model) {
			free(aux->info.model);
		}
		if (aux->info.numeSofer) {
			free(aux->info.numeSofer);
		}
		free(aux);
	}
}

float calculeazaPretMediu(Nod* primul) {
	float pretTotal = 0;
	int nrMasini = 0;
	while (primul) {
		pretTotal += primul->info.pret;
		nrMasini++;
		primul = primul->urmator;
	}
	if (nrMasini) return pretTotal/nrMasini;
	return 0;
}

void stergeMasiniDinSeria(Nod** primul, char serieCautata) {
	Nod* curent = *primul;
	Nod* anterior = NULL;
	while (curent) {
		if (curent->info.serie == serieCautata) {
			Nod* aux = curent;
			if (anterior) {
				anterior->urmator = curent->urmator;
			}
			else {
				(*primul) = curent->urmator;
			}
			curent=curent->urmator;
			if (aux->info.model) {
				free(aux->info.model);
			}
			if (aux->info.numeSofer) {
				free(aux->info.numeSofer);
			}
			free(aux);
		}
		else {
			anterior=curent;
			curent=curent->urmator;
		}
	}

}

float calculeazaPretulMasinilorUnuiSofer(Nod* primul, const char* numeSofer) {
	float pretTotal = 0;
	while (primul) {
		if (strcmp(primul->info.numeSofer, numeSofer)==0) {
			pretTotal += primul->info.pret;
		}
		primul = primul->urmator;
	}
	return pretTotal;
}

int main() {
	Nod* primul = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(primul);
	//dezalocareListaMasini(&primul);
	//afisareListaMasini(primul);
	printf("Stergerea masinilor avand seria A:\n");
	stergeMasiniDinSeria(&primul, 'A');
	afisareListaMasini(primul);

	Masina masina;
	masina.id = 11;
	masina.model = "X6";
	masina.nrUsi = 4;
	masina.numeSofer = "Daria";
	masina.pret = 39000;
	masina.serie = 'A';

	printf("\nDupa adaugare la inceput:\n");
	adaugaLaInceputInLista(&primul, masina);
	afisareListaMasini(primul);

	printf("\nSuma pt Ionescu:\n");
	char nume[8] = "Ionescu";
	printf("%5.2f", calculeazaPretulMasinilorUnuiSofer(primul, nume));
	return 0;
}