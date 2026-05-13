#include "coders/codexion.h"

void compile(TIME t_compile)
{
    usleep(t_compile * 1000);
}

void cooldown(TIME t_cooldown)
{
    usleep(t_cooldown * 1000);
}

void debugging(TIME t_debugging)
{
    usleep(t_debugging * 1000);
}


void refactoring(TIME t_refactoring)
{
    usleep(t_refactoring * 1000);
}


long get_timestamp_ms(struct timeval start)
{
    struct timeval now;

    gettimeofday(&now, NULL);

    long seconds = now.tv_sec - start.tv_sec;
    long microseconds = now.tv_usec - start.tv_usec;

    return (seconds * 1000) + (microseconds / 1000);
}