#include "../include/ft_nm.h"

void    parse_arg(int ac, char **av, t_info *info)
{
    for (int i = 1; i < ac; ++i) {
        if (av[i][0] == '-') {
            // opt
        } else {
            add_list(&(info->files), new_node(av[i]));
        }
    }
}


int     main(int ac, char **av)
{
    t_info info = {0};

    if (ac <= 1) {
        add_list(&(info.files), new_node("a.out"));
    } else {
        parse_arg(ac, av, &info);
    }

    for (t_file *tmp = info.files; tmp; tmp = tmp->next)
        printf("%s\n", tmp->name);

    clear_list(&(info.files));
}
