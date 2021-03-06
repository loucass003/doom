/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_skip.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/26 12:57:57 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/19 02:32:03 by llelievr         ###   ########.fr       */
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
	while (io_peek(r, NULL))
	{
		if (io_expect(r, str))
			return (TRUE);
		io_next(r);
	}
	return (FALSE);
}
