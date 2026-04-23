#include "coders/codexion.h"

void compiling(TIME t_compile)
{
    usleep(t_compile);
}

void debugging(TIME t_debug)
{
    printf("debugging ...\n");
    usleep(t_debug);
}

void refactoring (TIME t_refactor)
{
    printf("refactoring ...\n");
    usleep(t_refactor);
}

void cooldown(TIME t_cooldown)
{
    usleep(t_cooldown);
}
int ft_check_dongles(min_heap *min_heap,t_dongle *left,t_dongle *right)
{
    unsigned int i = 0;
    t_coder *queue = min_heap->queue;
    while (i < min_heap->size)
    {
        if((queue + i)->left == left ||
         (queue + i)->left == right ||
          (queue + i)->right == right ||
           (queue + i)->right == left )
           return 1;
        i++;
    }
    return 0;
    
}

void push(min_heap *min_heap,t_coder info)
{
    min_heap->queue[min_heap->size].coder_id = info.coder_id;
    min_heap->queue[min_heap->size].count_compiled = info.count_compiled;
    min_heap->queue[min_heap->size].left = info.left;
    min_heap->queue[min_heap->size++].right = info.right;

}

void pop(min_heap *min_heap)
{
    if (min_heap->size > 1)
    {
        unsigned int i = 1;
        while (i < min_heap->size)
        {
            min_heap->queue[i-1].coder_id = min_heap->queue[i].coder_id;
            min_heap->queue[i-1].count_compiled = min_heap->queue[i].count_compiled;
            min_heap->queue[i-1].left = min_heap->queue[i].left;
            min_heap->queue[i-1].right = min_heap->queue[i].right;
            i++;
        }
        
    }
    min_heap->size -=1;
}

void *routine(void *data)
{
    t_shared_data *s_data = (t_shared_data *)data;
    pthread_mutex_lock(s_data->m_heap_compile->mutex);

    if(!ft_check_dongles(s_data->m_heap_compile,s_data->coder.left,s_data->coder.right))
    {
        push(s_data->m_heap_compile,s_data->coder);
        printf("coder %d has taken a dongle %d\n",s_data->m_heap_compile->queue->coder_id,s_data->m_heap_compile->queue->left->dongle_id);
        printf("coder %d has taken a dongle %d\n",s_data->m_heap_compile->queue->coder_id,s_data->m_heap_compile->queue->right->dongle_id);
        compiling(s_data->args->time_to_compile);
        printf("coder %d is compiling\n",s_data->m_heap_compile->queue->coder_id);
        pop(s_data->m_heap_compile);
        cooldown(s_data->args->dongle_cooldown);

        // pthread_mutex_lock(s_data->m_heap_debug_refactore->mutex);
        // push(s_data->m_heap_debug_refactore,s_data->coder);
        // int i = -1;
        // while (++i < (int)s_data->m_heap_debug_refactore->size)
        //     printf("coder %d takes %d and %d\n",s_data->m_heap_debug_refactore->queue[i].coder_id,s_data->m_heap_debug_refactore->queue[i].left->dongle_id,s_data->m_heap_debug_refactore->queue[i].right->dongle_id);        
        // pthread_mutex_unlock(s_data->m_heap_debug_refactore->mutex);        
        



        
        pthread_cond_broadcast(s_data->m_heap_compile->cond);
        

        

    }
    else
    {
        pthread_cond_wait(s_data->m_heap_compile->cond,s_data->m_heap_compile->mutex);
    }
    pthread_mutex_unlock(s_data->m_heap_compile->mutex);

    return NULL;
}







int main(int c,char **v)
{
    t_args args;
    pthread_mutex_t m_1,m_2,m_3;
    pthread_cond_t c_1,c_2,c_3;
    
    //initialize the mutexes
    pthread_mutex_init(&m_1,NULL);
    pthread_mutex_init(&m_2,NULL);
    pthread_mutex_init(&m_3,NULL);

    //initialize condition variables
    pthread_cond_init(&c_1,NULL);
    pthread_cond_init(&c_2,NULL);
    pthread_cond_init(&c_3,NULL);

    if(!parsing(c,v,&args))
        return 1;
    
    pthread_t arr_threads[args.number_of_coders];
    t_shared_data *s_data = malloc(sizeof(t_shared_data) * args.number_of_coders);
    
    min_heap *compiling_queue = malloc(sizeof(min_heap));
    compiling_queue->cond = &c_1;
    compiling_queue->mutex = &m_1;
    compiling_queue->size = 0;
    compiling_queue->queue = malloc(sizeof(t_coder) * (args.number_of_coders / 2));
    
    min_heap *waiting_queue = malloc(sizeof(min_heap));
    waiting_queue->cond = &c_2;
    waiting_queue->mutex = &m_2;
    compiling_queue->size = 0;
    compiling_queue->queue = malloc(sizeof(t_coder) * (args.number_of_coders - (args.number_of_coders / 2)));
    
    min_heap *debugging_refactoring_queue = malloc(sizeof(min_heap));
    debugging_refactoring_queue->cond = &c_3;
    debugging_refactoring_queue->mutex = &m_3;
    debugging_refactoring_queue->size = 0;
    debugging_refactoring_queue->queue = malloc(sizeof(t_coder) *(args.number_of_coders - (args.number_of_coders / 2)));
    if(!s_data || !compiling_queue || !waiting_queue || !debugging_refactoring_queue || !compiling_queue->queue || !compiling_queue->queue || !debugging_refactoring_queue->queue)
        return 1;
    t_coder tmp;
    
    t_dongle *arr_dongles[args.number_of_coders];
    int index_d = 0;
    int index_aff = 0;
    int i = -1;
    
    while (++i < (int)args.number_of_coders)
    {
        if(i == 0)
        {
            arr_dongles[index_d] = malloc(sizeof(t_dongle));
            arr_dongles[index_d]->dongle_id = index_d+1;
            index_d++;
            arr_dongles[index_d] = malloc(sizeof(t_dongle));
            arr_dongles[index_d]->dongle_id = index_d+1;
            index_d++;
        }
        else if(i != (int)args.number_of_coders -1)
        {
            arr_dongles[index_d] = malloc(sizeof(t_dongle));
            arr_dongles[index_d]->dongle_id = index_d+1;
            index_d++;
        }
        
        //set coder
        tmp.coder_id = i+1;
        tmp.count_compiled = 0;
        tmp.left = arr_dongles[index_aff];
        index_aff++;
        tmp.right = i == (int)args.number_of_coders - 1 ?  arr_dongles[0] :arr_dongles[index_aff];
        
        //prepare the share data
        (s_data+i)->args = &args;
        (s_data+i)->coder = tmp;
        (s_data+i)->m_heap_compile = compiling_queue;
        (s_data+i)->m_heap_debug_refactore = debugging_refactoring_queue;
        (s_data+i)->m_heap_waiters = waiting_queue;
        pthread_create(arr_threads+i,NULL,routine,s_data+i);
    }

    i = -1;
    while (++i < (int)args.number_of_coders)
        pthread_join(arr_threads[i],NULL);  
    

    

    //destroy the mutexes
    pthread_mutex_destroy(&m_1);
    pthread_mutex_destroy(&m_2);
    pthread_mutex_destroy(&m_3);

    //destroy condition variables
    pthread_cond_destroy(&c_1);
    pthread_cond_destroy(&c_2);
    pthread_cond_destroy(&c_3);
    


    return 0;
}