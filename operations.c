#include "coders/codexion.h"

int sleep_for_operation(TIME t_operation,t_shared_data *s_data)
{
    long end = get_timestamp_ms(s_data->start) + t_operation;
    while (get_timestamp_ms(s_data->start) < end)
    {
        if(*s_data->flag_burnout)
            return 0;
        usleep(500);
    }
    return 1;
}



long get_timestamp_ms(struct timeval start)
{
    struct timeval now;

    gettimeofday(&now, NULL);

    long seconds = now.tv_sec - start.tv_sec;
    long microseconds = now.tv_usec - start.tv_usec;

    return (seconds * 1000) + (microseconds / 1000);
}