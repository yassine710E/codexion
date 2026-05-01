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
    // struct timeval tv;
    // gettimeofday(&tv, NULL);
    if(!data)
        return NULL;
    t_shared_data *s_data = (t_shared_data *)data;
    request_dongle(s_data->coder,s_data->coder->left,s_data->mutex_display,s_data->cond_display);
    request_dongle(s_data->coder,s_data->coder->right,s_data->mutex_display,s_data->cond_display);

    t_coder *queue_left = (t_coder *)s_data->coder->left->m_heap->queue;
    t_coder *queue_right = (t_coder *)s_data->coder->right->m_heap->queue;
    pthread_mutex_lock(s_data->coder->mutex_coder);
    while (s_data->coder->coder_id != queue_left[0].coder_id || s_data->coder->coder_id != queue_right[0].coder_id){
        pthread_cond_wait(s_data->coder->cond_coder,s_data->coder->mutex_coder);
        printf("coder %d should wait\n",s_data->coder->coder_id);
        queue_left = (t_coder *)s_data->coder->left->m_heap->queue;
        queue_right = (t_coder *)s_data->coder->right->m_heap->queue;

    }
    
    pthread_mutex_unlock(s_data->coder->mutex_coder);
    
    // if (s_data->coder->coder_id == queue_left[0].coder_id && s_data->coder->coder_id == queue_right[0].coder_id)
    // {
    compile(s_data->args->time_to_compile*1000);
    usleep(s_data->args->dongle_cooldown);

    pthread_mutex_lock(s_data->coder->mutex_coder);
    
    printf("coder %d is compiled and leave dongle\n",s_data->coder->coder_id);
    
    pthread_mutex_lock(s_data->coder->left->mutex_dongle);
        pop(s_data->coder->left->m_heap);
        push(s_data->coder->left->m_heap,s_data->coder);
    pthread_mutex_unlock(s_data->coder->left->mutex_dongle);

    pthread_mutex_lock(s_data->coder->right->mutex_dongle);
        pop(s_data->coder->right->m_heap);
        push(s_data->coder->right->m_heap,s_data->coder);
    pthread_mutex_unlock(s_data->coder->right->mutex_dongle);

    printf("[");
    debugging_hh(s_data->coder->left);
    printf("]\n");
    printf("[");
    debugging_hh(s_data->coder->right);
    printf("]\n");

    pthread_cond_broadcast(s_data->coder->cond_coder);

    pthread_mutex_unlock(s_data->coder->mutex_coder);
    // }

    return NULL;
}

