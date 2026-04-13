#ifndef CODEXION
#define CODEXION

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>

#define TIME unsigned int 

typedef struct
{   
    unsigned int number_of_coders; 
    TIME time_to_burnout;
    TIME time_to_compile;
    TIME time_to_debug;
    TIME time_to_refactor;
    unsigned int number_of_compiles_required;
    TIME dongle_cooldown;
    char *scheduler;

} t_args;

typedef struct s_dongle
{
    unsigned int dongle_id;
    int state_dongle;
    struct s_dongle *next;

} t_dongle;

typedef struct 
{
    unsigned int coder_id;
    unsigned int count_compiled;
    char *state_coder;
    t_dongle *left;
    t_dongle *right;
} t_coder;


typedef struct
{
    t_coder *queue_coders;
    t_dongle *list_dongles;
    unsigned int size_coders;
    int index_queue;
    int start_index_list;
    t_args *args;
} t_shared_data;


int parsing (int c,char **v,t_args *args);
void set_coders(t_coder *coder,unsigned int n_coders);
void set_dongles (t_dongle *dongle,unsigned int n_dongles);
void set_shared_data(t_shared_data *data,t_coder *coders,t_dongle *dongles,t_args *args);
void free_pointer(void *ptr);
void free_list (t_dongle *list);
unsigned int size_list(t_dongle *head);
t_dongle *get_dongle(unsigned int index,t_dongle *head);

#endif 