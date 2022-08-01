/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ltoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/25 16:16:32 by jle-corr          #+#    #+#             */
/*   Updated: 2021/03/20 16:36:13 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_cell(long n)
{
	int			i;
	long		e;

	if (n < 0)
		i = 2;
	else
		i = 1;
	e = n / 10;
	while (e != 0)
	{
		e /= 10;
		i++;
	}
	return (i);
}

static void	fill_ltoa(long n, char *str, int i)
{
	short int	res;
	short int	stop;
	int			e;

	e = 0;
	stop = 0;
	if (n < 0)
		stop = 1;
	while (i-- > stop)
	{
		res = ((n / ft_pow(10, e++)) % 10);
		if (n > 0)
			str[i] = res + 48;
		else
			str[i] = res + 48 - (res * 2);
	}
	if (n < 0)
		str[i] = '-';
}

char	*ft_ltoa(long n)
{
	char		*str;
	int			i;

	i = count_cell(n);
	str = ft_strnew(i);
	if (!(str))
		return (NULL);
	fill_ltoa(n, str, i);
	return (str);
}
