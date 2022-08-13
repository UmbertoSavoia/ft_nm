#include "../include/ft_nm.h"

void    parse_arg(int ac, char **av, t_info *info)
{
    for (int i = 1; i < ac; ++i) {
        if (av[i][0] == '-') {
            for (int j = 0; av[i][j]; ++j) {
                if (is_opt(av[i][j]))
                    info->opt[av[i][j]] = 1;
            }
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

    /**
     * DEBUG
     */
    for (t_file *tmp = info.files; tmp; tmp = tmp->next)
        printf("%s\n", tmp->name);
    for (unsigned char i = 0; i < 128; ++i)
        if (info.opt[i])
            printf("%c - ", i);
    puts("");
    // ----------------------

    clear_list(&(info.files));
}
