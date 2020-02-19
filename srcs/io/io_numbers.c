/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_numbers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/26 12:57:00 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/19 17:51:35 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"

static t_bool	get_sign(t_reader *r, t_bool s)
{
	t_bool		sign;
	char		c;

	sign = FALSE;
	if (io_peek(r, &c) && (c == '-' || c == '+'))
	{
		if (!s)
			return (FALSE);
		sign = c == '-';
		io_next(r);
	}
	return (sign);
}

static long		ft_powd(long n, long pow)
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

static t_bool	readnum(t_reader *r, int32_t *out, size_t *len, t_bool s)
{
	char		c;
	uint32_t	res;
	t_bool		sign;

	*out = 0;
	res = 0;
	*len = 0;
	sign = get_sign(r, s);
	while (io_peek(r, &c) && c >= '0' && c <= '9')
	{
		res = res * 10 + (c - '0');
		io_next(r);
		(*len)++;
	}
	*out = sign ? -res : res;
	return (TRUE);
}

t_bool			io_readnum(t_reader *r, int32_t *out)
{
	size_t	len;

	return (readnum(r, out, &len, TRUE));
}

t_bool			io_readfloat(t_reader *r, float *out)
{
	int32_t	part;
	char	c;
	size_t	len;
	t_bool	sign;

	sign = get_sign(r, TRUE);
	if (!readnum(r, &part, &len, FALSE))
		return (FALSE);
	*out = part;
	if (!io_peek(r, &c))
		c = -1;
	if (ft_isspace(c))
		return (TRUE);
	if (!ft_isdigit(c) && c != '.')
		return (FALSE);
	io_next(r);
	if (!readnum(r, &part, &len, FALSE))
		return (FALSE);
	if (!io_peek(r, &c))
		c = -1;
	if ((ft_isprint(c) && !ft_isspace(c)))
		return (FALSE);
	(*out) += (float)part / ft_powd(10, len);
	(*out) *= sign ? -1 : 1;
	return (TRUE);
}
