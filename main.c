#include "coders/codexion.h"

pthread_mutex_t mutex_1;
pthread_cond_t cond_1;

void compile(unsigned int time_to_compile)
{
    usleep(time_to_compile*1000);
}

void debug(unsigned int time_to_debug)
{
    usleep(time_to_debug*1000);
}

void refactor(unsigned int time_to_refactor)
{
    usleep(time_to_refactor*1000);
}


void *monitor(void *data)
{
    (void)data;
    return NULL;

}


void *routine(void *data)
{
    pthread_mutex_lock(&mutex_1);
    t_shared_data *t_data = (t_shared_data *)data;
    t_coder *coder = get_coder(t_data->queue_coders,t_data->id_thread);
    if(coder->left->state_dongle && coder->right->state_dongle )
        pthread_cond_wait(&cond_1,&mutex_1);
    else
    {
        coder->left->state_dongle = 1;
        coder->right->state_dongle = 1;
        compile(t_data->args->time_to_compile);
        printf("coder %d compiling ...., unsing dongle  left %d(%p)=>{%d} and right %d(%p)=>{%d}\n",
        coder->coder_id,
        coder->left->dongle_id,
        coder->left,
        coder->left->state_dongle,
        coder->right->dongle_id,
        coder->right,
        coder->right->state_dongle
        );
        usleep(t_data->args->dongle_cooldown * 1000);
        coder->left->state_dongle = 0;
        coder->right->state_dongle = 0;
        pthread_cond_broadcast(&cond_1);

    }
    
    // // else{

    

    // if(!coder->left->state_dongle || !coder->right->state_dongle)
    //     printf("not allowed to run \n");
    // else
    //     printf("allow to run \n");
    // // }
    pthread_mutex_unlock(&mutex_1);

    return NULL;
}



int main(int c,char **v)
{
    pthread_mutex_init(&mutex_1,NULL);
    pthread_cond_init(&cond_1,NULL);
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
    t_coder *coders = malloc(sizeof(t_coder));
    t_coder *c_coders = coders;
    if(!coders  || !s_data)
        return 1;
    set_coders(coders,args->number_of_coders);
    set_dongles(coders);

    
    int i = -1;
    while (++i < (int)args->number_of_coders)
    {
        set_shared_data(c_coders->coder_id,s_data+i,coders,args);
        c_coders = c_coders->next;
        pthread_create(&arr[i],NULL,routine,s_data+i);
    }
    i = -1;
    while (++i < (int)args->number_of_coders)
        pthread_join(arr[i],NULL);


    
    
    
   
    
    // free_pointer(coders);
    // // free_list(dongles);
    // free_pointer(args); 
    // free_pointer(s_data);
    pthread_cond_destroy(&cond_1);

    pthread_mutex_destroy(&mutex_1);


    return 0;
}