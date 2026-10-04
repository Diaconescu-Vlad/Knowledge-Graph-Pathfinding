#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/graph.h"

/* =========================================================
 * graph.c
 * Implementarea grafului direcționat ponderat cu liste de
 * adiacență.
 * ========================================================= */

/* ----------------------------------------------------------
 * Funcții ajutătoare pentru relații
 * ---------------------------------------------------------- */

const char *relation_type_to_str(RelationType type)
{
    /* TODO: returnează șir cu litere mici pentru fiecare enum de relație */
    switch(type){
        case WORKS_AT: return "works_at";
        case FRIEND_OF: return "friend_of";
        case LOCATED_IN: return "located_in";
        case PARTICIPATES_IN: return "participates_in";
        default: return "unknown";
    }
    
}

RelationType str_to_relation_type(const char *str)
{
    /* TODO: parsează șirul relației la valoarea enum */
    if(strcmp(str,"works_at")==0) return WORKS_AT;
    if(strcmp(str,"friend_of")==0) return FRIEND_OF;
    if(strcmp(str,"located_in")==0) return LOCATED_IN;
    if(strcmp(str,"participates_in")==0) return PARTICIPATES_IN;
    return (RelationType)-1;
}

/* ----------------------------------------------------------
 * Ciclul de viață
 * ---------------------------------------------------------- */

Graph *graph_create(int initial_capacity)
{
    /* TODO: alocă structura Graph și tabloul de noduri */
    if(initial_capacity<=0) initial_capacity=16;
    Graph *g=(Graph*)malloc(sizeof(Graph));
    if(!g){
        free(g);
        return NULL;
    }
    g->nodes=(GraphNode*)malloc(initial_capacity*sizeof(GraphNode));
    if(!g){
        free(g);
        return NULL;
    } 
    g->size=0;
    g->capacity=initial_capacity;
    return g;
}

void graph_free(Graph *g)
{
    /* TODO: eliberează șirul de nume al fiecărui nod și lista de muchii,
     *       apoi tabloul și structura */
    if(!g) return;
    for(int i=0;i<g->size;i++){
        free(g->nodes[i].entity.name);
        EdgeNode *e=g->nodes[i].edges;
        while(e){
            EdgeNode *next=e->next;
            free(e);
            e=next;
        }
    }
    free(g->nodes);
    free(g);
}

/* ----------------------------------------------------------
 * Mutație
 * ---------------------------------------------------------- */

int graph_add_node(Graph *g, const char *name, EntityType type)
{
    /* TODO: extinde tabloul dacă e necesar, inițializează noul GraphNode,
     *       returnează id-ul */
    if(!g||!name)
        return -1;
    if(g->size==g->capacity){
        int new_cap=g->capacity*2;
        GraphNode *tmp=(GraphNode*)realloc(g->nodes,new_cap*sizeof(GraphNode));
        if(!tmp)
            return -1;
        g->nodes=tmp;
        g->capacity=new_cap;
    }
    int id=g->size;
    GraphNode *n=&g->nodes[id];
    n->entity.name=strdup(name);
    if(!n->entity.name)
        return -1;
    n->entity.type=type;
    n->entity.id=id;
    n->edges=NULL;
    g->size++;
    return id;
}

int graph_add_edge(Graph *g, int src_id, int dest_id,
                   RelationType type, float cost)
{
    /* TODO: alocă EdgeNode, adaugă la finalul listei de muchii a sursei
     *       pentru a păstra ordinea de inserare */
    if(!g||src_id<0||src_id>=g->size||dest_id<0||dest_id>=g->size)
        return -1;
    EdgeNode *e=(EdgeNode*)malloc(sizeof(EdgeNode));
    if(!e)
        return -1;
    e->dest_id=dest_id;
    e->type=type;
    e->cost=cost;
    e->next=NULL;
    GraphNode *src=&g->nodes[src_id];
    if(!src->edges){
        src->edges=e;
    }
    else{
        EdgeNode *cur=src->edges;
        while(cur->next){
            cur=cur->next;
        }
        cur->next=e;
    }
    return 0;
}

/* ----------------------------------------------------------
 * Funcții ajutătoare pentru interogare
 * ---------------------------------------------------------- */

int graph_find_id(const Graph *g, const char *name)
{
    /* TODO: scanare liniară returnând id-ul potrivit sau -1 */
    if(!g||!name)
        return -1;
    for(int i=0;i<g->size;i++){
        if(strcmp(g->nodes[i].entity.name,name)==0)
            return i;
    }
    return -1;
}

GraphNode *graph_get_node(const Graph *g, int id)
{
    /* TODO: verifică limitele și returnează pointerul */
    if(!g||id<0||id>=g->size)
        return NULL;
    return &g->nodes[id];
}

/* ----------------------------------------------------------
 * Afișare
 * ---------------------------------------------------------- */

void graph_print(const Graph *g)
{
    /* TODO: afișează fiecare nod și lista sa de muchii */
    if(!g)
        return;
    for(int i=0;i<g->size;i++){
        const GraphNode *n=&g->nodes[i];
        printf("%d %s:",n->entity.id,n->entity.name);
    
        EdgeNode *e=n->edges;
        while(e){
            printf(" [%d %s %.2f]",e->dest_id,relation_type_to_str(e->type),e->cost);
            e=e->next;
        }
        printf("\n");
    }
}
