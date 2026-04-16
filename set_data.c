#include "coders/codexion.h"

void set_coders(t_coder *coder,unsigned int n_coders)
{

    unsigned int i = 1;
    unsigned int index = 0;
    int ids[n_coders]; 
    while (i <= n_coders)
    {
        ids[index++] = i;
        i += 2;
    }
    i = 2;
    while (i <= n_coders)
    {
        ids[index++] = i;
        i += 2;
    }
    i = 0;
    while (i < n_coders)
    {
        coder->coder_id =  ids[i];
        coder->count_compiled = 0;
        coder->left = NULL;
        coder->right = NULL;

        if(i == n_coders - 1)
            coder->next = NULL;
        else
            coder->next = malloc(sizeof(t_coder));

        coder = coder->next;
        i++;
    }
    

}

t_coder *get_coder(t_coder *coder,unsigned int id_coder)
{
    while(coder)
    {
        if (coder->coder_id == id_coder){
            return coder;
            }
        
        coder = coder->next;
    }   
    return NULL;
}

void set_dongles (t_coder *coder)
{
    unsigned int i = 0;
    t_dongle *dongle;
    t_coder *c_coder = coder;
    while (c_coder)
    {

        dongle = malloc(sizeof(t_dongle));
        dongle->dongle_id = c_coder->coder_id;
        c_coder->left = dongle;
        c_coder = c_coder->next;
        i++;
    }
    c_coder = coder;
    i = 0;
    t_dongle *first_coder_l_dongle = c_coder->left;
    while (c_coder)
    {
        t_coder *coder_goal = get_coder(coder,c_coder->coder_id+1);
        c_coder->right = coder_goal ? coder_goal->left : first_coder_l_dongle;
        c_coder = c_coder->next;
        i++;
    }
}

void set_shared_data(unsigned int id,t_shared_data *data,t_coder *coders,t_args *args)
{
    data->id_thread = id;
    data->queue_coders = coders;
    data->args = args;

}

