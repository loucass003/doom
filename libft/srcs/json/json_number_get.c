/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_number_get.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/18 14:11:34 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/18 14:11:55 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double			*json_to_number(t_json_value *value)
{
	if (!value || value->type != JSON_NUMBER)
		return (NULL);
	return (&(((t_json_number *)value)->value));
}

double			*json_get_number(t_json_object *obj, char *key)
{
	return (json_to_number(json_object_get(obj, key)));
}
