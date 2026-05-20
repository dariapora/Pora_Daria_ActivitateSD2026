#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Comanda {
	int id;
	char* produs;
	char* client;
	int durata;
	int pret;
};

typedef struct Comanda Comanda;
typedef struct Nod Nod;

struct Nod {
	Comanda info;
	Nod* dreapta;
	Nod* stanga;
};

Comanda citireComandaDinFisier(FILE* f)
{
	Comanda c;
	char buffer[190];
	char sep[4] = ",\n\r";
	fgets(buffer, 190, f);
	c.id = atoi(strtok(buffer, sep));
	char* aux = strtok(NULL, sep);
	c.produs = malloc(strlen(aux) + 1);
	strcpy_s(c.produs, strlen(aux) + 1, aux);
	aux = strtok(NULL, sep);
	c.client = malloc(strlen(aux) + 1);
	strcpy_s(c.client, strlen(aux) + 1, aux);
	c.durata = atoi(strtok(NULL, sep));
	c.pret = atoi(strtok(NULL, sep));
	return c;
}

int calculeazaInaltime(Nod* radacina)
{
	if (radacina)
	{
		int ls = calculeazaInaltime(radacina->stanga);
		int ld = calculeazaInaltime(radacina->dreapta);
		if (ls > ld) return 1 + ls;
		return 1 + ld;
	}
	return 0;
}

int gradEchilibru(Nod* radacina)
{
	return calculeazaInaltime(radacina->stanga) - calculeazaInaltime(radacina->dreapta);
}

void rotireStanga(Nod** radacina)
{
	Nod* aux = (*radacina)->dreapta;
	(*radacina)->dreapta = aux->stanga;
	aux->stanga = (*radacina);
	(*radacina) = aux;
}

void rotireDreapta(Nod** radacina)
{
	Nod* aux = (*radacina)->stanga;
	(*radacina)->stanga = aux->dreapta;
	aux->dreapta = (*radacina);
	(*radacina) = aux;
}

void adaugaComandaInArbore(Nod** radacina, Comanda c)
{
	if (*radacina)
	{
		if ((*radacina)->info.id > c.id)
		{
			adaugaComandaInArbore(&(*radacina)->stanga, c);
		}
		if ((*radacina)->info.id < c.id)
		{
			adaugaComandaInArbore(&(*radacina)->dreapta, c);
		}
		int ge = gradEchilibru(*radacina);
		if (ge == 2)
		{
			if (gradEchilibru((*radacina)->stanga) == -1)
				rotireStanga(&(*radacina)->stanga);
			rotireDreapta(radacina);
		}
		else if (ge == -2)
		{
			if(gradEchilibru((*radacina)->dreapta) == 1)
				rotireDreapta(&(*radacina)->dreapta);
			rotireStanga(radacina);
		}
	}
	else
	{
		Nod* nou = malloc(sizeof(Nod));
		nou->dreapta = NULL;
		nou->stanga = NULL;
		nou->info = c;
		(*radacina) = nou;
	}
}

Nod* citireArboreDinFisier(char* numeFisier)
{
	Nod* nou = NULL;
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f))
	{
		adaugaComandaInArbore(&nou, citireComandaDinFisier(f));
	}
	fclose(f);
	return nou;
}

void afisareComanda(Comanda c)
{
	printf("ID: %d\n", c.id);
	printf("Produs: %s\n", c.produs);
	printf("Client: %s\n", c.client);
	printf("Durata: %d\n", c.durata);
	printf("Pret: %d\n\n", c.pret);
}

void afisareArbore(Nod* radacina) {
	if (radacina)
	{
		afisareArbore(radacina->stanga);
		afisareComanda(radacina->info);
		afisareArbore(radacina->dreapta);
	}
}

Comanda getComandaById(int id, Nod* radacina)
{
	Comanda c;
	if (radacina)
	{
		if (id < radacina->info.id)
		{
			return getComandaById(id, radacina->stanga);
		}
		else if (id > radacina->info.id)
		{
			return getComandaById(id, radacina->dreapta);
		}
		else {
			c = radacina->info;
			c.produs = malloc(strlen(radacina->info.produs)+1);
			strcpy_s(c.produs, strlen(radacina->info.produs)+1, radacina->info.produs);
			c.client = malloc(strlen(radacina->info.client)+1);
			strcpy_s(c.client, strlen(radacina->info.client)+1, radacina->info.client);
			return c;
		}
	}
	c.id = -1;
	return c;
}

typedef struct NodLista NodLista;

struct NodLista {
	Comanda info;
	NodLista* next;
};

void adaugareComandaLista(Comanda c, NodLista** lista)
{
	NodLista* nou = malloc(sizeof(NodLista));
	nou->info = c;
	nou->next = NULL;
	if ((*lista))
	{
		NodLista* aux = (*lista);
		while (aux->next)
		{
			aux = aux->next;
		}
		aux->next = nou;
		nou->info.produs = malloc(strlen(c.produs) + 1);
		strcpy_s(nou->info.produs, strlen(c.produs) + 1, c.produs);
		nou->info.client = malloc(strlen(c.client) + 1);
		strcpy_s(nou->info.client, strlen(c.client) + 1, c.client);
	}
	else {
		(*lista) = nou;
	}
}

void adaugareLista(Nod* radacina, NodLista** lista)
{
	if (radacina)
	{
		adaugareLista(radacina->stanga, lista);
		adaugareComandaLista(radacina->info, lista);
		adaugareLista(radacina->dreapta, lista);
	}
}

void afisareLista(NodLista* lista) {
	if (lista)
	{
		NodLista* aux = lista;
		while (aux)
		{
			afisareComanda(aux->info);
			aux = aux->next;
		}
	}
}

int main()
{
	Nod* radacina = citireArboreDinFisier("comenzi.txt");
	afisareArbore(radacina);
	afisareComanda(getComandaById(1, radacina));
	NodLista* listaComenzi = NULL;
	adaugareLista(radacina, &listaComenzi);
	afisareLista(listaComenzi);
	return 0;
}