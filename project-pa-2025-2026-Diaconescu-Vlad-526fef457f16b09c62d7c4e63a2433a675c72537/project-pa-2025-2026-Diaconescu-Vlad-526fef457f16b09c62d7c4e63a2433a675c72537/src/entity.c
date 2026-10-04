#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/entity.h"

/* =========================================================
 * entity.c
 * Implementarea ciclului de viață al entității și a funcțiilor
 * de conversie.
 * ========================================================= */

/* ----------------------------------------------------------
 * Funcții de conversie
 * ---------------------------------------------------------- */

const char *entity_type_to_str(EntityType type)
{
    /* TODO: returnează șirul corespunzător fiecărei valori enum */
    switch(type){
        case PERSON: return "PERSON";
        case COMPANY: return "COMPANY";
        case LOCATION: return "LOCATION";
        case EVENT: return "EVENT";
        default: return "UNKNOWN";
    }
}

EntityType str_to_entity_type(const char *str)
{
    /* TODO: compară str cu numele cunoscute și returnează enum-ul */
    if(strcmp(str,"PERSON")==0) return PERSON;
    if(strcmp(str,"COMPANY")==0) return COMPANY;
    if(strcmp(str,"LOCATION")==0) return LOCATION;
    if(strcmp(str, "EVENT")==0) return EVENT;
    return (EntityType)-1;
}

/* ----------------------------------------------------------
 * Ciclul de viață
 * ---------------------------------------------------------- */

Entity *entity_create(const char *name, EntityType type, int id)
{
    /* TODO: alocă Entity, duplică name, atribuie câmpurile */
    Entity *e=(Entity *)malloc(sizeof(Entity));
    if(!e) return NULL;
    e->name=strdup(name);
    if(!e->name){
        free(e);
        return NULL;
    }
    e->type=type;
    e->id=id;
    return e;
}

void entity_free(Entity *e)
{
    /* TODO: eliberează name, apoi structura */
    if(!e) return;
    free(e->name);
    free(e);
}

/* ----------------------------------------------------------
 * Afișare
 * ---------------------------------------------------------- */

void entity_print(const Entity *e)
{
    /* TODO: afișează "<id> <name> <TYPE>" */
    if(!e) return;
    printf("%d %s %s\n",e->id,e->name,entity_type_to_str(e->type));
}
