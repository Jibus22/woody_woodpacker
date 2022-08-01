/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 15:58:21 by jle-corr          #+#    #+#             */
/*   Updated: 2021/03/20 01:14:28 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnew(int size)
{
	char	*new;

	new = (char *)malloc(sizeof(*new) * (size += 1));
	if (!(new))
		return (NULL);
	while (size--)
		new[size] = 0;
	return (new);
}
