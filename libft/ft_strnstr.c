/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 16:14:06 by jle-corr          #+#    #+#             */
/*   Updated: 2021/03/20 01:14:57 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	needle_len;

	needle_len = ft_strlen(needle);
	if (needle_len == 0 || needle == haystack)
		return ((char *)haystack);
	i = 0;
	while (i < len && haystack[i] && len - i >= needle_len)
	{
		if (ft_strncmp(haystack + i, needle, needle_len) == 0)
			return ((char*)haystack + i);
		i++;
	}
	return (NULL);
}
