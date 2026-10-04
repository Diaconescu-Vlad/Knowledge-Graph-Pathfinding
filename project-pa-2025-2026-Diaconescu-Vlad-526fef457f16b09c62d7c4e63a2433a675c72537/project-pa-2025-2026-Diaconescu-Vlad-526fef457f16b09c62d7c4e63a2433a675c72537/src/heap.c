#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#include "../include/heap.h"

/* =========================================================
 * heap.c
 * Min-heap de perechi (node_id, dist) pentru Dijkstra.
 * Folosește un tablou dinamic cu indexarea standard
 * părinte/copil:
 *   parinte(i)       = (i - 1) / 2
 *   copil_stang(i)   = 2 * i + 1
 *   copil_drept(i)   = 2 * i + 2
 * ========================================================= */

/* ----------------------------------------------------------
 * Helper intern de swap
 * ---------------------------------------------------------- */

static void swap(HeapNode *a, HeapNode *b)
{
    /* TODO: interschimbă cele două elemente HeapNode */
    HeapNode tmp=*a;
    *a=*b;
    *b=tmp;
}

/* ----------------------------------------------------------
 * Funcții de sift
 * ---------------------------------------------------------- */

static void sift_up(MinHeap *h, int i)
{
    /* TODO: mută elementul de la indexul i în sus până când
     *       proprietatea heap este respectată */
    while(i>0){
        int parent=(i-1)/2;
        if(h->data[i].dist<h->data[parent].dist){
            swap(&h->data[i],&h->data[parent]);
            i=parent;
        }
        else{
            break;
        }
    }
}

static void sift_down(MinHeap *h, int i)
{
    /* TODO: mută elementul de la indexul i în jos până când
     *       proprietatea heap este respectată */
    int n=h->size;
    while(1){
        int smallest=i;
        int left=2*i+1;
        int right=2*i+2;
        if(left<n && h->data[left].dist < h->data[smallest].dist)
            smallest=left;
        if(right<n && h->data[right].dist < h->data[smallest].dist)
            smallest=right;
        if(smallest==i)
            break;
        swap(&h->data[i], &h->data[smallest]);
        i=smallest;
    }
}

/* ----------------------------------------------------------
 * Ciclul de viață
 * ---------------------------------------------------------- */

MinHeap *heap_create(int initial_capacity)
{
    /* TODO: alocă MinHeap și tabloul său de date */
    if(initial_capacity <=0)
        initial_capacity = 16;
    MinHeap *h = (MinHeap *)malloc(sizeof(MinHeap));
    if(!h)
        return NULL;
    h->data=(HeapNode *)malloc(sizeof(HeapNode));
    if(!h->data){
        free(h);
        return NULL;
    }
    h->size =0;
    h->capacity=initial_capacity;
    return h;
}

void heap_free(MinHeap *h)
{
    /* TODO: eliberează tabloul de date, apoi structura */
    if(!h)
        return;
    free(h->data);
    free(h);
}

/* ----------------------------------------------------------
 * Operații de bază
 * ---------------------------------------------------------- */

int heap_push(MinHeap *h, int node_id, float dist)
{
    /* TODO: adaugă elementul, extinde dacă e necesar, execută sift up */
    if(h->size==h->capacity){
        int new_cap=h->capacity*2;
        HeapNode *tmp=(HeapNode *)realloc(h->data, new_cap*sizeof(HeapNode));
        if(!tmp)
            return -1;
        h->data=tmp;
        h->capacity=new_cap;
    }
    h->data[h->size].node_id=node_id;
    h->data[h->size].dist=dist;
    sift_up(h,h->size);
    h->size++;
    return 0;
}

HeapNode heap_pop(MinHeap *h)
{
    /* TODO: interschimbă rădăcina cu ultimul element, micșorează,
     *       execută sift down, returnează vechea rădăcină */
    HeapNode rezultat = {-1, FLT_MAX};
    if(!h|| h->size==0)
        return rezultat;
    rezultat=h->data[0];
    h->size--;
    if(h->size>0){
        h->data[0]=h->data[h->size];
        sift_down(h,0);
    }
    return rezultat;
}

int heap_is_empty(const MinHeap *h)
{
    /* TODO: returnează 1 când size == 0 */
    if(!h || h->size==0)
        return 1;
    return 0;
}

int heap_decrease_key(MinHeap *h, int node_id, float new_dist)
{
    /* TODO: găsește node_id, actualizează dist dacă e mai mic,
     *       execută sift up */
    if(!h)
        return -1;
    for(int i=0;i<h->size;i++){
        if(h->data[i].node_id==node_id){
            if(new_dist< h->data[i].dist){
                h->data[i].dist=new_dist;
                sift_up(h,i);
                return 0;
            }
            return -1;
        }
    }
    return -1;
}
