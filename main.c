#include "coders/codexion.h"

void *routine_set_coders_in_queue(void *data)
{
    t_shared_data *s_data = (t_shared_data *)data;
    pthread_mutex_lock(s_data->mutex_1);
        s_data->coder.priorety = ++ *s_data->priority_counter;
        push(s_data->m_heap,s_data->coder);
    pthread_mutex_unlock(s_data->mutex_1);
    return NULL;
}

void compiling(TIME t_compile)
{
    usleep(1000 * t_compile);
}

void debugging(TIME t_debug)
{
    usleep(1000 * t_debug);
}

void refactoring (TIME t_refactor)
{
    usleep(1000 * t_refactor);
}

void cooldown (TIME t_cooldown)
{
    usleep(1000 * t_cooldown);
}

// void *routine(void *data)
// {
//     t_shared_data *s_data = (t_shared_data *)data;

//     pthread_mutex_lock(s_data->mutex_1);

//     while (1)
//     {
//         int index_coder = ft_get_index(
//             s_data->m_heap,
//             s_data->coder.coder_id
//         );

//         if (index_coder == -1)
//         {
//             pthread_mutex_unlock(s_data->mutex_1);
//             return NULL;
//         }

//         // Wait until both dongles are free
//         while (s_data->m_heap->queue[index_coder].left->state_dongle ||
//                s_data->m_heap->queue[index_coder].right->state_dongle)
//         {
//             pthread_cond_wait(s_data->cond_1, s_data->mutex_1);

//             // Recompute index after wake-up (heap may have changed)
//             index_coder = ft_get_index(
//                 s_data->m_heap,
//                 s_data->coder.coder_id
//             );

//             if (index_coder == -1)
//             {
//                 pthread_mutex_unlock(s_data->mutex_1);
//                 return NULL;
//             }
//         }

//         // Acquire dongles
//         s_data->m_heap->queue[index_coder].left->state_dongle = 1;
//         s_data->m_heap->queue[index_coder].right->state_dongle = 1;
//         s_data->m_heap->queue[index_coder].state_coder = 1;

//         move_coder(s_data->m_heap, index_coder, &index_coder);

//         printf("coder %d takes dongle %d\n",
//                s_data->m_heap->queue[index_coder].coder_id,
//                s_data->m_heap->queue[index_coder].left->dongle_id);

//         printf("coder %d takes dongle %d\n",
//                s_data->m_heap->queue[index_coder].coder_id,
//                s_data->m_heap->queue[index_coder].right->dongle_id);

//         printf("coder %d is compiling ...\n\n",
//                s_data->m_heap->queue[index_coder].coder_id);

//         pthread_mutex_unlock(s_data->mutex_1);

//         // Work outside the critical section
//         compiling(s_data->args->time_to_compile);
//         cooldown(s_data->args->dongle_cooldown);

//         pthread_mutex_lock(s_data->mutex_1);

//         // Recompute again before releasing (safest)
//         index_coder = ft_get_index(
//             s_data->m_heap,
//             s_data->coder.coder_id
//         );

//         if (index_coder != -1)
//         {
//             s_data->m_heap->queue[index_coder].left->state_dongle = 0;
//             s_data->m_heap->queue[index_coder].right->state_dongle = 0;
//             s_data->m_heap->queue[index_coder].state_coder = 0;
//         }

//         pthread_cond_broadcast(s_data->cond_1);
//         break;
//     }

//     pthread_mutex_unlock(s_data->mutex_1);
//     return NULL;
// }

void *routine(void *data)
{
    t_shared_data *s_data = (t_shared_data *)data;
    pthread_mutex_lock(s_data->mutex_1);

    int index_coder = ft_get_index(s_data->m_heap,s_data->coder.coder_id);
    if(index_coder == -1)
    {
        pthread_mutex_unlock(s_data->mutex_1);
        return NULL;
    }

    if(!s_data->m_heap->queue[index_coder].left->state_dongle && !s_data->m_heap->queue[index_coder].right->state_dongle)
    {
        s_data->m_heap->queue[index_coder].left->state_dongle = 1;
        s_data->m_heap->queue[index_coder].right->state_dongle = 1;
        s_data->m_heap->queue[index_coder].state_coder = 1;
        move_coder(s_data->m_heap,index_coder,&index_coder); 
        printf("coder %d takes dongle %d\n",s_data->m_heap->queue[index_coder].coder_id,s_data->m_heap->queue[index_coder].left->dongle_id);
        printf("coder %d takes dongle %d\n",s_data->m_heap->queue[index_coder].coder_id,s_data->m_heap->queue[index_coder].right->dongle_id);
        printf("coder %d is compiling ...\n",s_data->m_heap->queue[index_coder].coder_id);
        printf("\n");

        pthread_mutex_unlock(s_data->mutex_1);
        compiling(s_data->args->time_to_compile);
        cooldown(s_data->args->dongle_cooldown);
        
        pthread_mutex_lock(s_data->mutex_1);
        
        s_data->m_heap->queue[index_coder].left->state_dongle = 0;
        s_data->m_heap->queue[index_coder].right->state_dongle = 0;
        s_data->m_heap->queue[index_coder].state_coder = 0;  
        // pop(s_data->m_heap);
        pthread_cond_broadcast(s_data->cond_1);

    }
    else
        pthread_cond_wait(s_data->cond_1,s_data->mutex_1);
    pthread_mutex_unlock(s_data->mutex_1);
    return NULL;
}



int main(int c,char **v)
{
    pthread_mutex_t mutex;
    pthread_cond_t cond_1;
    pthread_mutex_init(&mutex, NULL);  
    pthread_cond_init(&cond_1, NULL);
    t_args args;
    if(!parsing(c,v,&args))
        return 1;
    t_shared_data *shared_data = malloc(sizeof(t_shared_data) * args.number_of_coders);

    min_heap *m_heap = malloc(sizeof(min_heap));
    t_coder tmp;
    pthread_t arr_pthread[args.number_of_coders];
    if(!m_heap  || !shared_data)
        return 1;
    m_heap->queue = malloc(sizeof(t_coder) * args.number_of_coders);
    m_heap->size = 0;
    if(!m_heap->queue)
        return 1;

    int i;
    i = -1;
    t_dongle *left;
    t_dongle *right;
    t_dongle *right_tmp;
    t_dongle *left_tmp;
    unsigned int priority_counter = 0;
    while (++i < (int)args.number_of_coders)
    {
        if(i == 0)
        {
            left = malloc(sizeof(t_dongle));
            right = malloc(sizeof(t_dongle));
            if(!left || !right)
                return 1;
            left->dongle_id = i+1;
            right->dongle_id = i+2;
            right_tmp = right;
            left_tmp = left;

        }
        else if(i == (int)args.number_of_coders - 1)
        {
            left = right_tmp;
            right = left_tmp;
        }
        else
        {
            right = malloc(sizeof(t_dongle));
            left = right_tmp;
            if(!right)
                return 1;
            right->dongle_id = i+2;
            right_tmp = right;            
        }
        
        left->state_dongle = 0;
        right->state_dongle = 0;
        
        tmp.coder_id = i+1;
        tmp.count_compiled = 0;
        tmp.state_coder = 0;
        tmp.left = left;
        tmp.right = right;
        
        (shared_data + i)->args = &args;
        (shared_data + i)->priority_counter = &priority_counter;
        (shared_data + i)->coder = tmp;
        (shared_data + i)->cond_1 = &cond_1;
        (shared_data + i)->m_heap = m_heap;
        (shared_data + i)->mutex_1 = &mutex;
        pthread_create(arr_pthread + i,NULL,routine_set_coders_in_queue,(shared_data + i));
    }
    i = -1;
    while (++i < (int)args.number_of_coders)
        pthread_join(arr_pthread[i],NULL);
    
    
    i = -1;
    while (++i < (int)m_heap->size)
    {
        printf("id : %d\n",m_heap->queue[i].coder_id);
    }
    
    i = -1;
    while (++i < (int)args.number_of_coders)
        pthread_create(arr_pthread + i,NULL,routine,(shared_data + i));
    
    i = -1;
    while (++i < (int)args.number_of_coders)
        pthread_join(arr_pthread[i],NULL);

    i = -1;
    printf("\n");
    while (++i < (int)m_heap->size)
    {
        printf("id : %d state dongle left %d and right %d\n",m_heap->queue[i].coder_id,m_heap->queue[i].left->state_dongle,m_heap->queue[i].right->state_dongle);
    }
    
    
    



    



    
    
    
    pthread_cond_destroy(&cond_1);       
    pthread_mutex_destroy(&mutex);     
    



    return 0;
}