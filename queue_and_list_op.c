#include "coders/codexion.h"
unsigned int size_list(t_dongle *head)
{
    unsigned int counter = 0;
    while (head)
    {
        counter++;
        head = head->next;
    }
    return counter;
}

t_dongle *get_dongle(unsigned int index,t_dongle *head)
{
    unsigned int i = 0;
    while (head)
    {
        if(i == index)
            return head;
        i++;
        head = head->next;
    }
    return NULL;
}

void rotate(t_dongle **pptr)
{
    t_dongle *head = *pptr;
    if (head && head->next)
    {
        while (head && head->next)
            head = head->next;
        head->next = *pptr;
        *pptr = (*pptr)->next;
        head->next->next = NULL; 
    }   
}


void rrotate(t_dongle **pptr)
{
    t_dongle *head = *pptr;
    if(head && head->next)
    {
        while (head && head->next && head->next->next)
            head = head->next;
        t_dongle *tmp = head->next;
        head->next = NULL;
        tmp->next = *pptr;
        *pptr = tmp;


    }    
}