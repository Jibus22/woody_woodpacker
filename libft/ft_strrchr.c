/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 16:12:50 by jle-corr          #+#    #+#             */
/*   Updated: 2021/03/20 01:15:47 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		s_len;
	char	*s_cp;
	char	d;

	s_len = ft_strlen(s) + 1;
	s_cp = (char *)s;
	d = (char)c;
	while (s_len--)
		if (*(s_cp + s_len) == d)
			return (s_cp + s_len);
	return (NULL);
}
