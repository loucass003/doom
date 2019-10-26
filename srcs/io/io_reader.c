/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_reader.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 16:30:11 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/26 04:04:06 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "io.h"

static ssize_t	io_fill(t_reader *r)
{
	ssize_t	ret;

	r->pos = 0;
	if ((ret = read(r->fd, r->buffer, sizeof(r->buffer))) < 0)
		return (ret);
	return (r->len = ret);
}

t_bool			io_peek(t_reader *r, char *c)
{
	if (r->pos == r->len)
	{
		if (io_fill(r) <= 0)
		{
			if (c)
				*c = -1;
			return (FALSE);
		}
	}
	if (c)
		*c = r->buffer[r->pos];
	return (TRUE);
}

void			io_next(t_reader *r)
{
	r->pos++;
	r->total_read++;
}

t_bool			io_expect(t_reader *r, const char *str)
{
	char		c;

	while (*str)
	{
		if (!io_peek(r, &c) || c != *str)
			return (FALSE);
		io_next(r);
		str++;
	}
	return (TRUE);
}

t_bool			io_memcpy(t_reader *r, void *dst, size_t n)
{
	size_t		i;
	char		c;

	i = 0;
	while (i < n)
	{
		if (!io_peek(r, &c))
			return (FALSE);
		io_next(r);
		((uint8_t *)dst)[i++] = c;
	}
	return (TRUE);
}