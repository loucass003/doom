/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 16:30:11 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/29 02:45:20 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "obj.h"

static ssize_t	io_fill(t_reader *r)
{
	ssize_t	ret;

	r->pos = 0;
	if ((ret = read(r->fd, r->buffer, sizeof(r->buffer))) < 0)
		return (ret);
	return (r->len = ret);
}

char			io_peek(t_reader *r)
{
	if (r->pos == r->len)
		if (io_fill(r) <= 0)
			return (-1);
	return (r->buffer[r->pos]);
}

void			io_next(t_reader *r)
{
	r->pos++;
}

t_bool			io_expect(t_reader *r, const char *str)
{
	while (*str)
	{
		if (io_peek(r) != *str)
			return (FALSE);
		r->pos++;
		str++;
	}
	return (TRUE);
}