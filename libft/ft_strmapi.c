/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 16:16:44 by jle-corr          #+#    #+#             */
/*   Updated: 2021/03/20 01:10:37 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	char	*strmap;

	if (!s || !f)
		return (NULL);
	strmap = (char *)malloc(sizeof(*strmap) * (ft_strlen(s) + 1));
	if (!(strmap))
		return (NULL);
	i = -1;
	while (s[++i] != '\0')
		strmap[i] = (*f)(i, s[i]);
	strmap[i] = '\0';
	return (strmap);
}
