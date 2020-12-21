#include "..\header\pq.h"

static int *getElements_site(PQ_t *pq, int elementSize, int index) //ok
{
    void *temp = pq->heap.elements;
    temp += elementSize * index;
    return temp;
}

static void createHeap(Heap_t *heap, int elementSize, int Size) //ok
{
    heap->numElements = 0;
    heap->elements = (int *)malloc(elementSize * Size);
}

static void swap(void *elementA, void *elementB, int elementSize) //ok
{
    int *temp = (int *)malloc(sizeof(char) * elementSize);
    if (temp == NULL)
        return;
    memcpy(temp, elementA, elementSize);
    memcpy(elementA, elementB, elementSize);
    memcpy(elementB, temp, elementSize);
    free(temp);
}

static void ReheapDown(PQ_t *pq, int root, int bottom) //2020/12/18
{
    int leftchild = root * 2 + 1;
    int rightchild = root * 2 + 2;
    int MAXchild, MINchild;
    if (leftchild <= bottom)
    {
        if (leftchild == bottom)
        {
            MAXchild = leftchild;
            MINchild = leftchild;
        }
        else
        {
            void *leftchild_site = getElements_site(pq, pq->elementSize, leftchild);
            void *rightchild_site = getElements_site(pq, pq->elementSize, rightchild);

            if (pq->compare(leftchild_site, rightchild_site) == 1)
            //  leftchild_value > rightchild_value
            {
                MINchild = rightchild;
                MAXchild = leftchild;
            }
            else
            //  rightchild_value > leftchild_value
            {
                MINchild = leftchild;
                MAXchild = rightchild;
            }
        } //else

        int *root_site = getElements_site(pq, pq->elementSize, root);
        int *MAXchild_site = getElements_site(pq, pq->elementSize, MAXchild);
        int *MINchild_site = getElements_site(pq, pq->elementSize, MINchild);
        if ((pq->pqClass == MAXHEAP) && (pq->compare(root_site, MAXchild_site) == -1))
        // elements[root] < elements[maxChild]
        {
            swap(root_site, MAXchild_site, pq->elementSize);
            ReheapDown(pq, MAXchild, bottom);
        }
        if ((pq->pqClass == MINHEAP) && (pq->compare(root_site, MINchild_site) == 1))
        // elements[root] > elements[minChild]
        {
            swap(root_site, MINchild_site, pq->elementSize);
            ReheapDown(pq, MINchild, bottom);
        }
    } //if (leftchild <= bottom)
}

static void ReheapUp(PQ_t *pq, int root, int bottom)
{
    int parent;
    if (bottom > root) //tree is not empty
    {
        parent = (bottom - 1) / 2;
        int *parent_site = getElements_site(pq, pq->elementSize, parent);
        int *bottom_site = getElements_site(pq, pq->elementSize, bottom);
        if (((pq->pqClass == MAXHEAP) && (pq->compare(parent_site, bottom_site) == -1)) || ((pq->pqClass == MINHEAP) && (pq->compare(parent_site, bottom_site) == 1)))
        // MAXHEAP: elements[parent] < elements[bottom]
        // MINHEAP: elements[parent] > elements[bottom]
        {
            swap(parent_site, bottom_site, pq->elementSize);
            ReheapUp(pq, root, parent);
        }
    }
}

void createPQ(PQ_t *pq, H_class pqClass, int elementSize, int maxSize, int (*compare)(void *elementA, void *elementB))
//initialize priorty queue
{
    pq->pqClass = pqClass;
    createHeap(&pq->heap, elementSize, maxSize);
    pq->maxSize = maxSize;
    pq->elementSize = elementSize;
    pq->compare = compare;
}

int Enqueue(PQ_t *pq, void *elementA) /* add an element into PQ */
{
    pq->heap.numElements++;
    int *bottom_site = getElements_site(pq, pq->elementSize, pq->heap.numElements - 1);
    memcpy(bottom_site, elementA, pq->elementSize);
    ReheapUp(pq, 0, pq->heap.numElements - 1);
    return (int)bottom_site;
}
int IsEmpty(PQ_t *pq) /* return 0: not empty, 1: empty*/
{
    return (pq->heap.numElements == 0);
}
int IsFull(PQ_t *pq) /* return 0: not full, 1:full */
{
    return (pq->maxSize == pq->heap.numElements);
}
void *Dequeue(PQ_t *pq) /*delete an element from PQ */
{
    memcpy(getElements_site(pq, pq->elementSize, 0),
           getElements_site(pq, pq->elementSize, pq->heap.numElements - 1), pq->elementSize);
    //copy content from array[bottom] to array[0]
    pq->heap.numElements--;
    ReheapDown(pq, 0, pq->heap.numElements - 1);
}