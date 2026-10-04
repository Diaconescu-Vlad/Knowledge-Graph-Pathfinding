#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "../include/query.h"
#include "../include/heap.h"

/* =========================================================
 * query.c
 * Parsarea și dispatcharea interogărilor de graf:
 *   EXISTS, EDGE, NEIGHBORS, PATH (BFS), DIJKSTRA
 * ========================================================= */

/* ----------------------------------------------------------
 * Helper de parsare
 * ---------------------------------------------------------- */

QueryType parse_query_type(const char *line)
{
    /* TODO: potrivește cuvântul cheie de la începutul liniei */
    if(strncmp(line, "EXISTS",6)==0) return Q_EXISTS;
    if(strncmp(line, "EDGE",4)==0) return Q_EDGE;
    if(strncmp(line, "NEIGHBORS",9)==0) return Q_NEIGHBORS;
    if(strncmp(line, "PATH",4)==0) return Q_PATH;
    if(strncmp(line, "DIJKSTRA",8)==0) return Q_DIJKSTRA;
    return Q_UNKNOWN;
}

/* ----------------------------------------------------------
 * EXISTS
 * ---------------------------------------------------------- */

void process_exists(const BST *tree, const char *name)
{
    /* TODO: caută în BST, afișează DA/NU */
    GraphNode *gn=bst_search(tree,name);
    if(gn){
        printf("EXISTS %s: DA\n",name);
    }else{
        printf("EXISTS %s: NU\n",name);
    }
}

/* ----------------------------------------------------------
 * EDGE
 * ---------------------------------------------------------- */

void process_edge(const Graph *g, const BST *tree,
                  const char *src_name, const char *dest_name)
{
    /* TODO: găsește ambele noduri via BST, scanează lista de muchii
     *       a sursei pentru destinație */
    (void)g;
    GraphNode *src=bst_search(tree,src_name);
    GraphNode *dest=bst_search(tree,dest_name);
    if(!src||!dest){
        printf("EDGE %s %s: NU\n", src_name, dest_name);
        return;
    }
    EdgeNode *e=src->edges;
    while(e){
        if(e->dest_id==dest->entity.id){
            printf("EDGE %s %s: DA\n",src_name,dest_name);
            return;
        }
        e=e->next;
    }
    printf("EDGE %s %s: NU\n",src_name,dest_name);

}

/* ----------------------------------------------------------
 * NEIGHBORS
 * ---------------------------------------------------------- */

void process_neighbors(const Graph *g, const BST *tree,
                       const char *name)
{
    /* TODO: găsește nodul, iterează lista de muchii, afișează
     *       numele vecinilor; dacă nu există muchii de ieșire
     *       afișează "NEIGHBORS <name>: NULL" */
    (void)g;
    GraphNode *node=bst_search(tree,name);
    if(!node||!node->edges){
        printf("NEIGHBORS %s: NULL\n",name);
        return;
    }
    printf("NEIGHBORS %s:",name);
    EdgeNode *e=node->edges;
    while(e){
        GraphNode *dest=graph_get_node(g,e->dest_id);
        if(dest)
            printf(" %s",dest->entity.name);
        e=e->next;
    }
    printf("\n");
}

/* ----------------------------------------------------------
 * PATH (BFS)
 * ---------------------------------------------------------- */

void process_path_bfs(const Graph *g, const BST *tree,
                      const char *src_name, const char *dest_name)
{
    /* TODO: BFS de la src la dest, reconstruiește și afișează calea;
     *       dacă nu există cale afișează "PATH <src> <dest>: NU" */
    GraphNode *src=bst_search(tree,src_name);
    GraphNode *dest=bst_search(tree,dest_name);

    if(!src||!dest){
        printf("PATH %s %s: NU\n",src_name,dest_name);
        return;
    }

    int n=g->size;
    int src_id=src->entity.id;
    int dest_id=dest->entity.id;

    int *vizitat = (int *)calloc(n, sizeof(int));
    int *parinte = (int *)malloc(n * sizeof(int));
    int *coada = (int *)malloc(n * sizeof(int));
    if (!vizitat || !parinte || !coada) {
        if(vizitat) free(vizitat);
        if(parinte) free(parinte);
        if(coada) free(coada);
        return;
    }
    for(int i=0;i<n;i++){
        parinte[i]=-1;
    }
    int head=0, tail=0;
    coada[tail++]=src_id;
    vizitat[src_id]=1;
    while(head<tail){
        int current_id=coada[head++];
        if(current_id==dest_id)
            break;
        EdgeNode *e = g->nodes[current_id].edges;
        while (e) {
            int vecin = e->dest_id;
            if (!vizitat[vecin]) {
                vizitat[vecin] = 1;
                parinte[vecin] = current_id; 
                coada[tail++] = vecin;
            }
            e = e->next;
        }
    }
    if(!vizitat[dest_id]){
        printf("PATH %s %s: NU\n",src_name,dest_name);
    }
    else{
        int drum[1000], lungime=0;
        int deplasare=dest_id;
        while(deplasare!=-1){
            drum[lungime++]=deplasare;
            deplasare=parinte[deplasare];
        }
        printf("PATH %s %s: ",src_name,dest_name);
        for(int i=lungime-1;i>=0;i--){
            printf("%s",g->nodes[drum[i]].entity.name);
            if(i>0)
                printf(" -> ");
        }
        printf("\n");
    }
    free(vizitat);
    free(parinte);
    free(coada);
}

/* ----------------------------------------------------------
 * DIJKSTRA
 * ---------------------------------------------------------- */

void process_dijkstra(const Graph *g, const BST *tree,
                      const char *src_name, const char *dest_name)
{
    /* TODO: Dijkstra cu min-heap, afișează costul și calea;
     *       dacă nu există cale afișează "DIJKSTRA <src> <dest>: NU" */
    GraphNode *src=bst_search(tree,src_name);
    GraphNode *dest=bst_search(tree, dest_name);
    if(!src || !dest){
        printf("DIJKSTRA %s %s: NU\n",src_name,dest_name);
        return;
    }
    int n=g->size;
    float *dist=(float*)malloc(n*sizeof(float));
    int *parinte=(int *)malloc(n*sizeof(int));
    MinHeap *h=heap_create(n*2);

    for(int i=0;i<n;i++){
        dist[i]=FLT_MAX;
        parinte[i]=-1;
    }
    dist[src->entity.id]=0.0f;
    heap_push(h,src->entity.id,0.0f);
    while(!heap_is_empty(h)){
        HeapNode extra=heap_pop(h);
        int u=extra.node_id;
        if(u==dest->entity.id)
            break;
        if(extra.dist > dist[u])
            continue;
        EdgeNode *e=g->nodes[u].edges;
        while(e){
            int v=e->dest_id;
            float cost_nou=dist[u]+e->cost;

            if(cost_nou<dist[v]){
                dist[v]=cost_nou;
                parinte[v]=u;
                heap_push(h,v,cost_nou);
            }
            e=e->next;
        }
    }
    if(dist[dest->entity.id]==FLT_MAX){
        printf("DIJKSTRA %s %s: NU\n",src_name,dest_name);
    }
    else{
        int drum[1000], lungime=0;
        int deplasare=dest->entity.id;
        while(deplasare!=-1){
            drum[lungime++]=deplasare;
            deplasare=parinte[deplasare];
        }
        printf("DIJKSTRA %s %s: COST = %.2f; DRUM = ", src_name, dest_name, dist[dest->entity.id]);
        for(int i=lungime-1;i>=0;i--){
            printf("%s",g->nodes[drum[i]].entity.name);
            if(i>0){
                printf(" -> ");
            }
        }
        printf("\n");
    }
    free(dist);
    free(parinte);
    heap_free(h);
}

/* ----------------------------------------------------------
 * Dispatcher batch
 * ---------------------------------------------------------- */

void process_all_queries(Queue *q, const Graph *g, const BST *tree)
{
    /* TODO: extrage fiecare linie, parsează tipul, dispatchează
     *       la handlerul corespunzător */
    if(!q||!g||!tree)
        return;
    while(!queue_is_empty(q)){
        char *line=queue_dequeue(q);
        char tip_str[32]={0}, arg1[128]={0}, arg2[128]={0};
        int cuv_gasite=sscanf(line,"%s %s %s", tip_str, arg1, arg2);
        QueryType tip=parse_query_type(tip_str);
        if(tip==Q_EXISTS && cuv_gasite>=2){
            process_exists(tree,arg1);
        }
        else if(tip==Q_EDGE && cuv_gasite>=3){
            process_edge(g,tree,arg1,arg2);
        }
        else if(tip==Q_NEIGHBORS && cuv_gasite>=2){
            process_neighbors(g,tree,arg1);
        }
        else if(tip==Q_PATH && cuv_gasite>=3){
            process_path_bfs(g,tree,arg1,arg2);
        }
        else if(tip==Q_DIJKSTRA && cuv_gasite>=3){
            process_dijkstra(g,tree,arg1,arg2);
        }
        free(line);
    }
}
