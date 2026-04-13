#include "coders/codexion.h"

void set_coders(t_coder *coder,unsigned int n_coders)
{
    unsigned int i = 1;
    unsigned int index = 0;
    while (i <= n_coders)
    {
        (coder+index)->coder_id = i;
        (coder+index)->count_compiled = 0;
        (coder+index)->left = NULL;
        (coder+index)->right= NULL; 
        (coder+index++)->state_coder = "initialization";
        i += 2;
        
    }
    i = 2;
    while (i <= n_coders)
    {
        (coder+index)->coder_id = i;
        (coder+index)->count_compiled = 0;
        (coder+index)->left = NULL;
        (coder+index)->right= NULL; 
        (coder+index++)->state_coder = "initialization";
        i += 2;
    }

}

void set_dongles (t_dongle *dongle,unsigned int n_dongles)
{
    unsigned int i = 0;
    while (i < n_dongles)
    {
        dongle->dongle_id = i+1;
        dongle->state_dongle = 1;
        if(i < n_dongles - 1)
            dongle->next = malloc(sizeof(t_dongle));
        else
            dongle->next = NULL;
        dongle = dongle->next;
        i++;
    }
    
}

void set_shared_data(t_shared_data *data,t_coder *coders,t_dongle *dongles,t_args *args)
{
    data->queue_coders = coders;
    data->list_dongles = dongles;
    data->args = args;
    data->size_coders = args->number_of_coders;
    data->start_index_list = 0;
    data->index_queue = 0; 

}

