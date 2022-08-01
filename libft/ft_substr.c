/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 16:14:59 by jle-corr          #+#    #+#             */
/*   Updated: 2021/03/20 01:33:03 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	i;

	sub = ft_strnew(len);
	if (!(sub))
		return (NULL);
	i = 0;
	if (start < ft_strlen(s))
		while (i < len && s[start])
			sub[i++] = s[start++];
	return (sub);
}
