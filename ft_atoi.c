/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 18:30:41 by dwanetta          #+#    #+#             */
/*   Updated: 2021/01/15 01:40:29 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *str)
{
	long int	result;
	int			pluse;
	int			i;

	i = 0;
	result = 0;
	pluse = 1;
	while (str[i] == ' ' || str[i] == '\f' || str[i] == '\v'
	|| str[i] == '\t' || str[i] == '\n' || str[i] == '\r')
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			pluse = -1;
	while (str[i] != '\0')
		if (str[i] >= 48 && str[i] <= 57)
		{
			result = result * 10 + (str[i++] - 48);
			if (i >= 19 && pluse < 0)
				return (0);
			else if (i >= 19)
				return (-1);
		}
		else
			break ;
	return ((int)(result * pluse));
}
