/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_skip.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/26 12:57:57 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/26 13:53:45 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj.h"

void			io_skip_ws(t_reader *r)
{
	while (io_peek(r) == ' ' || io_peek(r) == '\t')
		r->pos++;
}

void			io_skip_empty(t_reader *r)
{
	char		c;

	while ((c = io_peek(r)) != -1 && !ft_isalpha(c))
		r->pos++;
}

t_bool			io_skip_until(t_reader *r, char *str)
{
	int16_t c;
	size_t	i;

	while ((c = io_peek(r)) != -1)
	{
		i = 0;
		while (str[i])
		{
			if (str[i] == c)
				return (TRUE);
			i++;
		}
		io_next(r);
	}
	return (FALSE);
}