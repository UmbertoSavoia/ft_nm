#include "../include/ft_nm.h"

void    nm_putchar(int fd, char c)
{
    write(fd, &c, 1);
}

void    nm_putstr(int fd, char *s)
{
    while (*s)
        nm_putchar(fd, *s++);
}

int     nbrlen(int num, int base_len)
{
    int count = 0;
    int u_num = num < 0 ? -(int)num : (int)num;

    if(num == 0)
        return 1;
    while(u_num > 0) {
        count++;
        u_num = u_num / base_len;
    }
    return count;
}

void    nm_printnbr(int fd, size_t num, int base_len, int width)
{
    int len_num = nbrlen(num, base_len);
    char *base = (base_len == 10) ? "0123456789" : "0123456789abcdef";
    char *str = 0;

    if (!(str = malloc(len_num + 1)))
        return ;
    if (num == 0 && len_num == 1) {
        str[0] = '0';
        str[1] = 0;
    } else {
        for (int i =  len_num - 1; num != 0; --i, num /= base_len) {
            str[i] = base[num % base_len];
        }
        str[len_num] = 0;
    }
    for (int i = 0; i < (width - len_num); ++i)
        nm_putchar(fd, '0');
    nm_putstr(fd, str);
    free(str);
}

void    nm_print(int fd, const char *fmt, ...)
{
    int width = 0, zero = 0;
    va_list ap;
    va_start(ap, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            ++fmt;
            if (*fmt == '0') {
                zero = 1;
                ++fmt;
            }
            if (*fmt == '*') {
                width = va_arg(ap, int);
                ++fmt;
            }
            if (*fmt == 'x') {
                nm_printnbr(fd, va_arg(ap, size_t), 16, (zero && width) ? width : 0);
            } else if (*fmt == 's') {
                nm_putstr(fd, va_arg(ap, char*));
            } else if (*fmt == 'c') {
                char c = va_arg(ap, int);
                if (!zero && width) {
                    for (int i = 0; i < width; ++i)
                        nm_putchar(fd, c);
                } else {
                    nm_putchar(fd, c);
                }
            }
            zero = width = 0;
        } else {
            nm_putchar(fd, *fmt);
        }
        ++fmt;
    }
    va_end(ap);
}