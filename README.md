# 建立Priority Queue library 
*2020.12.21* Priority Queue library 的說明與紀錄

作業網址: [https://hackmd.io/@chtsai/2020DS-homework-3](https://hackmd.io/@chtsai/2020DS-homework-3)

github: [https://github.com/erictsai1224tw/erictsai/tree/DS/priority_queue](https://github.com/erictsai1224tw/erictsai/tree/DS/priority_queue)

HACKMD:
[https://hackmd.io/@erictsai1224tw/Priority_Queue_library](https://hackmd.io/@erictsai1224tw/Priority_Queue_library)

**此pq需要的節點結構:**
```c
typedef struct HeapType {
    void *elements; // element array
    int numElements; // number of elements
} Heap_t;

typedef enum {
    MINHEAP=0,
    MAXHEAP
} H_class;

typedef struct PQ {
    H_class pqClass;
    Heap_t  heap;
    int maxSize; 
    int elementSize;  
    int (*compare)(void* elementA, void* elementB);
} PQ_t;
```

## static函式 
外部無法存取這些函式，僅供內部使用

### 取得陣列指定index的位置
```c=
static int *getElements_site(PQ_t *pq, int elementSize, int index)
{
    void *temp = pq->heap.elements;
    temp += elementSize * index;
    return temp;
}
```

### 建立heap
```c=
static void createHeap(Heap_t *heap, int elementSize, int Size)
{
    heap->numElements = 0;
    heap->elements = (int *)malloc(elementSize * Size);
}
```

### swap: 利用memcpy交換元素
```c=
static void swap(void *elementA, void *elementB, int elementSize)
{
    int *temp = (int *)malloc(sizeof(char) * elementSize);
    if (temp == NULL)  //如果沒空間能分配了
        return;
    memcpy(temp, elementA, elementSize);
    memcpy(elementA, elementB, elementSize);
    memcpy(elementB, temp, elementSize);
    free(temp);
}
```

### ReheapDown
```c=
static void ReheapDown(PQ_t *pq, int root, int bottom) 
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
```

### ReheapUp
```c=
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
```

## 使用者可以呼叫使用的函式

### Create Priority Queue
針對結構內部函數進行initialize
```c=
void createPQ(PQ_t *pq, H_class pqClass, int elementSize, int maxSize, int (*compare)(void *elementA, void *elementB))
//initialize priorty queue
{
    pq->pqClass = pqClass;
    createHeap(&pq->heap, elementSize, maxSize);
    pq->maxSize = maxSize;
    pq->elementSize = elementSize;
    pq->compare = compare;
}
```

### Enqueue
將元素插入到priority queue中
```c=
int Enqueue(PQ_t *pq, void *elementA) /* add an element into PQ */
{
    pq->heap.numElements++;
    int *bottom_site = getElements_site(pq, pq->elementSize, pq->heap.numElements - 1);
    memcpy(bottom_site, elementA, pq->elementSize);
    ReheapUp(pq, 0, pq->heap.numElements - 1);
    return (int)bottom_site;
}
```

### IsEmpty
```c=
int IsEmpty(PQ_t *pq) /* return 0: not empty, 1: empty*/
{
    return (pq->heap.numElements == 0);
}
```

### IsFull
```c=
int IsFull(PQ_t *pq) /* return 0: not full, 1:full */
{
    return (pq->maxSize == pq->heap.numElements);
}
```

### Dequeue
```c=
void *Dequeue(PQ_t *pq) /*delete an element from PQ */
{
    memcpy(getElements_site(pq, pq->elementSize, 0),
           getElements_site(pq, pq->elementSize, pq->heap.numElements - 1), pq->elementSize);
    //copy content from array[bottom] to array[0]
    pq->heap.numElements--;
    ReheapDown(pq, 0, pq->heap.numElements - 1);
}
```

## Demo
以下為自行宣告的一個結構，並建立一個結構陣列，且對結構內的值進行初始值設定
```c=
typedef struct myElement
{
    char ID[10];
    int math;
    int eng;
    int sci;
} student_t;

student_t node[6] = {
    {"C120308001", 70, 100, 10},
    {"B220406001", 60, 90, 89},
    {"D120306001", 80, 95, 70},
    {"A220407001", 65, 90, 20},
    {"D220506001", 10, 70, 65},
    {"A120406001", 90, 90, 40}
    };
```

### 建立比較函式及印出函式
使用者須自建，並傳入priority queue library中 (比較函式)
```c=
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

void print(PQ_t *pq)
{
    student_t *temp;
    for (int i = 0; i < pq->heap.numElements; i++)
    {
        temp = (student_t *)(pq->heap.elements + i * sizeof(student_t));
        printf("index=%d, ID=%s, math=%d, eng=%d, sci=%d\n", i, temp->ID, temp->math, temp->eng, temp->sci);
    }
}
```


### 採用minheap排列
root為最小值，並用"sci"此欄位作為比較大小進行排序
```c=
PQ_t maxPQ;

createPQ(&maxPQ, MINHEAP, sizeof(student_t), 100, compareSci);

for (int i = 0; i < 6; i++)
    Enqueue(&maxPQ, &node[i]);
print(&maxPQ);
printf("\n");
```
![](https://i.imgur.com/rltuE3s.png)

```c
Dequeue(&maxPQ);
```
![](https://i.imgur.com/LtyqmZh.png)

```c
Dequeue(&maxPQ);
```
![](https://i.imgur.com/vugOH1w.png)

可以發現，在每次dequeue後，"student_t"資料型態的陣列中，"sci"欄位最低的陣列元素就會被抓到第一個出現，並在下一次的dequeue中被移除，再由"sci"欄位次低的陣列元素遞補

> 結論: 將每次dequeue出來的root，依照印出的先後順序排序，可發現值是由最小到最大排(值是看比較哪個欄位)

### 採用MAXheap排列
root為最小值，並用"sci"此欄位作為比較大小進行排序
```c=
PQ_t maxPQ;

createPQ(&maxPQ, MAXHEAP, sizeof(student_t), 100, compareSci);

for (int i = 0; i < 6; i++)
    Enqueue(&maxPQ, &node[i]);
print(&maxPQ);
printf("\n");
```
![](https://i.imgur.com/cjgCyx5.png)

```c
Dequeue(&maxPQ);
```
![](https://i.imgur.com/QZKen7S.png)

```c
Dequeue(&maxPQ);
```
![](https://i.imgur.com/x67UJA3.png)

可以發現，在每次dequeue後，"student_t"資料型態的陣列中，"sci"欄位最高的陣列元素就會被抓到第一個出現，並在下一次的dequeue中被移除，再由"sci"欄位次高的陣列元素遞補

> 結論: 將每次dequeue出來的root，依照印出的先後順序排序，可發現值是由最大到最小排 (值是看比較哪個欄位)