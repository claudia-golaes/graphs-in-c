#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cerinta1.h"
#define INT_MAX 9999


//functie pentru adaugarea unui nou nod in lista de adiacenta
struct AdjListNode *newAdjListNode(int dest, int cost)
{
    struct AdjListNode *newNode = (struct AdjListNode *)malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->cost = cost;
    newNode->next = NULL;
    return newNode;
}

//functia pentru crearea grafului si initializarea acestuia
struct Graph *createGraph(int V)
{
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->V = V;

    graph->array = (struct AdjList *)malloc(V * sizeof(struct AdjList));

    graph->zoneNames = (char **)malloc(V * sizeof(char *));

    int i;
    for (i = 0; i < V; ++i)
    {
        graph->array[i].head = NULL;
        graph->zoneNames[i] = NULL;
    }

    return graph;
}

//functie pentru adaugarea unei muchii intre doua noduri (de doua ori, fiind un
//graf neorientat)
void addEdge(struct Graph *graph, int src, int dest, int cost)
{
    struct AdjListNode *newNode = newAdjListNode(dest, cost);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    newNode = newAdjListNode(src, cost);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

//functie de DFS modificata astfel incat vectorul meu visited sa nu aiba numai 
//valori egale cu 1, ci fiecare vector[nod]=x, oricare ar fi nod apartinand de 
//componenta conexa x (insula)
void DFS1(struct Graph *graph, int v, int *island, int *visited, int *numIslands)
{
    visited[v] = *island;

    struct AdjListNode *pCrawl = graph->array[v].head;
    while (pCrawl)
    {
        int dest = pCrawl->dest;
        if (!visited[dest])
        {
            DFS1(graph, dest, island, visited, numIslands);
        }
        pCrawl = pCrawl->next;
    }
}

//functie care apeleaza DFS1 si retine si numarul de componente conexe(insule)
void DFS2(struct Graph *graph, int num, int *visited, int *numIslands)
{
    int island = 1;

    for (int v = 0; v < num; v++)
    {
        if (!visited[v])
        {
            DFS1(graph, v, &island, visited, numIslands);
            (*numIslands)++;
            island++;
        }
    }
}

//functie care gaseste indexul unui nod intr-un graf, o folosesc pentru a adauga
//muchii in componentele conexe din functia createIslands
int findIndex(struct Graph *graph, const char *node)
{
    int i;
    for (i = 0; i < graph->V; i++)
    {
        if (strcmp(graph->zoneNames[i], node) == 0)
            return i;
    }
    return -1;
}

//algoritmul lui Kruskal
int getMinimumTotalCost(struct Graph *graph)
{
    //initializarea necesara pentru algoritmul lui Kruskal: costul total, nodul 
    //de start, vectorul pentru nodurile vizitate si numarul de muchii
    int totalCost = 0;
    int startNode = 0;
    int *visited = (int *)calloc(graph->V, sizeof(int));
    visited[startNode] = 1;
    int numEdges = 0;

    while (numEdges < graph->V - 1)
    {
        int minCost = INT_MAX;
        int minSrc, minDest;
        for (int i = 0; i < graph->V; i++)
        {
            if (visited[i])
            {
                struct AdjListNode *pCrawl = graph->array[i].head;
                while (pCrawl)
                {
                    if (!visited[pCrawl->dest] && pCrawl->cost < minCost)
                    {
                        minCost = pCrawl->cost;
                        minSrc = i;
                        minDest = pCrawl->dest;
                    }
                    pCrawl = pCrawl->next;
                }
            }
        }

        // adaugam muchia selectata in arborele de acoperire minim
        totalCost += minCost;
        visited[minDest] = 1;
        numEdges++;
    }

    free(visited);

    return totalCost;
}

//fucntie care foloseste algoritmul lui Kruskal definit mai sus pe fiecare 
//componenta conexa si salveaza in vectorul minCost costul minim al fiecarei
//insule
void createIslands(struct Graph *graph, int *visited, int *minCost)
{
    int i;
    int index = 0;
    int prevIndex = -1; //valoare initiala care nu poate fi atinsa în mod normal
    while (index < graph->V && index != prevIndex)
    {
        prevIndex = index;
        int val = index;
        int ok = 1;
        int currentIsland = visited[index];
        for (i = val; i < graph->V && ok == 1; i++)
        {
            if (visited[i] == currentIsland)
            {
                index++;
            }
            else
            {
                ok = 0;
            }
        }

        struct Graph *islandTemp = createGraph(index - val);
        int j;
        for (j = val; j < index; j++)
        {
            islandTemp->zoneNames[j - val] = strdup(graph->zoneNames[j]);
        }

        for (j = val; j < index; j++)
        {
            struct AdjListNode *pCrawl = graph->array[j].head;
            while (pCrawl)
            {
                int srcIndex = findIndex(islandTemp, graph->zoneNames[j]);
                int destIndex = findIndex(islandTemp, graph->zoneNames[pCrawl->dest]);
                addEdge(islandTemp, srcIndex, destIndex, pCrawl->cost);
                pCrawl = pCrawl->next;
            }
        }
        minCost[currentIsland - 1] = getMinimumTotalCost(islandTemp);
        for (j = 0; j < islandTemp->V; j++)
        {
            free(islandTemp->zoneNames[j]);
            struct AdjListNode *current = islandTemp->array[j].head;
            while (current)
            {
                struct AdjListNode *temp = current;
                current = current->next;
                free(temp);
            }
        }
        free(islandTemp->zoneNames);
        free(islandTemp->array);
        free(islandTemp);
    }
}

//functie pentru sortarea crescatoare a vectorului minCost (conform cerintei)
void bubbleSort(int minCost[], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (minCost[j] > minCost[j + 1])
            {
                int temp = minCost[j];
                minCost[j] = minCost[j + 1];
                minCost[j + 1] = temp;
            }
        }
    }
}


void main_cerinta1()
{
    //citire din fisier
    FILE *file = fopen("tema3.in", "r");
    FILE *out = fopen("tema3.out", "w");
    int n, m;
    fscanf(file, "%d %d", &n, &m);
    struct Graph *graph = createGraph(n);

    int i, j;
    int objIndex = 0;
    for (i = 0; i < m; i++)
    {
        char src[100], dest[100];
        int cost;
        fscanf(file, "%s %s %d", src, dest, &cost);

        int srcIndex = -1, destIndex = -1;
        for (j = 0; j < objIndex; j++)
        {
            if (strcmp(src, graph->zoneNames[j]) == 0)
                srcIndex = j;
            if (strcmp(dest, graph->zoneNames[j]) == 0)
                destIndex = j;
        }

        if (srcIndex == -1)
        {
            graph->zoneNames[objIndex] = strdup(src);
            srcIndex = objIndex;
            objIndex++;
        }

        if (destIndex == -1)
        {
            graph->zoneNames[objIndex] = strdup(dest);
            destIndex = objIndex;
            objIndex++;
        }

        addEdge(graph, srcIndex, destIndex, cost);
    }

    fclose(file);
    int numIslands = 0;
    int *visited = (int *)calloc(graph->V, sizeof(int));
    DFS2(graph, graph->V, visited, &numIslands);
    int *minCost = (int *)calloc(numIslands, sizeof(int));
    createIslands(graph, visited, minCost);
    bubbleSort(minCost, numIslands);
    fprintf(out, "%d\n", numIslands);
    for (i = 0; i < numIslands; i++)
    {
        fprintf(out, "%d\n", minCost[i]);
    }
    fclose(out);
    //zona de eliberare a memoriei
    for (i = 0; i < graph->V; i++)
    {
        struct AdjListNode *current = graph->array[i].head;
        while (current)
        {
            struct AdjListNode *temp = current;
            current = current->next;
            free(temp);
        }
    }

    for (i = 0; i < graph->V; i++)
    {
        free(graph->zoneNames[i]);
    }

    free(graph->array);
    free(graph->zoneNames);
    free(graph);

    free(visited);
    free(minCost);
}