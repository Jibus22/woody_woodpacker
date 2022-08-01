/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 16:15:36 by jle-corr          #+#    #+#             */
/*   Updated: 2021/03/20 01:18:17 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*trimed;
	char	*adr;
	int		i;

	while (*s1 && (ft_strchr(set, *s1)))
		s1++;
	adr = (char *)s1;
	while (*s1)
		s1++;
	if (adr != s1)
		while ((ft_strchr(set, *(s1 - 1))) && (s1 - 1) != adr)
			s1--;
	trimed = ft_strnew(s1 - adr);
	if (!(trimed))
		return (NULL);
	i = 0;
	while (adr != s1)
		*(trimed + i++) = *adr++;
	return (trimed);
}
