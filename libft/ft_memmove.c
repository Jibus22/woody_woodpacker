/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 16:09:28 by jle-corr          #+#    #+#             */
/*   Updated: 2021/03/20 01:04:46 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char		*dst_cp;
	const char	*src_cp;

	if (!dst && !src)
		return (NULL);
	dst_cp = (char *)dst;
	src_cp = (const char *)src;
	if (dst_cp > src_cp)
		while (len--)
			*(dst_cp + len) = *(src_cp + len);
	else
		while (len--)
			*dst_cp++ = *src_cp++;
	return (dst);
}
