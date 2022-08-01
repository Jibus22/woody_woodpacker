/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ultox.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/25 16:16:32 by jle-corr          #+#    #+#             */
/*   Updated: 2021/03/20 16:43:23 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_ultox(unsigned long int n)
{
	unsigned long int	e;
	short int			res;
	char				*str;
	int					i;

	i = 1;
	e = n / 16;
	while (e)
	{
		e /= 16;
		i++;
	}
	str = ft_strnew(i);
	if (!(str))
		return (NULL);
	while (i--)
	{
		res = ((n / ft_pow(16, e++)) % 16);
		if (res < 10)
			res += 48;
		else
			res += 87;
		str[i] = res;
	}
	return (str);
}
