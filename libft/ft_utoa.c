/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/25 16:16:32 by jle-corr          #+#    #+#             */
/*   Updated: 2021/03/20 16:30:00 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_utoa(unsigned int n)
{
	char			*str;
	unsigned int	e;
	int				i;

	i = 1;
	e = n / 10;
	while (e)
	{
		e /= 10;
		i++;
	}
	str = ft_strnew(i);
	if (!(str))
		return (NULL);
	while (i--)
		str[i] = ((n / ft_pow(10, e++)) % 10) + 48;
	return (str);
}
