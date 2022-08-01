/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 16:09:40 by jle-corr          #+#    #+#             */
/*   Updated: 2021/03/20 01:03:47 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*s_cp;
	unsigned char	d;

	d = (unsigned char)c;
	s_cp = (unsigned char *)s;
	while (n--)
		if (d == *s_cp++)
			return (s_cp - 1);
	return (NULL);
}
