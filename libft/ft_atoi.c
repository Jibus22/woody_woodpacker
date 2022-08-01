/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 16:14:19 by jle-corr          #+#    #+#             */
/*   Updated: 2021/03/20 16:52:25 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	unsigned int	nbr;
	int				i;
	int				neg;

	i = 0;
	while (str[i] == 32 || (str[i] > 8 && str[i] < 14))
		i++;
	neg = 0;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			neg = 1;
	nbr = 0;
	while (ft_isdigit(str[i]))
		nbr = nbr * 10 + str[i++] - 48;
	if (neg)
		return ((int)(-nbr));
	else
		return ((int) nbr);
}
