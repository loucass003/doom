/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 22:55:23 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/01 15:36:15 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_json_value	*parse_value(t_json_state *s)
{
	char			c;
	t_bool			is_true;

	if (s->pos < s->len)
	{
		c = json_skip_ws(s);
		if (c == '"')
			return (json_parse_string(s));
		else if (c == '{')
			return (json_parse_object(s));
		else if (c == '[')
			return (json_parse_array(s));
		s->pos--;
		if ((is_true = json_match_key(s, s->str + s->pos, "true", 4))
		|| json_match_key(s, s->str + s->pos, "false", 5))
			return (json_parse_boolean(s, is_true));
		else if (json_match_key(s, s->str + s->pos, "null", 4))
			return (json_parse_null(s));
		else if (ft_isdigit(c) || c == '+' || c == '-')
			return (json_parse_number(s));
		else
			return (NULL);
	}
	return (NULL);
}
