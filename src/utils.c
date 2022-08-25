#include "../include/ft_nm.h"

int     ft_memcmp(const void *s1, const void *s2, size_t n)
{
    const unsigned char *su1;
    const unsigned char *su2;
    int                 ret;

    su1 = s1;
    su2 = s2;
    ret = 0;
    while (0 < n)
    {
        if ((ret = *su1 - *su2) != 0)
            break ;
        su1++;
        su2++;
        n--;
    }
    return (ret);
}

t_file  *new_file_node(char *name)
{
    t_file *ret = 0;

    if (!(ret = malloc(sizeof(t_file))))
        return 0;
    ret->name = name;
    ret->next = 0;
    return ret;
}

void    add_file_list(t_file **head, t_file *new)
{
    if (!*head) {
        (*head) = new;
    } else {
        t_file *tmp = 0;

        for (tmp = *head; tmp->next; tmp = tmp->next);
        tmp->next = new;
    }
}

void    clear_file_list(t_file **head)
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

t_symbol    *new_symbol_node(t_symbol param)
{
    t_symbol *ret = 0;

    if (!(ret = malloc(sizeof(t_symbol))))
        return 0;
    memset(ret, 0, sizeof(t_symbol));
    memcpy(ret, &param, sizeof(t_symbol));
    return ret;
}

void    add_symbol_list(t_symbol **head, t_symbol *new)
{
    t_symbol *tmp = 0;

    if (!*head) {
        *head = new;
    } else {
        for (tmp = *head; tmp->next; tmp = tmp->next);
        tmp->next = new;
        new->prev = tmp;
    }
}

void    add_in_order_symbol_list(t_info *info, t_symbol **head, t_symbol *new, int (*compare)())
{
    if (!*head || compare(info, (*head)->name, new->name, (*head)->value, new->value, ft_strcasecmp) >= 0) {
        new->next = *head;
        if (*head)
            (*head)->prev = new;
        *head = new;
    } else {
        t_symbol *tmp = *head;
        while (tmp->next && compare(info, tmp->next->name, new->name, tmp->next->value, new->value, ft_strcasecmp) < 0)
            tmp = tmp->next;
        new->next = tmp->next;
        tmp->next = new;
        new->prev = tmp;
        if (tmp->next && tmp->next->next)
            tmp->next->next->prev = new;
    }
}

void    clear_symbol_list(t_symbol **head)
{
    t_symbol *tmp = 0;
    t_symbol *t = 0;

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

void    *map_file(t_file *file)
{
    int         fd = 0;
    void        *ret = 0;
    struct stat buf = {0};

    if ((fd = open(file->name, O_RDONLY)) < 0)
        error(file->name, "No such file");
    if (fstat(fd, &buf) < 0)
        error(file->name, "fstat error");
    file->size = buf.st_size;
    if ((ret = mmap(0, buf.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED)
        error(file->name, "mmap error");
    close(fd);
    return ret;
}

int     check_file(t_file *file, void *mem)
{
    unsigned char *e_ident = mem;
    unsigned char magic[] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};

    if (ft_memcmp(mem, magic, sizeof(magic)))
        error(file->name, "file format not recognized");
    if (e_ident[EI_CLASS] != ELFCLASS32 && e_ident[EI_CLASS] != ELFCLASS64)
        error(file->name, "class not recognized");
    return 1;
}

int     ft_strcasecmp(const char *s1, const char *s2, char skip)
{
    int c1, c2;

    do {
        while (skip && *s1 && !isalpha(*s1) && !isdigit(*s1))
            s1++;
        while (skip && *s2 && !isalpha(*s2) && !isdigit(*s2))
            s2++;
        c1 = tolower(*s1++);
        c2 = tolower(*s2++);
    } while (c1 == c2 && c1 != 0);
    return c1 - c2;
}

int     nm_compare(t_info *info, char *_s1, char *_s2, uint64_t v1, uint64_t v2, int (*compare)())
{
    char *s1 = _s1;
    char *s2 = _s2;
    int  ret = 0;

    while (*s1 && !isalpha(*s1))
        s1++;
    while (*s2 && !isalpha(*s2))
        s2++;
    if (!(ret = compare(s1, s2, 1))) {
        if (!(ret = compare(_s1, _s2, 0)) && !info->opt['r']) {
            ret = v1 - v2;
        }
    }
    return ret;
}