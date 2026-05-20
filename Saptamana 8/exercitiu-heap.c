#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct Pacient {
	char* nume;
	char* prenume;
	int idPacient;
	int varsta;
	float gradUrgenta;
};

typedef struct Pacient Pacient;
typedef struct Heap Heap;

struct Heap {
	Pacient* vector;
	int lungime;
	int nrElViz;
};

Heap initializareHeap(int lungime)
{
	Heap heap;
	heap.lungime = lungime;
	heap.nrElViz = 0;
	heap.vector = malloc(sizeof(Pacient) * lungime);
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod)
{
	int pozitieStanga = 2 * pozitieNod + 1;
	int pozitieDreapta = 2 * pozitieNod + 2;
	int pozitieMax = pozitieNod;
	if (pozitieDreapta < heap.nrElViz && heap.vector[pozitieDreapta].gradUrgenta > heap.vector[pozitieNod].gradUrgenta)
	{
		pozitieMax = pozitieDreapta;
	}
	if (pozitieStanga < heap.nrElViz && heap.vector[pozitieStanga].gradUrgenta > heap.vector[pozitieMax].gradUrgenta)
	{
		pozitieMax = pozitieStanga;
	}
	if (pozitieMax != pozitieNod)
	{
		Pacient aux = heap.vector[pozitieMax];
		heap.vector[pozitieMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
		if (pozitieMax <= (heap.nrElViz - 2) / 2) // daca e parinte
			filtreazaHeap(heap, pozitieMax);
	}
}

void inserareHeap(Pacient pacient, Heap* heap)
{
	heap->vector[heap->nrElViz] = pacient;
	heap->nrElViz++;
	for (int i = (heap->nrElViz - 2) / 2; i >= 0; i--)
	{
			filtreazaHeap(*heap, i);
	}
}

Pacient citirePacientDinFisier(FILE* f)
{
	Pacient p;
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, f);
	char* aux = strtok(buffer, sep);
	p.nume = malloc(strlen(aux) + 1);
	strcpy_s(p.nume, strlen(aux) + 1, aux);
	aux = strtok(NULL, sep);
	p.prenume = malloc(strlen(aux) + 1);
	strcpy_s(p.prenume, strlen(aux) + 1, aux);
	p.idPacient = atoi(strtok(NULL, sep));
	p.varsta = atoi(strtok(NULL, sep));
	p.gradUrgenta = atof(strtok(NULL, sep));
	return p;
}

Heap citireHeapDinFisier(char* numeFisier)
{
	Heap heap = initializareHeap(6);
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f))
	{
		inserareHeap(citirePacientDinFisier(f), &heap);
	}
	fclose(f);
	return heap;
}

void afisarePacient(Pacient p)
{
	printf("ID: %d\n", p.idPacient);
	printf("Nume: %s\n", p.nume);
	printf("Grad Urgenta: %f\n\n", p.gradUrgenta);
}

void afisareHeap(Heap heap)
{
	for (int i = 0; i < heap.nrElViz; i++)
	{
		afisarePacient(heap.vector[i]);
	}
}

Pacient extrageDinHeap(Heap* heap)
{
	Pacient p;
	p.idPacient = -1;
	if (heap->nrElViz > 0)
	{
		Pacient temp = heap->vector[0];
		p.idPacient = heap->vector[0].idPacient;
		p.varsta = heap->vector[0].varsta;
		p.gradUrgenta = heap->vector[0].gradUrgenta;
		p.nume = malloc(strlen(heap->vector[0].nume) + 1);
		strcpy_s(p.nume, (strlen(heap->vector[0].nume) + 1), heap->vector[0].nume);
		p.prenume = malloc(strlen(heap->vector[0].prenume) + 1);
		strcpy_s(p.prenume, (strlen(heap->vector[0].prenume) + 1), heap->vector[0].prenume);
		heap->vector[0] = heap->vector[heap->nrElViz - 1];
		heap->vector[heap->nrElViz - 1] = temp;
		heap->nrElViz--;
		for (int i = 0; i < (heap->nrElViz - 2) / 2; i++)
		{
			filtreazaHeap(*heap, i);
		}
	}
	return p;
}

typedef struct NodLista NodLista;

struct NodLista {
	Pacient info;
	NodLista* next;
};

void adaugaPacientInLista(NodLista** lista, Pacient pacient)
{
	NodLista* nou = malloc(sizeof(NodLista));
	nou->info.idPacient = pacient.idPacient;	
	nou->info.varsta = pacient.varsta;
	nou->info.gradUrgenta = pacient.gradUrgenta;
	nou->info.nume = malloc(strlen(pacient.nume) + 1);
	strcpy_s(nou->info.nume, (strlen(pacient.nume) + 1), pacient.nume);
	nou->info.prenume = malloc(strlen(pacient.prenume) + 1);
	strcpy_s(nou->info.prenume, (strlen(pacient.prenume) + 1), pacient.prenume);
	nou->next = NULL;
	if (*lista)
	{
		NodLista* aux = (*lista);
		while (aux->next)
		{
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		(*lista) = nou;
	}
}

NodLista* salveazaInLista(Heap heap)
{
	NodLista* lista = NULL;
	for (int i = 0; i < heap.nrElViz; i++)
	{
		if (heap.vector[i].varsta <= 20)
			adaugaPacientInLista(&lista, heap.vector[i]);
	}
	return lista;
}

void afisareLista(NodLista* radacina)
{
	if (radacina)
	{
		NodLista* aux = radacina;
		while (aux)
		{
			afisarePacient(aux->info);
			aux = aux->next;
		}
	}
}

void dezalocare(Heap* heap, NodLista** lista, Pacient* p)
{
	for (int i = 0; i < heap->lungime; i++)
	{
		free(heap->vector[i].nume);
		free(heap->vector[i].prenume);
	}
	heap->nrElViz = 0;
	heap->lungime = 0;
	free(heap->vector);
	heap->vector = NULL;
	if (*lista)
	{
		NodLista* aux = (*lista);
		while (aux)
		{
			NodLista* deSters = aux;
			aux = aux->next;
			free(deSters->info.nume);
			free(deSters->info.prenume);
			free(deSters);
		}
		(*lista) = NULL;
	}
	free(p->nume);
	free(p->prenume);
}

int main()
{
	Heap heap = citireHeapDinFisier("pacienti.txt");
	afisareHeap(heap);

	Pacient p = extrageDinHeap(&heap);
	printf("Extragerea primului pacient din heap: \n");
	afisarePacient(p);

	NodLista* lista = salveazaInLista(heap);
	printf("Afisare lista cu pacienti avand varsta <= 20:\n");
	afisareLista(lista);

	dezalocare(&heap, &lista, &p);
	printf("Afisare heap dupa dezalocare: \n");
	afisareLista(lista);
	printf("Afisare lista dupa dezalocare: \n");
	afisareHeap(heap);
	printf("Afisare pacient extras dupa dezalocare: \n");
	afisarePacient(p);
}