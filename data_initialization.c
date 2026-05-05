#include "coders/codexion.h"

min_heap *init_min_heap()
{
    min_heap *heap = malloc(sizeof(min_heap));
    if(!heap)
        return NULL;
    heap->queue = malloc(sizeof(t_coder) * 2);
    if(!heap->queue)
        return NULL;
    heap->size = 0;
    return heap;
}

void set_dongle(t_dongle *dongle,unsigned int dongle_id,min_heap *m_heap,pthread_mutex_t *mutex,pthread_cond_t *cond)
{
    dongle->dongle_id = dongle_id;
    dongle->m_heap = m_heap;
    dongle->mutex_dongle = mutex;
    dongle->cond_dongle = cond;
}

void set_coders(t_coder *arr_coders , unsigned int size_coders ,pthread_mutex_t *ptr_mutex,pthread_cond_t *ptr_cond)
{
    unsigned int i = 0;
    t_dongle *left,*right,*tmp_left,*tmp_right;
    min_heap *r_heap,*l_heap;
    while (i < size_coders)
    {
        if (i == 0)
        {
            left = malloc(sizeof(t_dongle));
            right = malloc(sizeof(t_dongle));
            l_heap = init_min_heap();
            r_heap = init_min_heap();
            if(!left || !right || !l_heap || !r_heap)
                return;
            set_dongle(left,i+1,l_heap,ptr_mutex+i,ptr_cond+i);
            set_dongle(right,i+2,r_heap,ptr_mutex+i+1,ptr_cond+i+1);
            tmp_left = right;
            tmp_right = left;
        }
        else if (i == size_coders-1)
        {
            left = tmp_left;
            right = tmp_right;
        }
        else if(i > 0)
        {
            left = tmp_left;
            right = malloc(sizeof(t_dongle));
            r_heap = init_min_heap();
            if(!right || !r_heap)
                return;
            set_dongle(right,i+2,r_heap,ptr_mutex+i+1,ptr_cond+i+1);
            tmp_left = right;
        }
        arr_coders[i].coder_id = i+1;
        arr_coders[i].count_compiled = 0;
        arr_coders[i].left = left;
        arr_coders[i].right=right;
        i++;
    }
}


void set_shared_data (t_shared_data *arr_s_data,t_coder *arr_coders,t_args *args,pthread_mutex_t *d_mutex,pthread_cond_t *d_cond,pthread_mutex_t *main_mutex,pthread_cond_t *main_cond,struct timeval start,unsigned int *how_many_waits)
{
    unsigned int i = 0;
    while (i < args->number_of_coders)
    {
        arr_s_data[i].args = args;
        arr_s_data[i].coder = arr_coders+i;
        arr_s_data[i].cond_display = d_cond;
        arr_s_data[i].mutex_display = d_mutex;
        arr_s_data[i].main_mutex = main_mutex;
        arr_s_data[i].main_cond = main_cond;
        arr_s_data[i].start = start;
        arr_s_data[i].how_many_coders_wait = how_many_waits;
        i++;
    }
    
}