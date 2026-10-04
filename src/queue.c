#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/queue.h"

/* =========================================================
 * queue.c
 * Coadă FIFO susținută de o listă simplu înlănțuită.
 * Stochează copii alocate pe heap ale șirurilor.
 * ========================================================= */

/* ----------------------------------------------------------
 * Ciclul de viață
 * ---------------------------------------------------------- */

Queue *queue_create(void)
{
    /* TODO: alocă și inițializează cu zero o structură Queue */
    Queue *q=calloc(1,sizeof(Queue));
    return q;
}

void queue_free(Queue *q)
{
    /* TODO: golește coada eliberând fiecare nod și datele sale */
    if(!q) return;
    while(q->front){
        QueueNode *sterge=q->front;
        q->front=q->front->next;
        free(sterge->data);
        free(sterge);
    }
    free(q);
}

/* ----------------------------------------------------------
 * Mutație
 * ---------------------------------------------------------- */

int queue_enqueue(Queue *q, const char *data)
{
    if (q == NULL || data == NULL) return -1;

    QueueNode *nod = (QueueNode*)malloc(sizeof(QueueNode));
    if (nod == NULL) return -1;
    nod->data = strdup(data);
    nod->next = NULL;

    if (nod->data == NULL) {
        free(nod);
        return -1;
    }
    if (q->front == NULL) {
        q->front = nod;
        q->rear = nod;
        q->size++;
        return 0;
    }
    q->rear->next = nod;
    q->rear = nod;
    q->size++;
    
    return 0;
}


char *queue_dequeue(Queue *q)
{
    /* TODO: elimină nodul din față, returnează datele sale (apelantul
     *       eliberează) */
    if(!q||!q->front) return NULL;
    QueueNode *node=q->front;
    char *data=node->data;
    q->front=node->next;
    if(!q->front){
        q->rear=NULL;
    }
    q->size--;
    free(node);
    return data;
}

/* ----------------------------------------------------------
 * Inspecție
 * ---------------------------------------------------------- */

int queue_is_empty(const Queue *q)
{
    /* TODO: returnează 1 când size == 0 */
    if(!q|| q->size==0) return 1;
    return 0;
}

void queue_print(const Queue *q)
{
    /* TODO: iterează de la front la rear, afișează fiecare șir de date */
    if(!q) 
        return;
    const QueueNode *cur=q->front;
    while(cur){
        printf("%s\n",cur->data);
        cur=cur->next;
    }
}
