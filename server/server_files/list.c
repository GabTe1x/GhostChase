#include <stdlib.h> 
#include "list.h"

extern pthread_mutex_t verrou;

// Remove element (return 0 if success, -1 if error)
int removeEl(struct listElements *list, struct element *el)
{
    int result = 0;

    if ((!list) || (!el))
    {
        return -1;
    }
    pthread_mutex_lock(&verrou);
    if (el->prev && el->next) 
    {
        el->prev->next = el->next;
        el->next->prev = el->prev;
    }
    else
    { 
        result = -1;

        if (el == list->first)
        {
            list->first = el->next;

            if (list->first)
            {
                list->first->prev = NULL;
            }

            result = 0;  
        }

        if (el == list->last)
        {
            list->last = el->prev;
            if (list->last) 
            {
                list->last->next = NULL; 
            }

            result = 0;  
        }
    }
    list->count--;
    pthread_mutex_unlock(&verrou);
    
    free(el);
    return result;
}

// Add element after prevEl
struct element* addEl(struct listElements *list, struct element *prevEl, void *data)
{
    struct element *newEl = (struct element*)malloc(sizeof(struct element));
    
    if (!newEl)
    {
        return NULL;
    }

    newEl->data = data;
    newEl->next = NULL;
    newEl->prev = NULL;

    pthread_mutex_lock(&verrou);
    list->count++;
    if (prevEl)
    {
        newEl->next = prevEl->next; 
        newEl->prev = prevEl;
        if (prevEl->next)
        {
            prevEl->next->prev = newEl;
        }
        prevEl->next = newEl;
    }

    if ((!prevEl) && (list->first))
    {
        list->first->prev = newEl;
        newEl->next = list->first;
        list->first = newEl;
    }

    if (!list->first)
    {
        list->first = newEl;
        list->last = newEl;
    }

    if ((list->last) && (list->last == prevEl))
    {
        list->last = newEl;        
    }
    pthread_mutex_unlock(&verrou);
    return newEl;
}