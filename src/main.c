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
            add_file_list(&(info->files), new_file_node(av[i]));
        }
    }
    if (info->opt['p'])
        info->opt['r'] = 0;
    if (info->opt['u'])
        info->opt['g'] = info->opt['a'] = 0;
    if (info->opt['g'])
        info->opt['a'] = 0;
}

int     do_file(t_info *info)
{
    void        *mem = 0;
    t_symbol    *head = 0;

    if (!(mem = map_file(info->files)))
        return 0;
    if (!check_file(info->files, mem)) {
        munmap(mem, info->files->size);
        return 0;
    }
    if (((unsigned char *)mem)[EI_CLASS] == ELFCLASS32) {
        save_symbol(info, &head, mem, (Elf32_Ehdr *)mem, (Elf32_Shdr *)0, (Elf32_Sym *)0);
    } else {
        save_symbol(info, &head, mem, (Elf64_Ehdr *)mem, (Elf64_Shdr *)0, (Elf64_Sym *)0);
    }

    for (t_symbol *t = head; t; t = t->next)
        printf("%s\n", t->name);

    clear_symbol_list(&head);
    munmap(mem, info->files->size);
    return 1;
}

int     main(int ac, char **av)
{
    t_info  info = {0};
    int     error = 0;

    if (ac <= 1) {
        add_file_list(&(info.files), new_file_node("a.out"));
    } else {
        parse_arg(ac, av, &info);
    }
    for (t_file *file = info.files; file; file = file->next) {
        if (!do_file(&info))
            error = 1;
    }

    /**
     * DEBUG
     */
     /*
    for (t_file *tmp = info.files; tmp; tmp = tmp->next)
        printf("%s\n", tmp->name);
    for (unsigned char i = 0; i < 128; ++i)
        if (info.opt[i])
            printf("%c - ", i);
    puts("");
    // ----------------------*/

    clear_file_list(&(info.files));
    return error;
}
