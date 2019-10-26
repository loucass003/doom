/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_skip.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/26 12:57:57 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/26 02:05:58 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"

void			io_skip_ws(t_reader *r)
{
	char		c;

	while (io_peek(r, &c) && (c == ' ' || c == '\t'))
		io_next(r);
}

void			io_skip_empty(t_reader *r)
{
	char		c;

	while (io_peek(r, &c) && (ft_isspace(c) || !ft_isprint(c)))
		io_next(r);
}

t_bool			io_skip_until(t_reader *r, char *str)
{
	char	 	c;
	size_t		i;

	while (io_peek(r, &c))
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
