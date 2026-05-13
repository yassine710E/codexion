#include "coders/codexion.h"

static void release_dongle_if_coder_takes_one_dongle(t_shared_data *s_data)
{
    t_coder *coder = s_data->coder;
    t_coder *left = (t_coder *)coder->left->m_heap->queue;
    t_coder *right = (t_coder *)coder->right->m_heap->queue;
    if (!(coder->coder_id == left[0].coder_id && coder->coder_id == right[0].coder_id))
    {
        if(coder->left->m_heap->size && coder->coder_id == left[0].coder_id)
            leave_dongle(coder->left);
        else if (coder->right->m_heap->size && coder->coder_id == right[0].coder_id)
            leave_dongle(coder->right);
        set_deadline(s_data->args->scheduler,coder,s_data->start,s_data->args->time_to_burnout); 
    }
}


// static void wait_for_dongle_cooldown(t_shared_data *s_data)
// {
//     while(1)
//     {
//         if(get_timestamp_ms(s_data->start) > s_data->coder->last_compile_start + s_data->args->time_to_compile + s_data->args->dongle_cooldown)
//             break;
//     }
// }

static void wait_for_waiter_coders(t_shared_data *s_data)
{
    t_coder *coder = s_data->coder;
    t_coder *left = (t_coder *)coder->left->m_heap->queue;
    t_coder *right = (t_coder *)coder->right->m_heap->queue;
    pthread_mutex_lock(s_data->main_mutex);
    
    while(coder->coder_id != left[0].coder_id || coder->coder_id != right[0].coder_id)
    {
        pthread_cond_wait(s_data->main_cond , s_data->main_mutex);
        if(!is_coder_exist_in_queue(coder->left->m_heap,coder->coder_id))
               request_dongle(coder,coder->left);
        if(!is_coder_exist_in_queue(coder->right->m_heap,coder->coder_id))
               request_dongle(coder,coder->right);
        // wait_for_dongle_cooldown(s_data);
        // printf("coder %d awake and last compile start %ld\n",coder->coder_id,coder->last_compile_start);
        // printf("left : [");
        //     debugging_hh(coder->left);
        // printf("]\n");
        // printf("right : [");
        //     debugging_hh(coder->right);
        // printf("]\n");

    }
    
    pthread_mutex_unlock(s_data->main_mutex);
}

static void broadcast_other_coders(t_shared_data *s_data)
{
    pthread_mutex_lock(s_data->main_mutex);
        pthread_cond_broadcast(s_data->main_cond);
    pthread_mutex_unlock(s_data->main_mutex);
}


void *coder_routine (void *data)
{
    if(!data)
        return NULL;
    t_shared_data *s_data = (t_shared_data *)data;
    t_coder *coder = s_data->coder;
        
    
    //set deadline here !!   
    set_deadline(s_data->args->scheduler,coder,s_data->start,s_data->args->time_to_burnout); 
    
    while (coder->count_compiled < s_data->args->number_of_compiles_required)
    {
        request_dongle(coder,coder->left);
        request_dongle(coder,coder->right);
        
        release_dongle_if_coder_takes_one_dongle(s_data);
        
        wait_for_waiter_coders(s_data);
        
        //coder takes two dongles
        logs(s_data->mutex_display,"has taken a dongle",get_timestamp_ms(s_data->start),coder->coder_id);
        logs(s_data->mutex_display,"has taken a dongle",get_timestamp_ms(s_data->start),coder->coder_id);

    
        // //compiling
        coder->last_compile_start  = get_timestamp_ms(s_data->start);
        logs(s_data->mutex_display,"is compiling",coder->last_compile_start,coder->coder_id);
        compile(s_data->args->time_to_compile);
    
        //leave dongles
        leave_dongle(coder->left);
        leave_dongle(coder->right);
     
        //increment count compiled
        coder->count_compiled++;
    
        //broadcast for other coders
        broadcast_other_coders(s_data); 
    
        logs(s_data->mutex_display,"is debugging",get_timestamp_ms(s_data->start),coder->coder_id);
        debugging(s_data->args->time_to_debug);
    
        logs(s_data->mutex_display,"is refactoring",get_timestamp_ms(s_data->start),coder->coder_id);
        refactoring(s_data->args->time_to_refactor);
         
    }


    return NULL;

}














//process of coder operation 


