struct AdjListNode
{
    int dest;
    int cost;
    struct AdjListNode *next;
};

struct AdjList
{
    struct AdjListNode *head;
};

struct Graph
{
    int V;
    struct AdjList *array;
    char **zoneNames;
};
void main_cerinta1();