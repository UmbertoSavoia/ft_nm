#ifndef FT_NM_H
#define FT_NM_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <elf.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

typedef struct  s_file
{
    char            *name;
    size_t          size;
    struct s_file   *next;
}               t_file;

typedef struct  s_info
{
    t_file  *files;
    int     nfiles;
    char    opt[128];
}               t_info;

typedef struct  s_symbol
{
    char            *name;
    unsigned char   type;
    unsigned char   bind;
    uint16_t        ndx;
    uint64_t        value;
    char            sym_table;
    struct s_symbol *prev;
    struct s_symbol *next;
}               t_symbol;

#define error(file, msg)                                    \
    do {                                                    \
        nm_print(2, "ft_nm: '%s': %s\n", file, msg);        \
        return 0;                                           \
    } while (0)

/**
 * utils.c
 */
int         ft_memcmp(const void *s1, const void *s2, size_t n);
t_file      *new_file_node(char *name);
void        add_file_list(t_file **head, t_file *new);
void        clear_file_list(t_file **head);
t_symbol    *new_symbol_node(t_symbol param);
void        add_symbol_list(t_symbol **head, t_symbol *new);
void        add_in_order_symbol_list(t_info *info, t_symbol **head, t_symbol *new, int (*compare)());
void        clear_symbol_list(t_symbol **head);
char        is_opt(char c);
void        *map_file(t_file *file);
int         check_file(t_file *file, void *mem);
int         nm_compare(t_info *info, char *_s1, char *_s2, uint64_t v1, uint64_t v2, int (*compare)());
int         ft_strcasecmp(const char *s1, const char *s2, char skip);

/**
 * print.c
 */
void    nm_print(int fd, const char *fmt, ...);

#define set_type_symbol(node, shdr)                                         \
do {                                                                        \
    char r = '?';                                                           \
    if ((node)->ndx == SHN_ABS) {                                           \
        r = (node)->bind == STB_LOCAL ? 'a' : 'A';                          \
    } else if ((node)->bind == STB_WEAK) {                                  \
        r = (node)->ndx == SHN_UNDEF ? 'w' : 'W';                           \
    } else if ((node)->bind == STB_WEAK && (node)->type == STT_OBJECT) {    \
        r = 'V';                                                            \
    } else if ((node)->ndx == SHN_UNDEF) {                                  \
        r = 'U';                                                            \
    } else if ((node)->ndx == SHN_COMMON) {                                 \
        r = 'C';                                                            \
    } else if ((node)->bind == STB_GNU_UNIQUE) {                            \
        r = 'u';                                                            \
    } else if ((shdr).sh_type == SHT_NOBITS) {                              \
        r = 'B';                                                            \
    } else if ((shdr).sh_flags == (SHF_ALLOC | SHF_MERGE)) {                \
        r = 'R';                                                            \
    } else if ((shdr).sh_flags == (SHF_ALLOC | SHF_WRITE)) {                \
        r = 'D';                                                            \
    } else if ((shdr).sh_type == SHT_INIT_ARRAY || (shdr).sh_type == SHT_FINI_ARRAY) { \
        r = 'T';                                                            \
    } else if ((shdr).sh_type == SHT_DYNAMIC) {                             \
        r = 'D';                                                            \
    } else if ((shdr).sh_flags == (SHF_ALLOC | SHF_EXECINSTR)) {            \
        r = 'T';                                                            \
    } else if ((shdr).sh_flags & SHF_ALLOC) {                               \
        r = 'R';                                                            \
    } else {                                                                \
        r = 'N';                                                            \
    }                                                                       \
    (node)->bind == STB_LOCAL ?                                             \
        ((node)->sym_table = tolower(r)) : ((node)->sym_table = r);         \
    if (!ft_memcmp((node)->name, ".debug", 6))                              \
        (node)->sym_table = 'N';                                            \
} while(0)

#define save_symbol(info, head, mem, ehdr, t_shdr, t_sym)                   \
do {                                                                        \
    typeof (t_shdr) shdr = mem + (ehdr)->e_shoff;                           \
    typeof (t_sym) sym = 0;                                                 \
    char *strtab = mem + shdr[(ehdr)->e_shstrndx].sh_offset;                \
    char *symtab = 0;                                                       \
    size_t tot_sym = 0;                                                     \
    t_symbol *node = 0;                                                     \
    for (int i = 0; i < (ehdr)->e_shnum; ++i) {                             \
        if (shdr[i].sh_type == SHT_SYMTAB) {                                \
            symtab = mem + shdr[shdr[i].sh_link].sh_offset;                 \
            tot_sym = shdr[i].sh_size / shdr[i].sh_entsize;                 \
            sym = mem + shdr[i].sh_offset;                                  \
            for (size_t j = 1; j < tot_sym; ++j) {                          \
                node = new_symbol_node((t_symbol) {                         \
                            (sym[j].st_info == STT_SECTION) ?               \
                                (strtab + shdr[sym[j].st_shndx].sh_name) :  \
                                (symtab + sym[j].st_name),                  \
                            ELF64_ST_TYPE(sym[j].st_info),                  \
                            ELF64_ST_BIND(sym[j].st_info),                  \
                            sym[j].st_shndx,                                \
                            sym[j].st_value,                                \
                            ' ',                                            \
                            0,                                              \
                            0                                               \
                        });                                                 \
                set_type_symbol(node, shdr[sym[j].st_shndx]);               \
                if ((info)->opt['p'])                                       \
                    add_symbol_list(head, node);                            \
                else                                                        \
                    add_in_order_symbol_list(info, head, node, nm_compare); \
            }                                                               \
            break;                                                          \
        }                                                                   \
    }                                                                       \
} while(0)

#endif
