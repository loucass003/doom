/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_numbers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/26 12:57:00 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/26 14:18:32 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj.h"

static long	ft_powd(long n, long pow)
{
	long	out;

	if (pow == 1)
		return (n);
	if (pow == 0)
		return (1);
	out = n;
	while (--pow > 0)
		out *= n;
	return (out);
}

static t_bool		readnum(t_reader *r, int32_t *out, size_t *len, t_bool s)
{
	int16_t		c;
	uint32_t	res;
	t_bool		sign;

	*out = 0;
	res = 0;
	*len = 0;
	sign = FALSE;
	if ((c = io_peek(r)) == '-' || c == '+')
	{
		if (!s)
			return (FALSE);
		sign = c == '-';
		io_next(r);
	}
	while ((c = io_peek(r)) >= '0' && c <= '9')
	{
		res = res * 10 + (c - '0');
		io_next(r);
		(*len)++;
	}
	*out = sign ? -res : res;
	return (TRUE);
}

t_bool		io_readnum(t_reader *r, int32_t *out)
{
	size_t	len;

	return (readnum(r, out, &len, TRUE));
}

t_bool		io_readfloat(t_reader *r, float *out)
{
	int32_t	part;
	char	c;
	size_t	len;
	
	part = 0;
	if(!io_readnum(r, &part))
		return (FALSE);
	*out = part;
	c = io_peek(r);
	if (ft_isprint(c) && c != '.')
		return (FALSE);
	io_next(r);
	part = 0;
	if (!readnum(r, &part, &len, FALSE))
		return (FALSE);
	c = io_peek(r);
	if (ft_isprint(c) && !ft_isspace(c))
		return (FALSE);
	(*out) += (float)part / ft_powd(10, len);
	return (TRUE);
}