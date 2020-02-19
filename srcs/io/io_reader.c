/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_reader.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 16:30:11 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/19 17:53:13 by lloncham         ###   ########.fr       */
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

t_bool			io_peek2(t_reader *r, uint8_t *c)
{
	if (r->pos == r->len)
	{
		if (io_fill(r) <= 0)
		{
			return (FALSE);
		}
	}
	if (c)
		*c = r->buffer[r->pos];
	return (TRUE);
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
