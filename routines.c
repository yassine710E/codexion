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

void release_dongle_and_broadcast_others(t_coder *coder,t_shared_data *s_data)
{
    t_coder *left = (t_coder *)coder->left->m_heap->queue;
    t_coder *right = (t_coder *)coder->right->m_heap->queue;
    int is_left = 0;
    if(coder->left->m_heap->size && coder->coder_id == left[0].coder_id)
    {
        is_left = 1;
        leave_dongle(coder->left);
    }
    else if (coder->right->m_heap->size && coder->coder_id == right[0].coder_id)
        leave_dongle(coder->right);
    
    set_deadline(s_data->args->scheduler,coder,s_data->start,s_data->args->time_to_burnout);
    
    if(is_left)
        request_dongle(coder,coder->left);
    else
        request_dongle(coder,coder->right);

    //broadcast for other coders
    pthread_mutex_lock(s_data->main_mutex);
        printf("coder %d \n",coder->coder_id);
        printf("left : [");
        debugging_hh(coder->left);
        printf("]\n");
        printf("right : [");
        debugging_hh(coder->right);
        printf("]\n");

    pthread_cond_broadcast(s_data->main_cond);
    pthread_mutex_unlock(s_data->main_mutex);

}

int get_wait_condition(t_coder *coder,int *flag_is_step_compiling_finished)
{
    t_coder *left = (t_coder *)coder->left->m_heap->queue;
    t_coder *right = (t_coder *)coder->right->m_heap->queue;

    if(! *flag_is_step_compiling_finished)
       return coder->coder_id != left[0].coder_id && coder->coder_id != right[0].coder_id; 

    return coder->coder_id != left[0].coder_id || coder->coder_id != right[0].coder_id;
}

void *coder_routine (void *data)
{
    if(!data)
        return NULL;
    t_shared_data *s_data = (t_shared_data *)data;
    t_coder *coder = s_data->coder;
    
    t_coder *left = (t_coder *)coder->left->m_heap->queue;
    t_coder *right = (t_coder *)coder->right->m_heap->queue;
    // long time_for_logs[5];
    
    
    //set deadline here !!   
    set_deadline(s_data->args->scheduler,coder,s_data->start,s_data->args->time_to_burnout); 
    
    // while(coder->count_compiled < s_data->args->number_of_compiles_required)
    request_dongle(coder,coder->left);
    request_dongle(coder,coder->right);
    
    pthread_mutex_lock(s_data->main_mutex);
    while(coder->coder_id != left[0].coder_id && coder->coder_id != right[0].coder_id)
        pthread_cond_wait(s_data->main_cond , s_data->main_mutex);
    pthread_mutex_unlock(s_data->main_mutex);
    


    if(coder->coder_id == left[0].coder_id && coder->coder_id == right[0].coder_id)
    {


    }
    else
        release_dongle_and_broadcast_others(coder,s_data);
        
    return NULL;

}







// while(1)
// {
//     if(get_timestamp_ms(s_data->start) > *s_data->last_compile_start_for_on_coders + s_data->args->time_to_compile + s_data->args->dongle_cooldown)
//         break;
// }


// printf("coder %d and its deadline %ld\n",coder->coder_id,coder->deadline);
// printf("left:[");
// debugging_hh(coder->left);
// printf("]\n");
// printf("right:[");
// debugging_hh(coder->right);
// printf("]\n\n");




//process of coder operation 
        // //coder takes two dongles
        // time_for_logs[0] = get_timestamp_ms(s_data->start);
        // time_for_logs[1] = get_timestamp_ms(s_data->start);
     
        // // //compiling
        // time_for_logs[2] = get_timestamp_ms(s_data->start);
        // compile(s_data->args->time_to_compile * 1000);
        // coder->last_compile_start = time_for_logs[2];
     
        // //leave dongles
        // leave_dongle(coder->left);
        // leave_dongle(coder->right);
         
        // //increment count compiled
        // coder->count_compiled++;
     
        // //broadcast for other coders
        // pthread_mutex_lock(s_data->main_mutex);
        //     *s_data->flag_is_step_compiling_finished = 1;
        //     pthread_cond_broadcast(s_data->main_cond);
        // pthread_mutex_unlock(s_data->main_mutex);
         
        // time_for_logs[3] = get_timestamp_ms(s_data->start);
        // debugging(s_data->args->time_to_debug * 1000);
     
        // time_for_logs[4] = get_timestamp_ms(s_data->start);
        // refactoring(s_data->args->time_to_refactor * 1000);
         
        //  pthread_mutex_lock(s_data->main_mutex);
        //      printf("%ld %d has taken a dongle\n",time_for_logs[0],coder->coder_id);
        //      printf("%ld %d has taken a dongle\n",time_for_logs[1],coder->coder_id);
        //      printf("%ld %d is compiling\n",time_for_logs[2],coder->coder_id);
        //      printf("%ld %d is debugging\n",time_for_logs[3],coder->coder_id);
        //      printf("%ld %d is refactoring\n\n",time_for_logs[4],coder->coder_id);
        //  pthread_mutex_unlock(s_data->main_mutex);