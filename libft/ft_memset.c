/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 16:08:21 by jle-corr          #+#    #+#             */
/*   Updated: 2021/03/20 01:05:08 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*b_cpy;
	unsigned char	d;

	d = (unsigned char)c;
	b_cpy = (unsigned char *)b;
	while (len--)
		*b_cpy++ = d;
	return (b);
}
