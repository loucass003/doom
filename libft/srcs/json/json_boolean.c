/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_boolean.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 22:59:46 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/01 15:45:22 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_json_value	*json_parse_boolean(t_json_state *state, t_bool is_true)
{
	t_json_boolean	*b;

	if (!(b = (t_json_boolean *)malloc(sizeof(t_json_boolean))))
		return (NULL);
	state->pos += is_true ? 4 : 5;
	b->value = is_true;
	b->super.type = JSON_BOOLEAN;
	return ((t_json_value *)b);
}

t_bool			*json_to_bool(t_json_value *value)
{
	if (!value || value->type != JSON_BOOLEAN)
		return (NULL);
	return (&(((t_json_boolean *)value)->value));
}

t_bool			*json_get_bool(t_json_object *obj, char *key)
{
	return (json_to_bool(json_object_get(obj, key)));
}
