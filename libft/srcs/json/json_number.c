/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_number.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 23:00:59 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/18 17:41:22 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

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
 
static long	d_atoi(char *c, t_json_state *state)
{
	long		part;

	part = 0;
	while (ft_isdigit(*c) && state->pos < state->len)
	{
		part = part * 10 + (*c - '0');
		*c = state->str[state->pos++];
	}
	return (part);
}

static double	parse_floating(char *c, t_json_state *state)
{
	long	part;
	int		pos;

	pos = state->pos;
	*c = state->str[state->pos++];
	part = d_atoi(c, state);
	if (part == 0)
		return (-1);
	return (part / (double)(ft_powd(10, (state->pos - 1 - pos))));
}

static void	parse_exponent(t_json_number *n, char *c, t_json_state *state)
{
	int		sign;

	*c = state->str[++state->pos];
	sign = (*c == '+' || *c == '-' ? *c == '-' : 0);
	if (*c == '+' || *c == '-')
		*c = state->str[state->pos++];
	long part = d_atoi(c, state);
	n->value *= ft_powd(10, part * sign);
}

t_json_value	*json_parse_number(t_json_state *state)
{
	char			sign;
	char			c;
	double			part;
	t_json_number	*num;

	if (state->pos < state->len)
	{
		c = state->str[state->pos++];
		if (!(num = (t_json_number *)malloc(sizeof(t_json_number))))
			return (NULL);
		*num = (t_json_number) { .super = { .type = JSON_NUMBER }, .value = 0 };
		sign = (c == '+' || c == '-' ? c == '-' : 0);
		if (c == '+' || c == '-')
			c = state->str[state->pos++];
		num->value = d_atoi(&c, state);
		if (c == '.' && (part = parse_floating(&c, state)) != -1)
			num->value += part;
		if (c == 'e')
			parse_exponent(num, &c, state);
		num->value *= sign ? -1 : 1;
		state->pos--;
		return ((t_json_value *)num);
	}
	return (NULL);
}
