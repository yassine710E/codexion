#include "coders/codexion.h"
void debugging_hh(t_dongle *dongle)
{   
    unsigned int i = 0;
    t_coder *coders_queue = (t_coder *)dongle->m_heap->queue;
    while (i < dongle->m_heap->size)
        printf("%d ",coders_queue[i++].coder_id);
}
void request_dongle(t_coder *coder,t_dongle *dongle,pthread_mutex_t *mutex_display,pthread_cond_t *cond_display)
{
    //coder request dongle
    (void)mutex_display;
    (void)cond_display;
    pthread_mutex_lock(dongle->mutex_dongle);
    push(dongle->m_heap,coder);    
    pthread_mutex_unlock(dongle->mutex_dongle);
}
void *coder_routine (void *data)
{
    if(!data)
        return NULL;
    t_shared_data *s_data = (t_shared_data *)data;
    long times[5];
    request_dongle(s_data->coder,s_data->coder->left,s_data->mutex_display,s_data->cond_display);
    request_dongle(s_data->coder,s_data->coder->right,s_data->mutex_display,s_data->cond_display);

    t_coder *queue_left = (t_coder *)s_data->coder->left->m_heap->queue;
    t_coder *queue_right = (t_coder *)s_data->coder->right->m_heap->queue;
    while(s_data->coder->count_compiled < s_data->args->number_of_compiles_required)
    {
    pthread_mutex_lock(s_data->main_mutex);
    while (s_data->coder->coder_id != queue_left[0].coder_id || s_data->coder->coder_id != queue_right[0].coder_id)
        pthread_cond_wait(s_data->main_cond,s_data->main_mutex);
    pthread_mutex_unlock(s_data->main_mutex);
    
    times[0] = get_timestamp_ms(s_data->start);
    times[1] = get_timestamp_ms(s_data->start);
    times[2] = get_timestamp_ms(s_data->start);
    compile(s_data->args->time_to_compile*1000);
    cooldown(s_data->args->dongle_cooldown*1000);
    pthread_mutex_lock(s_data->main_mutex);
    // printf("this is coder %d\n",s_data->coder->coder_id);
    // printf("left : [");
    // debugging_hh(s_data->coder->left);
    // printf("]\n");
    // printf("right : [");
    // debugging_hh(s_data->coder->right);
    // printf("]\n");

    s_data->coder->count_compiled++;
    
    pthread_mutex_lock(s_data->coder->left->mutex_dongle);
        pop(s_data->coder->left->m_heap);
        push(s_data->coder->left->m_heap,s_data->coder);
    pthread_mutex_unlock(s_data->coder->left->mutex_dongle);

    pthread_mutex_lock(s_data->coder->right->mutex_dongle);
        pop(s_data->coder->right->m_heap);
        push(s_data->coder->right->m_heap,s_data->coder);
    pthread_mutex_unlock(s_data->coder->right->mutex_dongle);

    pthread_cond_broadcast(s_data->main_cond);
    pthread_mutex_unlock(s_data->main_mutex);
    
    times[3] = get_timestamp_ms(s_data->start);
    debugging(s_data->args->time_to_debug*1000);
    times[4] = get_timestamp_ms(s_data->start); 
    refactoring(s_data->args->time_to_refactor*1000);
    
    pthread_mutex_lock(s_data->mutex_display);
        printf("%ld %d has taken a dongle\n",times[0],s_data->coder->coder_id);
        printf("%ld %d has taken a dongle\n",times[1],s_data->coder->coder_id);
        printf("%ld %d is compiling\n",times[2],s_data->coder->coder_id);
        printf("%ld %d is debugging\n",times[3],s_data->coder->coder_id);
        printf("%ld %d is refactoring\n",times[4],s_data->coder->coder_id);
    pthread_mutex_unlock(s_data->mutex_display);
    }
    return NULL;
}

