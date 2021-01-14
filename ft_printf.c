/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 16:20:36 by dwanetta          #+#    #+#             */
/*   Updated: 2021/01/06 16:24:40 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int ft_check_specifier(const char *fmt)
{
    int i;
    int j;
    char *flags;

    j = 0;
    flags = "cspdiuxX";
    while (fmt[j] != '\0')
    {
        i = 0;
        while (flags[i] != '\0')
        {
            if (flags[i] == fmt[j])
                return (j + 1);
            i++;
        }
        j++;
    }
    return (0);
}

void chek_size_flags (va_list ap, const char *fmt, int *size, int *i)
{
    while (fmt[*i] != '\0')
    {
        if (fmt[*i + 1] == '-')
            break ;
        if (fmt[*i] == '*')
        {
            *size = va_arg(ap, int);
            break ;
        }
        else if (fmt[*i] != '0')
        {
            *size = ft_atoi(fmt + *i);
            break ;
        }
        *i += 1;
    }
}

int ft_print_format(t_flag flag, int size, long int value)
{
    int return_value;
    char symbol;

    return_value = 0;
    if (value == 0 && flag.dot != 0 && flag.count_simbol == 0)
        flag.size_width += 1;
    if (flag.zero == 0 || flag.count_simbol != 0)
        symbol = ' ';
    else
        symbol = '0';
    if (value < 0 && symbol != ' ')
        return_value += ft_putchar('-');
    if (value < 0)
        flag.size_width -= 1;
    if (flag.count_simbol < size)
        flag.count_simbol = size;
    while (((flag.size_width--) - flag.count_simbol > 0) && flag.minus == 0)
        return_value += ft_putchar(symbol);
    if (value < 0 && symbol == ' ')
        return_value += ft_putchar('-');
    while (((flag.count_simbol--) - size) > 0)
        return_value += ft_putchar('0');
    return (return_value);
}

int ft_print_format_left(t_flag *flag, int size)
{
    int return_value;

    return_value = 0;
    if (flag->minus == 1 && flag->count_simbol < size)
        flag->count_simbol = size;
    while (flag->minus == 1 && (flag->size_width--) - flag->count_simbol - 1 > 0)
        return_value += ft_putchar(' ');
    return (return_value);
}

void ft_print_dec(va_list ap, char fmt, t_flag *flag, int *return_value)
{
    int dec;
    unsigned int u_dec;
    int size;

    if (fmt == 'u')
    {
        u_dec = va_arg(ap, unsigned int);
        size = ft_chek_u(u_dec);
        *return_value += ft_print_format(*flag, size, u_dec);
        if (u_dec != 0 || flag->dot == 0 || flag->count_simbol != 0)
            *return_value += ft_putnbr_u(u_dec);
        else
            flag->size_width++;
        flag->size_width++;
    }
    else
    {
        dec = va_arg(ap, int);
        size = ft_chek(dec);
        *return_value += ft_print_format(*flag, size, dec);
        if (dec != 0 || flag->dot == 0 || flag->count_simbol != 0)
            *return_value += ft_putnbr(dec, 1);
        else
            flag->size_width += 1;
        if (dec >= 0)
            flag->size_width++;
    }
    *return_value += ft_print_format_left(flag, size);
}

void ft_print_hex(va_list ap, char fmt, t_flag *flag, int *return_value)
{
    int size;
    char *hex;
    unsigned int u_dec;
    unsigned long long int ull_dec;

    if (fmt == 'x' || fmt == 'X')
    {
        u_dec = va_arg(ap, unsigned int);
        ft_puthex(fmt, u_dec, &hex);
    }
    else
    {
        ull_dec = va_arg(ap, unsigned long long int);
        ft_puthex_pointer(fmt, ull_dec, &hex);
    }
    size = ft_strlen(hex);
    *return_value += ft_print_format(*flag, size, 1);
    *return_value += ft_putstr(hex, 0);
    flag->size_width++;
    *return_value += ft_print_format_left(flag, size);
    if (hex != NULL)
        free(hex);
}

void ft_print_char(va_list ap, char fmt, t_flag *flag, int *return_value)
{
    char *str;
    int size;
    char c;

    flag->zero = 0;
    if (fmt == 'c')
    {
        flag->dot = 0;
        flag->count_simbol = 0;
        c = (char)va_arg(ap, int);
        ft_print_format(*flag, 1, 1);
        flag->size_width++;
        *return_value += ft_putchar(c);
        *return_value += ft_print_format_left(flag, 1);
    }
    else
    {
        str = va_arg(ap, char *);
        size = ft_strlen(str);
        if (size > flag->count_simbol && flag->count_simbol != 0)
            size = flag->count_simbol;
        flag->count_simbol = 0;
        *return_value += ft_print_format(*flag, size, 1);
        flag->size_width++;
        *return_value += ft_putstr(str, size);
        *return_value += ft_print_format_left(flag, size);
    }
}

int ft_print_type(va_list ap, char fmt, t_flag *flag)
{
    int return_value;

    return_value = 0;
    if (fmt == 'd' || fmt == 'i' || fmt == 'u')
        ft_print_dec(ap, fmt, flag, &return_value);
    if (fmt == 'x' || fmt == 'X' || fmt == 'p')
        ft_print_hex(ap, fmt, flag, &return_value);
    if (fmt == 'c' || fmt == 's')
        ft_print_char(ap, fmt, flag, &return_value);
    return (return_value);
}

void ft_check_format(va_list ap, const char *fmt, int i, int *return_value)
{
    t_flag  flag;

    flag.dot = 0;
    flag.zero = 0;
    flag.minus = 0;
    flag.size_width = 0;
    flag.count_simbol = 0;
    while (fmt[i++] != '\0')
    {
        if (fmt[i] == '-')
            flag.minus = 1;
        if (fmt[i] == '0' && fmt[i - 1] == '%')
            flag.zero = 1;
        if (fmt[i] == '.')
            flag.dot = 1;
        if ((ft_isdigit(fmt[i]) || fmt[i] == '*') && flag.dot == 0 && flag.size_width == 0)
            chek_size_flags(ap, fmt, &flag.size_width, &i);
        if ((ft_isdigit(fmt[i]) || fmt[i] == '*') && flag.dot == 1 && flag.count_simbol == 0)
            chek_size_flags(ap, fmt, &flag.count_simbol, &i);
        if(fmt[i] == 'd' || fmt[i] == 'i' || fmt[i] == 'u' || fmt[i] == 'x'
        || fmt[i] == 'X' || fmt[i] == 'p' || fmt[i] == 's' || fmt[i] == 'c')
        {
            *return_value += ft_print_type(ap, fmt[i], &flag);
            break ;
        }
    }
}

int ft_printf(const char *fmt, ...)
{
    va_list ap;
    int i;
    int count;
    int return_value;

    i = 0;
    return_value = 0;
    va_start(ap, fmt);
    while (fmt[i] != '\0')
    {
        count = 0;
        if (fmt[i] == '%' && fmt[i + 1] != '%' && (count = ft_check_specifier(fmt + i)) > 0)
        {
            ft_check_format(ap, fmt, i, &return_value);
            i += count - 1;
        }
        if (fmt[i] != '%' && count == 0)
            return_value += ft_putchar(fmt[i]);
        if (fmt[i] == '%' && fmt[i + 1] == '%')
        {
            return_value += ft_putchar(fmt[i]);
            i+= 2;
        }
        else
            i++;
    }
    return (return_value);
}

int	main()
{
    int a;
    char *b;
    a = 2;
    b = "abcd";
    printf("     %d\n", ft_printf("%05d", a));
    printf("\n");
    printf("     %d\n",  printf("%05%"));
    /*int a;
    printf("%p\n",&a);
    unsigned int p = (unsigned int)&a;
    char s[2*sizeof(p)];
    for(int i = 2*sizeof(p)-1; i >= 0; --i)
    {

        s[i] = "0123456789ABCDEF"[p & 0x0F];
        p >>= 4;
    }
    for(int i = 0; i < 2*sizeof(p); ++i)
    {
        putc(s[i],stdout);
    }*/
    return 0;
}
