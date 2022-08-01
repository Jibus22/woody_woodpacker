/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 16:13:30 by jle-corr          #+#    #+#             */
/*   Updated: 2021/03/20 01:11:30 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncpy(char *dst, const char *src, size_t len)
{
	char	*dst_cpy;
	size_t	i;

	i = 0;
	dst_cpy = dst;
	while (*src && i++ < len)
		*dst++ = *src++;
	while (i++ < len)
		*dst++ = 0;
	return (dst_cpy);
}
