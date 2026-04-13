#include "coders/codexion.h"

int ft_atoi(char *str,int *is_overflow)
{
    int i = 0;
    int sign = 1;
    long result = 0;
    while (str[i] == ' ' || str[i] == '\t')
            i++;    
    if(str[i] == '-' || str[i] == '+')
    {
        if(str[i] == '-')
            sign = -1;
        i++;
    }
    while (str[i] >= '0' && str[i] <= '9')
    {
        result = (str[i] - '0') +  (result * 10);
        if(result*sign > INT_MAX || result*sign < INT_MIN)
            *is_overflow = 1;
        i++;
    }
    return result * sign;
    
}

int ft_is_num(char *s)
{
    int i = 0;
    if(!*s)
        return 0;
    while (s[i] == ' ' || s[i] == '\t')
            i++;    
    if(s[i] == '-' || s[i] == '+')
        i++;
    while (s[i])
    {
        if(!(s[i] >= '0' && s[i] <= '9'))
            return 0;
        i++;
    }   
    return 1;
}   

int error(char *err_msg)
{
    printf("%s\n",err_msg);
    return 0;    
}

int parsing (int c,char **v,t_args *args)
{
    int i = 0;
    int is_overflow = 0;
    int tmp_num;
    if (c != 9)
        return error("number of arguments must be 8");
    while (++i < c - 1)
    {
        tmp_num = ft_atoi(v[i],&is_overflow);
        if(!ft_is_num(v[i])) 
                return error("Non-integer input or empty string (letters, symbols, floats like 1.5, mixed like 12abc)");
        if (is_overflow)
                return error("overflow hhhhhhhh");
        if (tmp_num < 0)
                return error("Negative numbers (e.g. -5)");
        if (tmp_num == 0 && i != 7)
                return error("invalid argument 0");
    }
    if (strcmp(v[c-1],"fifo") && strcmp(v[c-1],"edf"))
        return error("The value must be exactly one of: fifo or edf");
    args->number_of_coders = ft_atoi(v[1],&is_overflow);
    args->time_to_burnout = ft_atoi(v[2],&is_overflow);
    args->time_to_compile =  ft_atoi(v[3],&is_overflow);
    args->time_to_debug= ft_atoi(v[4],&is_overflow);
    args->time_to_refactor = ft_atoi(v[5],&is_overflow);
    args->number_of_compiles_required = ft_atoi(v[6],&is_overflow);
    args->dongle_cooldown = ft_atoi(v[7],&is_overflow);
    args->scheduler = v[8];
    return 1;

}