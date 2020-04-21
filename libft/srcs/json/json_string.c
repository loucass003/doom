/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_string.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 23:07:53 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/07 23:30:33 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static char		json_escape(t_json_state *state)
{
	char	c;

	c = state->str[state->pos - 1];
	if (c == '\\')
	{
		c = state->str[state->pos];
		if (c == '"')
			return ('"');
		if (c == 'n')
			return ('\n');
	}
	return (c);
}

static int		parse_len(t_json_state *state)
{
	int		len;
	int		pos;

	len = 0;
	pos = 0;
	while (state->str[state->pos + pos] != '"')
	{
		if (state->pos + pos >= state->len)
			return (-1);
		if (state->str[state->pos + pos] == '\\')
			pos++;
		pos++;
		len++;
	}
	return (len);
}

t_json_value	*json_parse_string(t_json_state *state)
{
	char			c;
	int				len;
	t_json_string	*string;

	if ((len = parse_len(state)) == -1)
		return (NULL);
	if (!(string = (t_json_string *)malloc(sizeof(t_json_string)
		+ (len + 1) * sizeof(char))))
		return (NULL);
	string->value[len] = 0;
	string->super.type = JSON_STRING;
	string->value_len = len;
	len = 0;
	while (state->pos < state->len && len <= string->value_len)
	{
		c = state->str[state->pos++];
		if (c == '"')
			return ((t_json_value *)string);
		string->value[len++] = json_escape(state);
		if (state->str[state->pos - 1] == '\\')
			state->pos++;
	}
	return (NULL);
}

t_json_string	*json_to_string(t_json_value *value)
{
	if (!value || value->type != JSON_STRING)
		return (NULL);
	return ((t_json_string *)value);
}

t_json_string	*json_get_string(t_json_object *obj, char *key)
{
	return (json_to_string(json_object_get(obj, key)));
}
