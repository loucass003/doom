/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_object.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 23:34:20 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/08 14:30:32 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static void			member_put(t_json_member **alst, t_json_member *n)
{
	t_json_member	*e;

	if (!*alst)
	{
		*alst = n;
		return ;
	}
	e = *alst;
	while (e->next)
		e = e->next;
	e->next = n;
}

static t_json_value	*parse_member(t_json_state *state, t_json_object *o)
{
	t_json_member	*e;
	char			c;

	if (state->pos < state->len)
	{
		c = json_skip_ws(state);
		if (c != '"' || !(e = (t_json_member *)malloc(sizeof(t_json_member))))
			return (NULL);
		e->next = NULL;
		if (!(e->string = (t_json_string *)json_parse_string(state)))
			return (NULL);
		c = json_skip_ws(state);
		if (c != ':' || !(e->value = parse_value(state)))
		{
			free(e->string);
			return (json_free_ret(e));
		}
		member_put(&o->elements, e);
		o->members_count++;
		if ((c = json_skip_ws(state)) == ',')
			return (parse_member(state, o));
		else if (c == '}')
			return ((t_json_value *)o);
	}
	return (NULL);
}

t_json_value		*json_parse_object(t_json_state *state)
{
	t_json_object	*o;
	char			c;

	if (state->pos < state->len)
	{
		c = json_skip_ws(state);
		if (state->pos >= state->len)
			return (NULL);
		if (!(o = (t_json_object *)malloc(sizeof(t_json_object))))
			return (NULL);
		o->super.type = JSON_OBJECT;
		o->members_count = 0;
		o->elements = NULL;
		if (c == '}')
			return ((t_json_value *)o);
		state->pos--;
		if (!parse_member(state, o))
		{
			if (o->elements)
				json_free_members(&o->elements);
			return (json_free_ret(o));
		}
		return ((t_json_value *)o);
	}
	return (NULL);
}

t_json_value		*json_object_get(t_json_object *obj, char *key)
{
	t_json_member	*mem;

	mem = obj->elements;
	while (mem)
	{
		if (mem->string->value_len == (int)ft_strlen(key)
			&& ft_strncmp(mem->string->value, key, mem->string->value_len) == 0)
			return (mem->value);
		mem = mem->next;
	}
	return (NULL);
}

t_json_object		*json_get_object(t_json_object *obj, char *key)
{
	const t_json_value	*val = json_object_get(obj, key);

	if (!val || val->type != JSON_OBJECT)
		return (NULL);
	return ((t_json_object *)val);
}
