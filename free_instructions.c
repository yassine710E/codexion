#include "coders/codexion.h"
void free_pointer(void *ptr)
{
    free(ptr);
}

void free_list (t_dongle *list)
{
    t_dongle *tmp = list;
    while (list)
    {
        tmp = list;
        list = list->next;
        free(tmp);
    }
    
    
}