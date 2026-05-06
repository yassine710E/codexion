#include "coders/codexion.h"

void debugging_hh(t_dongle *dongle)
{   
    int i = 0;
    t_coder *coders_queue = (t_coder *)dongle->m_heap->queue;
    while (i < dongle->m_heap->size)
        printf("%d ",coders_queue[i++].coder_id);
}
void request_dongle(t_coder *coder,t_dongle *dongle,char *scheduler)
{
    pthread_mutex_lock(dongle->mutex_dongle);
    push(dongle->m_heap,coder,scheduler);    
    pthread_mutex_unlock(dongle->mutex_dongle);
}

void leave_dongle (t_dongle *dongle)
{
    pthread_mutex_lock(dongle->mutex_dongle);
    pop(dongle->m_heap);
    pthread_mutex_unlock(dongle->mutex_dongle);
}

// void *coder_routine (void *data)
// {
//     if(!data)
//         return NULL;
//     long times[5];
//     t_shared_data *s_data = (t_shared_data *)data;
//     pthread_mutex_lock(s_data->main_mutex);
    
//     while (s_data->coder->count_compiled < s_data->args->number_of_compiles_required)
//     {
//         while(s_data->coder->left->m_heap->size || s_data->coder->right->m_heap->size)
//              pthread_cond_wait(s_data->main_cond,s_data->main_mutex);
//         request_dongle(s_data->coder,s_data->coder->left,s_data->args->scheduler);
//         request_dongle(s_data->coder,s_data->coder->right,s_data->args->scheduler);
//     pthread_mutex_unlock(s_data->main_mutex);
    
//         times[0] = get_timestamp_ms(s_data->start);
//         times[1] = get_timestamp_ms(s_data->start);
//         times[2] = get_timestamp_ms(s_data->start);
//         s_data->coder->last_compile_start = times[2];
//         compile(s_data->args->time_to_compile*1000);
//         cooldown(s_data->args->dongle_cooldown*1000);
    
//     pthread_mutex_lock(s_data->main_mutex);
//         s_data->coder->count_compiled++;
        
//         leave_dongle(s_data->coder->left);
//         leave_dongle(s_data->coder->right);
        
//         pthread_cond_broadcast(s_data->main_cond);
//     pthread_mutex_unlock(s_data->main_mutex);

//     times[3] = get_timestamp_ms(s_data->start);
//     debugging(s_data->args->time_to_debug*1000);
//     times[4] = get_timestamp_ms(s_data->start); 
//     refactoring(s_data->args->time_to_refactor*1000);
    
//     pthread_mutex_lock(s_data->mutex_display);
//         printf("%ld %d has taken a dongle\n",times[0],s_data->coder->coder_id);
//         printf("%ld %d has taken a dongle\n",times[1],s_data->coder->coder_id);
//         printf("%ld %d is compiling\n",times[2],s_data->coder->coder_id);
//         printf("%ld %d is debugging\n",times[3],s_data->coder->coder_id);
//         printf("%ld %d is refactoring\n",times[4],s_data->coder->coder_id);
//         printf("deadline of coder %d  :  %ld\n",s_data->coder->coder_id,s_data->coder->last_compile_start + s_data->args->time_to_burnout);
//     pthread_mutex_unlock(s_data->mutex_display);
    
//     }
//     return NULL;
// }

void set_priority(t_coder *coder)
{
    if(!coder)
        return;
    t_coder *left_queue = (t_coder *)coder->left->m_heap->queue;
    t_coder *right_queue = (t_coder *)coder->right->m_heap->queue;

    if (!coder->left->m_heap->size && !coder->right->m_heap->size)
        coder->priority_for_fifo = 1;
    else 
    {
        if ((coder->left->m_heap->size && left_queue[0].priority_for_fifo == 1) || (coder->right->m_heap->size && right_queue[0].priority_for_fifo == 1))
            coder->priority_for_fifo = 2;
        else
            coder->priority_for_fifo = 1;
    }
}

void *coder_routine (void *data)
{
    if(!data)
        return NULL;
    // long times[5];
    t_shared_data *s_data = (t_shared_data *)data;
    
    //set priorities
    set_priority(s_data->coder);

    request_dongle(s_data->coder,s_data->coder->left,s_data->args->scheduler);
    request_dongle(s_data->coder,s_data->coder->right,s_data->args->scheduler);
    
    pthread_mutex_lock(s_data->mutex_display);
    printf("coder %d and its priority %d\n",s_data->coder->coder_id,s_data->coder->priority_for_fifo);
    printf("left :[");
    debugging_hh(s_data->coder->left);
    printf("]\n");
    printf("right :[");
    debugging_hh(s_data->coder->right);
    printf("]\n");  
    pthread_mutex_unlock(s_data->mutex_display);
    
    // while(s_data->coder->priority_for_fifo > 1)
    //     pthread_cond_wait(s_data->main_cond,s_data->main_mutex);


    

    // pthread_mutex_unlock(s_data->mutex_display);
    
    // pthread_mutex_unlock(s_data->main_mutex);



    return NULL;

}