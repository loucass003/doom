/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 16:30:11 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/24 14:22:17 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	skip_ws(t_reader *r)
{
	while (io_peek(r) == ' ' || io_peek(r) == '\t')
		r->pos++;
}
