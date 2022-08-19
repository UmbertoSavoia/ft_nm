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
            info->nfiles++;
        }
    }
    if (info->opt['p'])
        info->opt['r'] = 0;
    if (info->opt['u'])
        info->opt['g'] = info->opt['a'] = 0;
    if (info->opt['g'])
        info->opt['a'] = 0;
}

void    print_line(t_symbol *s, int arch)
{
    if (s->ndx == SHN_UNDEF)
        printf("%*c", arch, ' ');
    else
        printf("%0*lx", arch, s->value);
    printf(" %c %s\n", s->sym_table, s->name);
}

void    print_symbols(t_info *info, t_file *file, t_symbol *sym, int arch)
{
    if (info->nfiles > 1)
        printf("\n%s:\n", file->name);
    for (t_symbol *s = sym; s; s = s->next) {
        if (info->opt['u']) {
            if (s->ndx == SHN_UNDEF)
                print_line(s, arch);
        }
        else if (info->opt['g']) {
            if (s->bind == STB_WEAK || s->bind == STB_GLOBAL)
                print_line(s, arch);
        }
        else if (info->opt['a']) {
            print_line(s, arch);
        }
        else if ((s->type != STT_SECTION) && (s->type != STT_FILE)) {
            print_line(s, arch);
        }
    }
}

int     do_file(t_info *info, t_file *file)
{
    void        *mem = 0;
    t_symbol    *head = 0;

    if (!(mem = map_file(file)))
        return 0;
    if (!check_file(file, mem)) {
        munmap(mem, file->size);
        return 0;
    }
    if (((unsigned char *)mem)[EI_CLASS] == ELFCLASS32)
        save_symbol(info, &head, mem, (Elf32_Ehdr *)mem, (Elf32_Shdr *)0, (Elf32_Sym *)0);
    else
        save_symbol(info, &head, mem, (Elf64_Ehdr *)mem, (Elf64_Shdr *)0, (Elf64_Sym *)0);
    print_symbols(info, file, head,
                  ((unsigned char *)mem)[EI_CLASS] == ELFCLASS32 ?
                  sizeof(Elf32_Addr)*2 :
                  sizeof(Elf64_Addr)*2);
    if (!head)
        fprintf(stderr, "ft_nm: '%s': %s\n", file->name, "no symbols");
    clear_symbol_list(&head);
    munmap(mem, file->size);
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
        if (!do_file(&info, file))
            error = 1;
    }
    clear_file_list(&(info.files));
    return error;
}
