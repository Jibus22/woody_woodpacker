/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 16:08:39 by jle-corr          #+#    #+#             */
/*   Updated: 2021/03/20 01:02:38 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char		*dst_cp;
	const char	*src_cp;

	if (!dst && !src)
		return (NULL);
	dst_cp = (char *)dst;
	src_cp = (const char *)src;
	while (n--)
		*dst_cp++ = *src_cp++;
	return (dst);
}
