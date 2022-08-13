#ifndef FT_NM_FT_NM_H
#define FT_NM_FT_NM_H

#include <stdio.h>
#include <stdlib.h>

typedef struct  s_file
{
    char            *name;
    struct s_file   *next;
}               t_file;

typedef struct  s_info
{
    t_file  *files;
    char    opt[128];
}               t_info;


/**
 * utils.c
 */
t_file  *new_node(char *name);
void    add_list(t_file **head, t_file *new);
void    clear_list(t_file **head);
char    is_opt(char c);

#endif
