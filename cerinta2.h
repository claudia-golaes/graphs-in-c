struct AdjListNode2
{
    int dest;
    int cost;
    struct AdjListNode2 *next;
};

struct AdjList2
{
    struct AdjListNode2 *head;
};

struct Graph2
{
    int V;
    struct AdjList2 *array;
    char **zoneNames;
    int *depths;
};
void main_cerinta2();
