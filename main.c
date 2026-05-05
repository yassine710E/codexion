#include "coders/codexion.h"

int main(int c,char **v)
{
    t_args args;
    struct timeval start;
    unsigned int how_many_waits = 0;
    if(!parsing(c,v,&args))
        return 1;
    t_coder *arr_coders = malloc(sizeof(t_coder) * args.number_of_coders);
    t_shared_data *s_data = malloc(sizeof(t_shared_data) * args.number_of_coders);
    
    //threads variables
    pthread_mutex_t mutex_arr[args.number_of_coders];
    pthread_mutex_t main_mutex;
    pthread_cond_t cond_arr[args.number_of_coders];
    pthread_cond_t main_cond;
    pthread_mutex_t display_mutex;
    pthread_cond_t display_cond;
    pthread_t arr_threads[args.number_of_coders];

    
    init_arr_mutex(mutex_arr,args.number_of_coders);
    pthread_mutex_init(&main_mutex,NULL);
    pthread_mutex_init(&display_mutex,NULL);
    init_arr_cond(cond_arr,args.number_of_coders);
    pthread_cond_init(&main_cond,NULL);
    pthread_cond_init(&display_cond,NULL);
    
    if(!arr_coders || !s_data)
        return 1;
    set_coders(arr_coders,args.number_of_coders,mutex_arr,cond_arr);
    gettimeofday(&start, NULL);
    set_shared_data(s_data,arr_coders,&args,&display_mutex,&display_cond,&main_mutex,&main_cond,start,&how_many_waits);
    unsigned int i = 0;
    while (i < args.number_of_coders)
    {
        pthread_create(arr_threads + i,NULL,coder_routine,s_data + i);
        i++;
    }

    i = 0;
    while (i < args.number_of_coders)
    {
        pthread_join(arr_threads[i],NULL);
        i++;
    }

    pthread_cond_destroy(&display_cond);
    pthread_cond_destroy(&main_cond);
    destroy_arr_cond(cond_arr,args.number_of_coders);
    pthread_mutex_destroy(&main_mutex);
    pthread_mutex_destroy(&display_mutex);
    destroy_arr_mutex(mutex_arr,args.number_of_coders);

    return 0;
}
