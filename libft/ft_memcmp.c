/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 16:10:04 by jle-corr          #+#    #+#             */
/*   Updated: 2021/03/20 01:04:17 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*s1_cp;
	const unsigned char	*s2_cp;

	s1_cp = (const unsigned char *)s1;
	s2_cp = (const unsigned char *)s2;
	while (n--)
		if (*s1_cp++ != *s2_cp++)
			return (*(--s1_cp) - *(--s2_cp));
	return (0);
}
