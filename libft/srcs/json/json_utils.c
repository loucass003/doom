/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 23:47:49 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/01 15:35:41 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		json_skip_ws(t_json_state *state)
{
	char	c;

	c = state->str[state->pos++];
	while (state->pos < state->len && json_is_ws(c))
		c = state->str[state->pos++];
	return (c);
}

int			json_is_ws(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

int			json_match_key(t_json_state *s, char *at, char *key, int len)
{
	return (s->pos + len < s->len && ft_strnequ(key, at, len));
}
