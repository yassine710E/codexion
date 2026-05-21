#include "codexion.h"

static void release_dongle_if_coder_takes_one_dongle(t_shared_data *s_data)
{
    t_coder *coder = s_data->coder;
    t_coder *left = (t_coder *)coder->left->m_heap->queue;
    t_coder *right = (t_coder *)coder->right->m_heap->queue;
    if (!(coder->left->m_heap->size && coder->coder_id == left[0].coder_id && coder->right->m_heap->size && coder->coder_id == right[0].coder_id))
    {
        if(coder->left->m_heap->size && coder->coder_id == left[0].coder_id)
            leave_dongle(coder->left);
        else if (coder->right->m_heap->size && coder->coder_id == right[0].coder_id)
            leave_dongle(coder->right);
        set_deadline(s_data->args->scheduler,coder,s_data->start,s_data->args->time_to_burnout); 
    }
}


static void wait_for_dongle_cooldown(t_shared_data *s_data,t_dongle *left,t_dongle *right)
{
    long lcs;
    if(left->last_compile_start_saver > right->last_compile_start_saver)
        lcs = left->last_compile_start_saver;
    else
        lcs = right->last_compile_start_saver;
    while(1)
    {
        if(get_timestamp_ms(s_data->start) >= lcs  + s_data->args->time_to_compile + s_data->args->dongle_cooldown)
            break;
    }
}

static int wait_for_waiter_coders(t_shared_data *s_data)
{
    t_coder *coder = s_data->coder;
    t_coder *left = (t_coder *)coder->left->m_heap->queue;
    t_coder *right = (t_coder *)coder->right->m_heap->queue;
    int left_id;
    int right_id;
    pthread_mutex_lock(s_data->main_mutex);
    while(1)
    {
        if(s_data->args->number_of_coders > 1)
        {
            pthread_mutex_lock(coder->left->mutex_dongle);
                left_id = left[0].coder_id;
            pthread_mutex_unlock(coder->left->mutex_dongle);


            pthread_mutex_lock(coder->right->mutex_dongle);
                right_id = right[0].coder_id;
            pthread_mutex_unlock(coder->right->mutex_dongle);
        }
        else
        {
            left_id = coder->coder_id + 1;
            right_id = left_id;
        }
        
        if(coder->coder_id == left_id && coder->coder_id == right_id)
                break;
        
        coder->is_waiting = 1;
        pthread_cond_wait(s_data->main_cond , s_data->main_mutex);
        
        if(*s_data->flag_burnout)
        {
            pthread_mutex_unlock(s_data->main_mutex);
            return 0;
        }
        
        if(!is_coder_exist_in_queue(coder->left->m_heap,coder->coder_id,coder->left->mutex_dongle))
               request_dongle(coder,coder->left);
        if(!is_coder_exist_in_queue(coder->right->m_heap,coder->coder_id,coder->right->mutex_dongle))
               request_dongle(coder,coder->right);
        wait_for_dongle_cooldown(s_data,coder->left,coder->right);

    }
    coder->is_waiting = 0;
    pthread_mutex_unlock(s_data->main_mutex);
    return 1;
}




void *coder_routine (void *data)
{
    if(!data)
        return NULL;
    t_shared_data *s_data = (t_shared_data *)data;
    t_coder *coder = s_data->coder;
    int coder_id;
    int condition;
    while (1)
    {
        
        pthread_mutex_lock(s_data->main_mutex);
            condition = coder->count_compiled < s_data->args->number_of_compiles_required && !*s_data->flag_burnout;
            coder_id = coder->coder_id;
        pthread_mutex_unlock(s_data->main_mutex);
        if(!condition)
            break;

        //set deadline here !!   
        set_deadline(s_data->args->scheduler,coder,s_data->start,s_data->args->time_to_burnout); 
        
        if(s_data->args->number_of_coders > 1)
        {
            request_dongle(coder,coder->left);
            request_dongle(coder,coder->right);
        }
        
        release_dongle_if_coder_takes_one_dongle(s_data);


        if(!wait_for_waiter_coders(s_data))
            break;
        
        //coder takes two dongles
        logs(s_data->mutex_display,"has taken a dongle",get_timestamp_ms(s_data->start),coder_id);
        logs(s_data->mutex_display,"has taken a dongle",get_timestamp_ms(s_data->start),coder_id);

        //compiling
        pthread_mutex_lock(s_data->main_mutex);
            coder->last_compile_start  = get_timestamp_ms(s_data->start);
        pthread_mutex_unlock(s_data->main_mutex);
    
        if(!sleep_for_operation(s_data->args->time_to_compile,s_data,coder->last_compile_start,"is compiling"))
            break;
        
        //leave dongles
        leave_dongle(coder->left);
        leave_dongle(coder->right);
        
        //increment count compiled
        pthread_mutex_lock(s_data->main_mutex);
            coder->count_compiled ++;
        pthread_mutex_unlock(s_data->main_mutex);
        
        //save last compile start in left dongle
        pthread_mutex_lock(coder->left->mutex_dongle);
           coder->left->last_compile_start_saver = coder->last_compile_start;
        pthread_mutex_unlock(coder->left->mutex_dongle);

        //save last compile start in right dongle
        pthread_mutex_lock(coder->right->mutex_dongle);
            coder->right->last_compile_start_saver = coder->last_compile_start;
        pthread_mutex_unlock(coder->right->mutex_dongle);

        //broadcast for other coders
        broadcast_other_coders(s_data); 
    
        if(!sleep_for_operation(s_data->args->time_to_debug,s_data,get_timestamp_ms(s_data->start),"is debugging"))
            break;
    
        if(!sleep_for_operation(s_data->args->time_to_refactor,s_data,get_timestamp_ms(s_data->start),"is refactoring"))
            break;
         
    }
    pthread_mutex_lock(s_data->main_mutex);
        *s_data->how_many_coders_finished += 1;        
    pthread_mutex_unlock(s_data->main_mutex);
    return NULL;
}

void *routine_monitor(void *data)
{
    if(!data)
        return NULL;
    t_shared_data *s_data = (t_shared_data *)data;
    t_coder *arr_coders = s_data->coder;
    (void)s_data;
    (void)arr_coders;
    int condition;
    int condition_2;
    int condition_3;
    (void)condition_3;
    int i;
    long now;
    while (1)
    {
        // this block of code is for loop condition and data race protection
        pthread_mutex_lock(s_data->main_mutex);
            condition = *s_data->how_many_coders_finished < s_data->args->number_of_coders && !*s_data->flag_burnout;
        pthread_mutex_unlock(s_data->main_mutex);
        if(!condition)
            break;
        // end of block this is like header condition
        
        i = -1;
        while (1)
        {
            // this block of code is for loop condition and data race protection
            pthread_mutex_lock(s_data->main_mutex);
                condition_2 = ++i < s_data->args->number_of_coders;
            pthread_mutex_unlock(s_data->main_mutex);
            if(!condition_2)
                break;
            // end of block this is like header condition
            
            
            pthread_mutex_lock(s_data->main_mutex);
                now = get_timestamp_ms(s_data->start);
                condition_3 = now - arr_coders[i].last_compile_start >= s_data->args->time_to_burnout
                    && arr_coders[i].count_compiled < s_data->args->number_of_compiles_required ;
                if(condition_3)
                   *s_data->flag_burnout = 1; 
            pthread_mutex_unlock(s_data->main_mutex);

            if(condition_3)
            {
                usleep(2000);
                broadcast_other_coders(s_data);
                pthread_mutex_lock(s_data->mutex_display);
                    printf("%ld %d burned out\n",now,arr_coders[i].coder_id);
                pthread_mutex_unlock(s_data->mutex_display);            
                return NULL;
            }
        }
        usleep(500);
    }
    return NULL;
}








// #include "codexion.h"

// /**
//  * Checks if the coder safely sits at the top of BOTH the left and right 
//  * dongle priority queues. Every read is protected by the dongle's mutex.
//  */
// static int holds_both_dongles(t_shared_data *s_data, t_coder *coder)
// {
//     int owns_left = 0;
//     int owns_right = 0;

//     if (s_data->args->number_of_coders == 1)
//     {
//         pthread_mutex_lock(coder->left->mutex_dongle);
//         if (coder->left->m_heap->size > 0)
//         {
//             t_coder *left_queue = (t_coder *)coder->left->m_heap->queue;
//             if (left_queue[0].coder_id == coder->coder_id)
//                 owns_left = 1;
//         }
//         pthread_mutex_unlock(coder->left->mutex_dongle);
//         return (owns_left);
//     }

//     // Check Left Dongle
//     pthread_mutex_lock(coder->left->mutex_dongle);
//     if (coder->left->m_heap->size > 0)
//     {
//         t_coder *left_queue = (t_coder *)coder->left->m_heap->queue;
//         if (left_queue[0].coder_id == coder->coder_id)
//             owns_left = 1;
//     }
//     pthread_mutex_unlock(coder->left->mutex_dongle);

//     // Check Right Dongle
//     pthread_mutex_lock(coder->right->mutex_dongle);
//     if (coder->right->m_heap->size > 0)
//     {
//         t_coder *right_queue = (t_coder *)coder->right->m_heap->queue;
//         if (right_queue[0].coder_id == coder->coder_id)
//             owns_right = 1;
//     }
//     pthread_mutex_unlock(coder->right->mutex_dongle);

//     return (owns_left && owns_right);
// }

// /**
//  * Throttles the thread using a precise sleep if a dongle is still in its
//  * mandatory cooldown phase, preventing 100% CPU usage spinning.
//  */
// static void handle_dongle_cooldown(t_shared_data *s_data, t_dongle *left, t_dongle *right)
// {
//     long last_release = (left->last_compile_start_saver > right->last_compile_start_saver) 
//                         ? left->last_compile_start_saver 
//                         : right->last_compile_start_saver;
    
//     long target_time = last_release + s_data->args->time_to_compile + s_data->args->dongle_cooldown;
//     long current_time = get_timestamp_ms(s_data->start);
    
//     if (current_time < target_time)
//     {
//         usleep((target_time - current_time) * 1000);
//     }
// }

// static int wait_for_waiter_coders(t_shared_data *s_data)
// {
//     t_coder *coder = s_data->coder;

//     pthread_mutex_lock(s_data->main_mutex);
//     while (!holds_both_dongles(s_data, coder))
//     {
//         coder->is_waiting = 1;
//         pthread_cond_wait(s_data->main_cond, s_data->main_mutex);
        
//         if (*s_data->flag_burnout)
//         {
//             pthread_mutex_unlock(s_data->main_mutex);
//             return 0;
//         }
        
//         // Re-verify heap presence safely if woken up
//         int missing_left = !is_coder_exist_in_queue(coder->left->m_heap, coder->coder_id, coder->left->mutex_dongle);
//         if (missing_left)
//             request_dongle(coder, coder->left);

//         if (s_data->args->number_of_coders > 1)
//         {
//             int missing_right = !is_coder_exist_in_queue(coder->right->m_heap, coder->coder_id, coder->right->mutex_dongle);
//             if (missing_right)
//                 request_dongle(coder, coder->right);
//         }
        
//         handle_dongle_cooldown(s_data, coder->left, coder->right);
//     }
//     coder->is_waiting = 0;
//     pthread_mutex_unlock(s_data->main_mutex);
//     return 1;
// }

// void *coder_routine(void *data)
// {
//     if (!data)
//         return NULL;
//     t_shared_data *s_data = (t_shared_data *)data;
//     t_coder *coder = s_data->coder;
//     int condition;

//     while (1)
//     {
//         pthread_mutex_lock(s_data->main_mutex);
//         condition = (coder->count_compiled < s_data->args->number_of_compiles_required) && !*s_data->flag_burnout;
//         pthread_mutex_unlock(s_data->main_mutex);
        
//         if (!condition)
//             break;

//         set_deadline(s_data->args->scheduler, coder, s_data->start, s_data->args->time_to_burnout); 
        
//         // Push requests into the priority queues
//         request_dongle(coder, coder->left);
//         if (s_data->args->number_of_coders > 1)
//             request_dongle(coder, coder->right);

//         if (!wait_for_waiter_coders(s_data))
//             break;
        
//         // Coder physically holds both dongles now. Serialize logs sequentially.
//         logs(s_data->mutex_display, "has taken a dongle", get_timestamp_ms(s_data->start), coder->coder_id);
//         if (s_data->args->number_of_coders > 1)
//             logs(s_data->mutex_display, "has taken a dongle", get_timestamp_ms(s_data->start), coder->coder_id);

//         pthread_mutex_lock(s_data->main_mutex);
//         coder->last_compile_start = get_timestamp_ms(s_data->start);
//         pthread_mutex_unlock(s_data->main_mutex);
    
//         if (!sleep_for_operation(s_data->args->time_to_compile, s_data, coder->last_compile_start, "is compiling"))
//             break;
        
//         // Release dongles back to other threads
//         leave_dongle(coder->left);
//         if (s_data->args->number_of_coders > 1)
//             leave_dongle(coder->right);
        
//         pthread_mutex_lock(s_data->main_mutex);
//         coder->count_compiled++;
//         pthread_mutex_unlock(s_data->main_mutex);
        
//         // Save resource tracking metrics safely
//         pthread_mutex_lock(coder->left->mutex_dongle);
//         coder->left->last_compile_start_saver = coder->last_compile_start;
//         pthread_mutex_unlock(coder->left->mutex_dongle);

//         if (s_data->args->number_of_coders > 1)
//         {
//             pthread_mutex_lock(coder->right->mutex_dongle);
//             coder->right->last_compile_start_saver = coder->last_compile_start;
//             pthread_mutex_unlock(coder->right->mutex_dongle);
//         }

//         broadcast_other_coders(s_data); 
    
//         if (!sleep_for_operation(s_data->args->time_to_debug, s_data, get_timestamp_ms(s_data->start), "is debugging"))
//             break;
    
//         if (!sleep_for_operation(s_data->args->time_to_refactor, s_data, get_timestamp_ms(s_data->start), "is refactoring"))
//             break;
//     }

//     pthread_mutex_lock(s_data->main_mutex);
//     *s_data->how_many_coders_finished += 1;        
//     pthread_mutex_unlock(s_data->main_mutex);
//     return NULL;
// }

// void *routine_monitor(void *data)
// {
//     if (!data)
//         return NULL;
//     t_shared_data *s_data = (t_shared_data *)data;
//     t_coder *arr_coders = s_data->coder;
//     int i;
//     long now;

//     while (1)
//     {
//         pthread_mutex_lock(s_data->main_mutex);
//         if (*s_data->how_many_coders_finished >= s_data->args->number_of_coders || *s_data->flag_burnout)
//         {
//             pthread_mutex_unlock(s_data->main_mutex);
//             break;
//         }
//         pthread_mutex_unlock(s_data->main_mutex);
        
//         for (i = 0; i < s_data->args->number_of_coders; i++)
//         {
//             pthread_mutex_lock(s_data->main_mutex);
//             now = get_timestamp_ms(s_data->start);
            
//             if (now - arr_coders[i].last_compile_start >= s_data->args->time_to_burnout &&
//                 arr_coders[i].count_compiled < s_data->args->number_of_compiles_required)
//             {
//                 *s_data->flag_burnout = 1;
//                 pthread_mutex_unlock(s_data->main_mutex);

//                 pthread_mutex_lock(s_data->mutex_display);
//                 printf("%ld %d burned out\n", now, arr_coders[i].coder_id);
//                 pthread_mutex_unlock(s_data->mutex_display);            
                
//                 broadcast_other_coders(s_data);
//                 return NULL;
//             }
//             pthread_mutex_unlock(s_data->main_mutex);
//         }
//         usleep(1000); 
//     }
//     return NULL;
// }


