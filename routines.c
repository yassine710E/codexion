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





void *coder_routine (void *data)
{
    if(!data)
        return NULL;
    t_shared_data *s_data = (t_shared_data *)data;
    t_coder *coder = s_data->coder;
    
    // t_coder *left = (t_coder *)coder->left->m_heap->queue;
    // t_coder *right = (t_coder *)coder->right->m_heap->queue;

    if(!strcmp("fifo",s_data->args->scheduler))
        coder->deadline = get_timestamp_ms(s_data->start);
    else
        coder->deadline = coder->last_compile_start + s_data->args->time_to_burnout;        
    
    request_dongle(coder,coder->left);
    request_dongle(coder,coder->right);
    
    // pthread_mutex_lock(s_data->main_mutex);
    // while(coder->coder_id != left[0].coder_id || coder->coder_id != right[0].coder_id)
    // {
    //     pthread_cond_wait(s_data->main_cond , s_data->main_mutex);
    //     while(1)
    //     {
    //         if(get_timestamp_ms(s_data->start) > *s_data->last_compile_start_for_on_coders + s_data->args->time_to_compile + s_data->args->dongle_cooldown)
    //             break;
    //     }

    // }
    // pthread_mutex_unlock(s_data->main_mutex);
    
    //coder takes two dongles
    // logs(s_data->mutex_display,"has taken a dongle",get_timestamp_ms(s_data->start),coder->coder_id);
//     logs(s_data->mutex_display,"has taken a dongle",get_timestamp_ms(s_data->start),coder->coder_id);

    // time_for_logs[0] = get_timestamp_ms(s_data->start);
    // time_for_logs[1] = get_timestamp_ms(s_data->start);

    //compiling
    // time_for_logs[2] = get_timestamp_ms(s_data->start);
    // compile(s_data->args->time_to_compile * 1000);
    // long tmp_last_compile = get_timestamp_ms(s_data->start);
    // // logs(s_data->mutex_display,"is compiling",tmp_last_compile,coder->coder_id);

    
    // leave_dongle(coder->left);
    // leave_dongle(coder->right);
    // coder->count_compiled++;

    // pthread_mutex_lock(s_data->main_mutex);
    //     *s_data->last_compile_start_for_on_coders = tmp_last_compile;
    //     pthread_cond_broadcast(s_data->main_cond);
    // pthread_mutex_unlock(s_data->main_mutex);
    
    // // time_for_logs[3] = get_timestamp_ms(s_data->start);
    // debugging(s_data->args->time_to_debug * 1000);
    // // logs(s_data->mutex_display,"is debugging",get_timestamp_ms(s_data->start),coder->coder_id);

    // // time_for_logs[4] = get_timestamp_ms(s_data->start);
    // refactoring(s_data->args->time_to_refactor * 1000);
    // // logs(s_data->mutex_display,"is refactoring",get_timestamp_ms(s_data->start),coder->coder_id);
    
    // pthread_mutex_lock(s_data->mutex_display);
    //     printf("%ld %d has taken a dongle\n",time_for_logs[0],coder->coder_id);
    //     printf("%ld %d has taken a dongle\n",time_for_logs[1],coder->coder_id);
    //     printf("%ld %d is compiling\n",time_for_logs[2],coder->coder_id);
    //     printf("%ld %d is debugging\n",time_for_logs[3],coder->coder_id);
    //     printf("%ld %d is refactoring\n\n",time_for_logs[4],coder->coder_id);
    // pthread_mutex_unlock(s_data->mutex_display);
    
    pthread_mutex_lock(s_data->mutex_display);
    printf("coder %d and its priority %ld\n",s_data->coder->coder_id,s_data->coder->deadline);
    printf("left :[");
    debugging_hh(s_data->coder->left);
    printf("]\n");
    printf("right :[");
    debugging_hh(s_data->coder->right);
    printf("]\n");  
    pthread_mutex_unlock(s_data->mutex_display);



    


    return NULL;

}


