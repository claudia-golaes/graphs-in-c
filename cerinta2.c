#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cerinta2.h"
#define INT_MAX 9999


// functie pentru adaugarea unui nou nod in lista de adiacenta
struct AdjListNode2 *newAdjListNode2(int dest, int cost)
{
    struct AdjListNode2 *newNode = (struct AdjListNode2 *)malloc(sizeof(struct AdjListNode2));
    newNode->dest = dest;
    newNode->cost = cost;
    newNode->next = NULL;
    return newNode;
}

// functia pentru crearea grafului si initializarea acestuia
struct Graph2 *createGraph2(int V)
{
    struct Graph2 *graph = (struct Graph2 *)malloc(sizeof(struct Graph2));
    graph->V = V;

    graph->array = (struct AdjList2 *)malloc(V * sizeof(struct AdjList2));
    graph->zoneNames = (char **)malloc(V * sizeof(char *));
    graph->depths = (int *)malloc(V * sizeof(int));

    for (int i = 0; i < V; ++i)
    {
        graph->array[i].head = NULL;
        graph->zoneNames[i] = NULL;
        graph->depths[i] = 1;
    }

    return graph;
}

// functie pentru adaugarea unei muchii intre doua noduri (o signura data, fiind
// un graf orientat)
void addEdge2(struct Graph2 *graph, int src, int dest, int cost)
{
    struct AdjListNode2 *newNode = newAdjListNode2(dest, cost);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
}

// functie care gaseste indexul unui nod intr-un graf
int findIndex2(struct Graph2 *graph, const char *node)
{
    for (int i = 0; i < graph->V; i++)
    {
        if (graph->zoneNames[i] == NULL)
            return -1;
        if (strcmp(graph->zoneNames[i], node) == 0)
            return i;
    }
    return -1;
}

//functie pentru dfs
int dfs(struct Graph2 *graph, int src, int dest, int *visited)
{
    visited[src] = 1;

    if (src == dest)
        return 1;

    struct AdjListNode2 *pCrawl = graph->array[src].head;
    while (pCrawl != NULL)
    {
        int v = pCrawl->dest;
        if (visited[v] == 0)
        {
            if (dfs(graph, v, dest, visited))
                return 1;
        }
        pCrawl = pCrawl->next;
    }

    return 0;
}

//functie care verifica daca exista un drum intre doua noduri folosindu-se de 
//functia dfs
int hasPath(struct Graph2 *graph, int src, int dest)
{
    int V = graph->V;
    int *visited = (int *)malloc(V * sizeof(int));
    for (int i = 0; i < V; i++)
    {
        visited[i] = 0;
    }
    return dfs(graph, src, dest, visited);
    free(visited);
}

//functia care imi printeaza in fisierul de iesire drumul optim pana la comoara
//totodata retin si adancimea minima
void printPath(FILE *file, int *parent, int j, struct Graph2 *graph, int *minDepth)
{
    if (*minDepth > graph->depths[j] && graph->depths[j] != 1)
    {
        *minDepth = graph->depths[j];
    }
    if (parent[j] == -1)
        return;

    printPath(file, parent, parent[j], graph, minDepth);
    fprintf(file, " %s", graph->zoneNames[j]);
}

//functie care imiplementeaza atat algoritmul lui dijkstra, cat imi si printeaza
//in fisierul de iesire costul minim, adancimea minima si numarul de drumuri pe 
//care trebuie sa le faca
void dijkstra(struct Graph2 *graph, int src, FILE *file, int weight)
{
    int V = graph->V;
    int *visited = (int *)malloc(V * sizeof(int));
    int *dist = (int *)malloc(V * sizeof(int));
    float *scores = (float *)malloc(V * sizeof(float));
    int *parent = (int *)malloc(V * sizeof(int));
    int minimCost = 0, minDepth = INT_MAX;
    for (int i = 0; i < V; i++)
    {
        visited[i] = 0;
        dist[i] = INT_MAX;
        scores[i] = INT_MAX;
        parent[i] = -1;
    }

    dist[src] = 0;
    scores[src] = 0;

    for (int count = 0; count < V - 1; count++)
    {
        int u;
        float minScore = INT_MAX;

        for (int v = 0; v < V; v++)
        {
            if (visited[v] == 0 && scores[v] < minScore)
            {
                minScore = scores[v];
                u = v;
            }
        }

        visited[u] = 1;

        struct AdjListNode2 *pCrawl = graph->array[u].head;

        while (pCrawl != NULL)
        {
            int v = pCrawl->dest;
            int cost = pCrawl->cost;

            if (!visited[v] && dist[u] != INT_MAX && dist[u] + cost < dist[v])
            {
                dist[v] = dist[u] + cost;
                scores[v] = (float)dist[v] / graph->depths[v];
                parent[v] = u;
            }

            pCrawl = pCrawl->next;
        }
        printf("%d\n", minimCost);
    }
    if (!hasPath(graph, findIndex2(graph, "Corabie"), findIndex2(graph, "Insula")))
    {
        fprintf(file, "Echipajul nu poate ajunge la insula\n");
    }
    else
    {
        if (!hasPath(graph, findIndex2(graph, "Insula"), findIndex2(graph, "Corabie")))
        {
            fprintf(file, "Echipajul nu poate transporta comoara inapoi la corabie\n");
        }
        else
        {
            fprintf(file, "%s", graph->zoneNames[src]);
            printPath(file, parent, findIndex2(graph, "Corabie"), graph, &minDepth);
            fprintf(file, "\n");
            fprintf(file, "%d\n", dist[findIndex2(graph, "Corabie")]);
            fprintf(file, "%d\n", minDepth);
            fprintf(file, "%d", weight / minDepth);
        }
    }
    free(visited);
    free(dist);
    free(scores);
    free(parent);
}

void main_cerinta2()
{
    FILE *file = fopen("tema3.in", "r");
    FILE *out = fopen("tema3.out", "w");
    int n, m, weight;
    fscanf(file, "%d %d", &n, &m);
    struct Graph2 *graph = createGraph2(n);
    int objIndex = 0;
    for (int i = 0; i < m; i++)
    {
        char src[100], dest[100];
        int cost;
        fscanf(file, "%s %s %d", src, dest, &cost);
        int srcIndex = findIndex2(graph, src);
        int destIndex = findIndex2(graph, dest);

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

        addEdge2(graph, srcIndex, destIndex, cost);
    }

    for (int i = 0; i < n; i++)
    {
        char node[100];
        int depth;
        fscanf(file, "%s %d", node, &depth);
        int index = findIndex2(graph, node);
        if (index != -1)
        {
            graph->depths[index] = depth;
        }
    }

    fscanf(file, "%d", &weight);
    for (int i = 0; i < graph->V; i++)
    {
        printf("%s are adancimea %d\n", graph->zoneNames[i], graph->depths[i]);
    }

    printf("%d\n", hasPath(graph, findIndex2(graph, "Insula"), findIndex2(graph, "Corabie")));
    dijkstra(graph, 0, out, weight);

    int i;
    for (i = 0; i < graph->V; i++)
    {
        struct AdjListNode2 *current = graph->array[i].head;
        while (current)
        {
            struct AdjListNode2 *temp = current;
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
    free(graph->depths);
    free(graph);
    fclose(file);
    fclose(out);
}
