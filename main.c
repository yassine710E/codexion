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
    while(count_compiles < s_data->args->number_of_compiles_required)
    {
    if (s_data->start_index_list >= 0)
    {
        printf("coder %d , left dongle %d ,right dongle %d\n",((s_data->queue_coders)+s_data->index_queue)->coder_id,
        s_data->start_index_list+1,s_data->start_index_list+2);
        (s_data->queue_coders+s_data->index_queue)->state_coder = "compiling";
        compile(s_data->args->time_to_compile*1000);
        count_compiles++;
        s_data->start_index_list = -1;
    }
    else if(!strcmp((s_data->queue_coders+s_data->index_queue)->state_coder,"initialization"))
    {
        printf("sleep the process less than burnout deadline ...\n");
        usleep(s_data->args->time_to_burnout - 1);
        s_data->start_index_list = 0;

    }
    else
    {
        (s_data->queue_coders+s_data->index_queue)->state_coder = "debugging";
        debug(s_data->args->time_to_debug*1000);
        (s_data->queue_coders+s_data->index_queue)->state_coder = "refactoring";
        refactor(s_data->args->time_to_refactor*1000);
        s_data->start_index_list = 0;
        unsigned int i = (s_data->args->number_of_coders/2);
        while (i < (s_data->args->number_of_coders))
        {
            s_data->queue_coders[i - (s_data->args->number_of_coders/2)] = s_data->queue_coders[i];
            i++;
        }
        
        s_data->queue_coders = realloc(s_data->queue_coders,sizeof(t_coder) * ((s_data->args->number_of_coders)-(s_data->args->number_of_coders/2)));
        s_data->size_coders = s_data->args->number_of_coders / 2;


    }}
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
    pthread_t arr[args->number_of_coders];
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
    while (++i < (int)args->number_of_coders)
        arr_indexes_coders[i] = i;

    i = -1;
    while (++i < (int)(args->number_of_coders / 2))
    {
        arr_indexes_dongles[i] = index_d;
        index_d += 2; 
    }
    i = -1;
    while (++i < (int)args->number_of_coders)
    {
        set_shared_data(s_data+i,coders,dongles,args);
        (s_data+i)->index_queue = arr_indexes_coders[i];
        if(i < (int)(args->number_of_coders / 2))
        {
            (s_data+i)->start_index_list = arr_indexes_dongles[i];
        }
        else{
            (s_data+i)->start_index_list = -1;
        }
        pthread_create (arr+i,NULL,routine,s_data+i);
    }
    i = -1;
    while (++i < (int)args->number_of_coders)
        pthread_join(arr[i],NULL);    
    
    free_pointer(coders);
    free_list(dongles);
    free_pointer(args); 
    free_pointer(s_data);
    pthread_mutex_destroy(&mutex_1);


    return 0;
}