/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 17:03:42 by llelievr          #+#    #+#             */
/*   Updated: 2018/11/16 16:43:38 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static size_t	count_words(const char *s, char c)
{
	size_t		count;

	count = 1;
	while (*(++s))
		if (*(s - 1) == c && *s != c)
			count++;
	return (count);
}

static size_t	word_len(const char *s, char c)
{
	size_t		len;

	len = 0;
	while (*s && *s++ != c)
		len++;
	return (len);
}

char			**ft_strsplit(const char *s, char c)
{
	size_t		words;
	size_t		i;
	char		**tab;

	if (!s || !c)
		return (NULL);
	words = count_words(s, c);
	if (!(tab = (char **)malloc(sizeof(char*) * words + 1)))
		return (NULL);
	i = 0;
	while (*s)
	{
		if (*s == c && (*s++))
			continue ;
		if ((i == 0 && *s != c) || *(s - 1) == c)
			if (!(tab[i++] = ft_strsub(s, 0, word_len(s, c))))
				return (NULL);
		s++;
	}
	tab[i] = NULL;
	return (tab);
}
