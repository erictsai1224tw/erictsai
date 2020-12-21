#ifndef PQ_H_INCLUDED
#define PQ_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct HeapType {
    void *elements; // element array
    int numElements; // number of elements
} Heap_t;

typedef enum {
    MINHEAP=0,
    MAXHEAP //1
} H_class;

typedef struct PQ {
    H_class pqClass;
    Heap_t  heap;
    int maxSize;
    int elementSize;
    int (*compare)(void* elementA, void* elementB);
} PQ_t;

void createPQ(PQ_t *pq, H_class pqClass, int elementSize, int maxSize, int (*compare)(void* elementA, void *elementB));
int Enqueue(PQ_t *pq, void * elementA);
int IsEmpty(PQ_t *pq);
int IsFull(PQ_t *pq);
void * Dequeue(PQ_t *pq);
#endif // DOUBLELINK_H_INCLUDED