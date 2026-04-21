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

struct Heap {
	Masina* vector;
	int lungime;
	int nr_elemente_vizibile;
};
typedef struct Heap Heap;

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
	strlcpy(m1.model, aux, strlen(aux) + 1);

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

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nr_elemente_vizibile = 0;
	heap.vector = malloc(sizeof(Masina) * lungime);
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int pozitie_stanga = 2*pozitieNod + 1;
	int pozitie_dreapta = 2*pozitieNod + 2;
	int pozitie_max = pozitieNod;
	if (pozitie_dreapta < heap.nr_elemente_vizibile && heap.vector[pozitie_max].pret < heap.vector[pozitie_dreapta].pret) {
		pozitie_max = pozitie_dreapta;
	}
	else if (pozitie_stanga < heap.nr_elemente_vizibile && heap.vector[pozitie_max].pret < heap.vector[pozitie_stanga].pret) {
		pozitie_max = pozitie_stanga;
	}
	if (pozitie_max!=pozitieNod) {
		Masina aux = heap.vector[pozitie_max];
		heap.vector[pozitie_max] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
		if (pozitie_max <= (heap.nr_elemente_vizibile-2)/2) {
			filtreazaHeap(heap, pozitie_max);
		}
	}
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {

	FILE* f = fopen("../masini.txt", "r");
	Heap heap=initializareHeap(10);
	int contor = 0;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		heap.vector[contor] = m;
		contor++;
	}
	heap.nr_elemente_vizibile = contor;
	fclose(f);
	for (int i = (contor-2)/2; i>=0; i--) {
		filtreazaHeap(heap, i);
	}
	return heap;
}

void afisareHeap(Heap heap) {
	for (int i=0; i<heap.nr_elemente_vizibile; i++) {
		afisareMasina(heap.vector[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	for (int i=heap.nr_elemente_vizibile; i<heap.lungime; i++) {
		afisareMasina(heap.vector[i]);
	}
}

Masina extrageMasina(Heap* heap) {
	//extrage si returneaza masina de pe prima pozitie
	//elementul extras nu il stergem...doar il ascundem
	Masina m;
	m.id = -1;
	if (heap->nr_elemente_vizibile !=0) {
		m = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nr_elemente_vizibile-1];
		heap->vector[heap->nr_elemente_vizibile-1] = m;
		heap->nr_elemente_vizibile--;
		for (int i = (heap->nr_elemente_vizibile-2)/2; i>=0; i--) {
			filtreazaHeap(*heap, i);
		}
	}
	return m;
}


void dezalocareHeap(Heap* heap) {
	//sterge toate elementele din Heap
	for (int i = 0; i < heap->lungime; ++i) {
		free(heap->vector[i].model);
		free(heap->vector[i].numeSofer);
	}
	free(heap->vector);
	heap->nr_elemente_vizibile = 0;
	heap->lungime = 0;
	heap->vector = NULL;
}

int main() {
	Heap heap = citireHeapDeMasiniDinFisier("masini.txt");
	afisareHeap(heap);
	printf("Extrageri :\n");
	afisareMasina(extrageMasina(&heap));
	afisareMasina(extrageMasina(&heap));
	afisareMasina(extrageMasina(&heap));
	printf("Heap ascuns:\n");
	afiseazaHeapAscuns(heap);
	return 0;
}