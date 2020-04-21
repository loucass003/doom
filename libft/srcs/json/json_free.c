/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/17 07:00:18 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/01 15:39:51 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	*json_free_ret(void *addr)
{
	free(addr);
	return (NULL);
}

void	json_free_array(t_json_element **elems)
{
	if ((elems[0])->next)
		json_free_array(&(elems[0])->next);
	if (elems[0]->value)
		json_free_value(elems[0]->value);
	json_free_ret(elems[0]);
}

void	json_free_members(t_json_member **member)
{
	if ((member[0])->next)
		json_free_members(&(member[0])->next);
	if (member[0]->string)
		json_free_ret(member[0]->string);
	if (member[0]->value)
		json_free_value(member[0]->value);
	json_free_ret(member[0]);
}

void	json_free_value(t_json_value *val)
{
	if (val->type == JSON_OBJECT)
	{
		if (((t_json_object *)val)->elements)
			json_free_members(&((t_json_object *)val)->elements);
	}
	else if (val->type == JSON_ARRAY)
	{
		if (((t_json_array *)val)->elements)
			json_free_array(&((t_json_array *)val)->elements);
	}
	json_free_ret(val);
}
