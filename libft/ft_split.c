/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 18:04:52 by jle-corr          #+#    #+#             */
/*   Updated: 2021/03/20 15:20:20 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	split_counter(char const *s, char c)
{
	char	*spliter;
	int		i;

	i = 0;
	while (s && *s)
	{
		spliter = ft_strchr(s, c);
		if ((spliter - s) != 0)
			i++;
		if (spliter)
			s = spliter + 1;
		else
			s = NULL;
	}
	return (i + 1);
}

char const	*ternarie_ret(char *adr)
{
	if (adr)
		return (adr + 1);
	else
		return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	char	*adr;
	int		i;

	i = split_counter(s, c);
	tab = (char **)malloc(sizeof(*tab) * i);
	if (!tab)
		return (NULL);
	tab[i - 1] = NULL;
	i = 0;
	while (s && *s)
	{
		adr = ft_strchr(s, c);
		if ((adr - s) != 0)
		{
			if (adr)
				tab[i] = ft_substr(s, 0, adr - s);
			else
				tab[i] = ft_substr(s, 0, ft_strlen(s));
			if (!(tab[i++]))
				return (ft_freetab(tab));
		}
		s = ternarie_ret(adr);
	}
	return (tab);
}
