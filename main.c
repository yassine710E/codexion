#include "coders/codexion.h"

pthread_mutex_t mutex_1;
pthread_cond_t cond_1;

void compile(unsigned int time_to_compile)
{
    usleep(time_to_compile);
}

void debug(unsigned int time_to_debug)
{
    usleep(time_to_debug);
}

void refactor(unsigned int time_to_refactor)
{
    usleep(time_to_refactor);
}



void *routine(void *data)
{
    t_shared_data *s_data = (t_shared_data *)data;
    unsigned int count_compiles = 0;
    // while(count_compiles < s_data->args->number_of_compiles_required)
    // {
    if (s_data->start_index_list >= 0 && !strcmp((s_data->queue_coders)->state_coder,"compile_step"))
    {
        printf("(%s) coder %d work with left dongle %d and right dongle %d\n",(s_data->queue_coders)->state_coder,((s_data->queue_coders))->coder_id,
        get_dongle(s_data->start_index_list,s_data->list_dongles)->dongle_id,get_dongle(s_data->start_index_list+1,s_data->list_dongles)->dongle_id);
        compile(s_data->args->time_to_compile*1000);
        count_compiles++;
        usleep(s_data->args->dongle_cooldown*1000);
        s_data->start_index_list = -1;
        (s_data->queue_coders)->state_coder = "debug_and_refactor_step";
        //rotate
        if (s_data->args->number_of_coders % 2 == 0)
            rotate(&(s_data->list_dongles));
        else
            rrotate(&(s_data->list_dongles));
        //fifo
        
        


    }
    else if(!strcmp((s_data->queue_coders)->state_coder,"to_do"))
    {
        printf("sleep the process less than burnout deadline ...\n");
        usleep(s_data->args->time_to_burnout - 1);
        s_data->start_index_list = 0;
        (s_data->queue_coders)->state_coder = "compile_step";





    }
    else if(!strcmp((s_data->queue_coders)->state_coder,"debug_and_refactor_step"))
    {
        printf("(%s) for coder %d\n",(s_data->queue_coders)->state_coder,(s_data->queue_coders)->coder_id);
        debug(s_data->args->time_to_debug * 1000);
        refactor(s_data->args->time_to_refactor * 1000);
        s_data->start_index_list = 0;
        (s_data->queue_coders)->state_coder = "compile_step";
    }
// }
    return NULL;
}



int main(int c,char **v)
{
    pthread_mutex_init(&mutex_1,NULL);
    t_args *args = malloc(sizeof(t_args));
    if(!args)
        return 1;
    if(!parsing(c,v,args))
    {
        free(args);
        return 1;
    }
    // pthread_t arr[args->number_of_coders];
    t_shared_data *s_data = malloc(sizeof(t_shared_data) * args->number_of_coders);
    t_coder *coders = malloc(sizeof(t_coder) * args->number_of_coders);
    t_dongle *dongles = malloc(sizeof(t_dongle));
    if(!coders || !dongles || !s_data)
        return 1;
    set_coders(coders,args->number_of_coders);
    set_dongles(dongles,args->number_of_coders);
    int i = -1;
    int arr_indexes_coders[args->number_of_coders];
    int arr_indexes_dongles[args->number_of_coders / 2];
    //set array of index queue and start dongle list index of each queue index
    //like : 
    //[0,1,2,3,4] : index queue [2,3,4]
    //[0,2] ==> [0,1,2,3,4] : [4,0,1,2,3]
    int index_d = 0;
    while (++i < (int)args->number_of_coders){
        arr_indexes_coders[i] = i;
        printf("%d\n",arr_indexes_coders[i]);
    }

    i = -1;
    printf("$$$$$$$$$$$$$$$\n");
    while (++i < (int)(args->number_of_coders / 2))
    {
        arr_indexes_dongles[i] = index_d;
        printf("%d\n",arr_indexes_dongles[i]);
        index_d += 2; 
    }
    printf("$$$$$$$$$$$$$$$\n");    
    
    i = -1;
    while (++i < (int)args->number_of_coders)
    {
        set_shared_data(s_data+i,coders,dongles,args);
        if(i < (int)(args->number_of_coders / 2))
        {
            (s_data+i)->start_index_list = arr_indexes_dongles[i];
        }
        else{
            (s_data+i)->start_index_list = -1;
        }
        // pthread_create (arr+i,NULL,routine,s_data+i);
        printf("%d\n",(s_data+i)->start_index_list);
    }
    i = -1;
    // while (++i < (int)args->number_of_coders)
    //     pthread_join(arr[i],NULL);    
    
    free_pointer(coders);
    free_list(dongles);
    free_pointer(args); 
    free_pointer(s_data);
    pthread_mutex_destroy(&mutex_1);


    return 0;
}