#include "../include/ft_nm.h"

t_file  *new_node(char *name)
{
    t_file *ret = 0;

    if (!(ret = malloc(sizeof(t_file))))
        return 0;
    ret->name = name;
    ret->next = 0;
    return ret;
}

void    add_list(t_file **head, t_file *new)
{
    if (!*head) {
        (*head) = new;
    } else {
        t_file *tmp = 0;

        for (tmp = *head; tmp->next; tmp = tmp->next);
        tmp->next = new;
    }
}

void    clear_list(t_file **head)
{
    t_file *tmp = 0;
    t_file *t = 0;

    tmp = *head;
    while (tmp) {
        t = tmp;
        tmp = tmp->next;
        free(t);
    }
    *head = 0;
}

char    is_opt(char c)
{
    if (c == 'a' || c == 'g' || c == 'u' || c == 'r' || c == 'p')
        return c;
    return 0;
}