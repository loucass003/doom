/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressource_script.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 14:15:25 by lloncham          #+#    #+#             */
/*   Updated: 2020/03/04 17:10:32 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "script.h"
#include "ressource.h"
#include "file_utils.h"
#include "item.h"

t_bool					load_script(t_ressource *r, char *path)
{
	t_script_data	*script;

	if (!(script = ft_memalloc(sizeof(t_script_data))))
		return (FALSE);
	if (!(script->script_str = get_file_content(path, &script->script_len)))
		return (FALSE);
	if (!parse_script_data(script))
		return (FALSE);
	r->data.script_data = script;
	r->loaded = TRUE;
	return (TRUE);
}

t_bool					read_script(t_ressource_manager *r,
	t_script_data **s_data)
{
	t_script_data	*s;

	if (!(s = malloc(sizeof(t_script_data))))
		return (FALSE);
	if (!io_memcpy(&r->reader, &s->script_len, sizeof(int)))
		return (FALSE);
	if (!(s->script_str = malloc(sizeof(char) * s->script_len)))
		return (FALSE);
	if (!io_memcpy(&r->reader, s->script_str, sizeof(char) * s->script_len))
		return (FALSE);
	if (!parse_script_data(s))
		return (FALSE);
	*s_data = s;
	return (TRUE);
}

t_bool					write_script(t_ressource_manager *rm,
	t_script_data *s_data)
{
	dp_write(rm, &s_data->script_len, sizeof(int));
	dp_write(rm, s_data->script_str, s_data->script_len * sizeof(char));
	return (TRUE);
}

t_bool					script_return_error(char *error)
{
	ft_putstr("Script error: ");
	ft_putendl(error);
	return (FALSE);
}

t_bool					trigger_area(t_trigger_area *area,
	t_json_object *object)
{
	double		*radius_d;

	if (!object)
		return (FALSE);
	if (!ft_json_vec3(json_object_get(object, "pos"), &area->pos))
		return (FALSE);
	if (!(radius_d = json_get_number(object, "radius")))
		return (FALSE);
	area->radius = *radius_d;
	return (TRUE);
}
