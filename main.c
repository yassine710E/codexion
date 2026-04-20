#include "coders/codexion.h"

pthread_mutex_t mutex_1;
pthread_cond_t cond_1;


void *routine_set_coders_in_queue(void *data)
{
    t_shared_data *s_data = (t_shared_data *)data;
    pthread_mutex_lock(&mutex_1);
    push(s_data->m_heap,s_data->coder);
    pthread_mutex_unlock(&mutex_1);

    return NULL;
}



void *routine(void *data)
{
    (void)data;
    return NULL;
}



int main(int c,char **v)
{
    pthread_mutex_init(&mutex_1, NULL);  
    pthread_cond_init(&cond_1, NULL); 
    t_args args;
    if(!parsing(c,v,&args))
        return 1;
    min_heap *m_heap = malloc(sizeof(min_heap));
    t_coder tmp;
    pthread_t arr_pthread[args.number_of_coders];
    t_shared_data *shared_data = malloc(sizeof(t_shared_data) * args.number_of_coders);
    if(!m_heap  || !shared_data)
        return 1;
    m_heap->queue = malloc(sizeof(t_coder) * args.number_of_coders);
    m_heap->size = 0;
    if(!m_heap->queue)
        return 1;

    int i,even_or_odd_id;
    i = -1;
    even_or_odd_id = 1;
    t_dongle *f_coder_left;
    t_dongle *arr_dongles[args.number_of_coders];
    t_dongle *left;
    t_dongle *right;

    int index_arr_dongles = 0;
    while (++i < (int)args.number_of_coders)
    {
        if(even_or_odd_id % 2 == 0)
        {
            left = arr_dongles[index_arr_dongles++];
            right = arr_dongles[index_arr_dongles++];
        }
        else
        {
            left= ft_create_dongle(even_or_odd_id);
            if(i == 0)
                f_coder_left = left;
            right = even_or_odd_id == (int)args.number_of_coders ? f_coder_left : ft_create_dongle(even_or_odd_id + 1);
            if(i == 0)
                  arr_dongles[index_arr_dongles++] = right;  
            else
            {
                arr_dongles[index_arr_dongles++] = left;
                arr_dongles[index_arr_dongles++] = right;
            }
        }
        tmp.count_compiled = 0;
        tmp.coder_id = even_or_odd_id;
        tmp.priorety = i+1;
        tmp.left = left;
        tmp.right = right;
        even_or_odd_id += 2;
        if(even_or_odd_id > (int)args.number_of_coders){
            even_or_odd_id = 2;
            if (index_arr_dongles < (int)args.number_of_coders)
                arr_dongles[index_arr_dongles] = f_coder_left;
            index_arr_dongles = 0;
        }
        (shared_data+i)->args = &args;
        (shared_data+i)->coder = tmp;
        (shared_data+i)->m_heap = m_heap;
        pthread_create(arr_pthread + i,NULL,routine_set_coders_in_queue,(shared_data + i));
    }

    i = -1;
    while (++i < (int)args.number_of_coders)
        pthread_join(arr_pthread[i],NULL);
    
    
    
    i = -1;
    printf("after : \n");
    while (++i < (int)m_heap->size)
        printf("priority : %d\n",m_heap->queue[i].priorety);
        


    



    
    
    
    pthread_cond_destroy(&cond_1);       
    pthread_mutex_destroy(&mutex_1);     
    



    return 0;
}