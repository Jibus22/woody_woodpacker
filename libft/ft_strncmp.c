/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 16:13:10 by jle-corr          #+#    #+#             */
/*   Updated: 2021/03/20 01:11:05 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	const unsigned char	*s1_cp;
	const unsigned char	*s2_cp;

	s1_cp = (const unsigned char *)s1;
	s2_cp = (const unsigned char *)s2;
	while (n--)
	{
		if (!*s1_cp || !*s2_cp)
			return (*s1_cp - *s2_cp);
		if (*s1_cp++ != *s2_cp++)
			return (*--s1_cp - *--s2_cp);
	}
	return (0);
}
