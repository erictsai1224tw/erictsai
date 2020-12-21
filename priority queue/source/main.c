#include "..\header\pq.h"

typedef struct myElement
{
    char ID[10];
    int math;
    int eng;
    int sci;
} student_t;

int compareMath(void *elementA, void *elementB)
{
    int mathA = ((student_t *)elementA)->math;
    int mathB = ((student_t *)elementB)->math;
    if (mathA > mathB)
    {
        return 1;
    }
    else if (mathA < mathB)
    {
        return -1;
    }
    return 0;
}

int compareSci(void *elementA, void *elementB)
{
    int sciA = ((student_t *)elementA)->sci;
    int sciB = ((student_t *)elementB)->sci;
    if (sciA > sciB)
    {
        return 1;
    }
    else if (sciA < sciB)
    {
        return -1;
    }
    return 0;
}

int compareEng(void *elementA, void *elementB)
{
    int engA = ((student_t *)elementA)->eng;
    int engB = ((student_t *)elementB)->eng;
    if (engA > engB)
    {
        return 1;
    }
    else if (engA < engB)
    {
        return -1;
    }
    return 0;
}

void print(PQ_t *pq)
{
    student_t *temp;
    for (int i = 0; i < pq->heap.numElements; i++)
    {
        temp = (student_t *)(pq->heap.elements + i * sizeof(student_t));
        printf("index=%d, ID=%s, math=%d, eng=%d, sci=%d\n", i, temp->ID, temp->math, temp->eng, temp->sci);
    }
}

int main()
{
    student_t node[6] = {
        {"C120308001", 70, 100, 10},
        {"B220406001", 60, 90, 89},
        {"D120306001", 80, 95, 70},
        {"A220407001", 65, 90, 20},
        {"D220506001", 10, 70, 65},
        {"A120406001", 90, 90, 40}};

    PQ_t maxPQ;

    createPQ(&maxPQ, MAXHEAP, sizeof(student_t), 100, compareSci);

    for (int i = 0; i < 6; i++)
        Enqueue(&maxPQ, &node[i]);
    print(&maxPQ);
    printf("\n");


    Dequeue(&maxPQ);
    print(&maxPQ);
    printf("\n");

    Dequeue(&maxPQ);
    print(&maxPQ);
    printf("\n");

    Dequeue(&maxPQ);
    print(&maxPQ);
    printf("\n");

    Dequeue(&maxPQ);
    print(&maxPQ);
    printf("\n");
}
