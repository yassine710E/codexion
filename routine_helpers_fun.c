#include "coders/codexion.h"
void debugging_hh(t_dongle *dongle)
{   
    int i = 0;
    t_coder *coders_queue = (t_coder *)dongle->m_heap->queue;
    while (i < dongle->m_heap->size)
        printf("%d ",coders_queue[i++].coder_id);
}

void request_dongle(t_coder *coder,t_dongle *dongle)
{
    pthread_mutex_lock(dongle->mutex_dongle);
        push(dongle->m_heap,coder);    
    pthread_mutex_unlock(dongle->mutex_dongle);
}

void leave_dongle (t_dongle *dongle)
{
    pthread_mutex_lock(dongle->mutex_dongle);
        pop(dongle->m_heap);
    pthread_mutex_unlock(dongle->mutex_dongle);
}

void logs(pthread_mutex_t *display_mutex,char *message,long current_time,int coder_id)
{
    pthread_mutex_lock(display_mutex);
        printf("%ld %d %s\n",current_time,coder_id,message);
    pthread_mutex_unlock(display_mutex);
}

void set_deadline(char *scheduler,t_coder *coder,struct timeval start,TIME t_to_burnout)
{
    if(!strcmp("fifo",scheduler))
        coder->deadline = get_timestamp_ms(start);
    else
        coder->deadline = coder->last_compile_start + t_to_burnout;    
}

int is_coder_exist_in_queue(min_heap *m_heap,int coder_id)
{   
    int i = 0;
    t_coder *arr_coders = (t_coder *)m_heap->queue;
    while (i < m_heap->size)
    {
        if(coder_id == arr_coders[i].coder_id)
            return 1;
        i++;
    }
    return 0;
}

void broadcast_other_coders(t_shared_data *s_data)
{
    pthread_mutex_lock(s_data->main_mutex);
        pthread_cond_broadcast(s_data->main_cond);
    pthread_mutex_unlock(s_data->main_mutex);
}