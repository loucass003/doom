/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressource_script.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 14:15:25 by lloncham          #+#    #+#             */
/*   Updated: 2020/02/12 13:12:57 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "script.h"
#include "ressource.h"
#include "file_utils.h"
#include "item.h"

t_bool		load_script(t_ressource *r, char *path)
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

t_bool			read_script(t_ressource_manager *r, t_script_data **s_data)
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

t_bool		write_script(t_ressource_manager *rm, t_script_data *s_data)
{
	dp_write(rm, &s_data->script_len, sizeof(int));
	dp_write(rm, s_data->script_str, s_data->script_len * sizeof(char));
    return (TRUE);
}

static t_trigger_type	get_trigger_type(char *str)
{
	const int	len = ft_strlen(str);

	if (len == 5 && ft_strncmp(str, "SPAWN", 5) == 0)
		return (TRIG_SPAWN);
	if (len == 9 && ft_strncmp(str, "PICK_ITEM", 9) == 0)
		return (TRIG_PICK_ITEM);
	if (len == 4 && ft_strncmp(str, "AREA", 4) == 0)
		return (TRIG_AREA);
	return (TRIG_NONE);
}


t_bool		script_return_error(char *error)
{
	ft_putstr("Script error: ");
	ft_putendl(error);
	return (FALSE);
}

static t_action_type	get_action_type(char *str)
{
	const int	len = ft_strlen(str);

	if (len == 7 && ft_strncmp(str, "MESSAGE", 7) == 0)
		return (ACTION_MESSAGE);
	if (len == 8 && ft_strncmp(str, "TELEPORT", 8) == 0)
		return (ACTION_TELEPORT);
	return (ACTION_NONE);
}

t_bool          trigger_area(t_trigger_area *area, t_json_object *object)
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

t_bool			parse_json_trigger(t_script *script, t_json_object *object)
{
	t_json_string	*j_string;

	if (!object)
		return (FALSE);
	if (!(j_string = json_get_string(object, "type")))
		return (FALSE);
	script->trigger.type = get_trigger_type(j_string->value);
	if (script->trigger.type == TRIG_NONE)
		return(script_return_error(j_string->value));
	if (script->trigger.type == TRIG_PICK_ITEM)
		return (trigger_pick_item(&script->trigger.data.pick_item, object));
	else if (script->trigger.type == TRIG_AREA)
		return (trigger_area(&script->trigger.data.area, object));
	return (TRUE);
}

t_bool			parse_action_message(t_action_message *message, t_json_object *object)
{
	t_json_array	*array;
	t_json_element	*element;
	t_json_string	*string;

	if (!(array = json_get_array(object, "text")))
		return (script_return_error("No text element or invalid text element"));
	if (!(message->textes = ft_memalloc(array->elems_count * sizeof(char *))))
		return (script_return_error("Unnable to alloc strings"));
	element = array->elements;
	while (element)
	{
		if (!(string = json_to_string(element->value)))
			return (script_return_error("Invalid message type (NOT A STRING)"));
		if (!(message->textes[message->textes_count] = ft_memalloc(sizeof(char) * (string->value_len + 1))))
			return (script_return_error("Unnable to alloc strings"));
		ft_memcpy(message->textes[message->textes_count], string->value, string->value_len);
		message->textes_count++;
		element = element->next;
	}
	return (TRUE);
}

t_bool			parse_action_teleport(t_action_teleport *teleport, t_json_object *object)
{
	t_json_value	*val;

	if (!ft_json_vec3(json_object_get(object, "pos"), &teleport->pos))
		return (script_return_error("No pos element or invalid pos element"));
	if ((val = json_object_get(object, "rotation")))
	{
		if (!ft_json_vec3(val, &teleport->rotation))
			return (script_return_error("Invalid rotation element"));
		teleport->is_rotation_set = TRUE;
	}
	return (TRUE);
}

t_bool			parse_json_action(t_action *action, t_json_object *object)
{
	t_json_string	*j_string;

	j_string = json_get_string(object, "type");
	if ((action->type = get_action_type(j_string->value)) == ACTION_NONE)
		return (script_return_error("Invalid action type"));
	if (action->type == ACTION_MESSAGE && !parse_action_message(&action->data.message, object))
		return (script_return_error("Invalid action message"));
	else if (action->type == ACTION_TELEPORT && !parse_action_teleport(&action->data.teleport, object))
		return (script_return_error("Invalid action teleport"));
	return (TRUE);
}

t_bool			parse_json_actions(t_script *script, t_json_array *array)
{
	t_json_element	*element;

	if (!array)
		return (FALSE);
	element = array->elements;
	while (element)
	{
		if (element->value->type != JSON_OBJECT)
			return (script_return_error("ELEMENT in ACTIONS is not an Object"));
		if (script->actions_count == MAX_ACTIONS - 1)
			return (script_return_error("Too much actions in script"));
		if (!parse_json_action(&script->action[script->actions_count], (t_json_object *)element->value))
			return (script_return_error("Invalid action"));
		script->actions_count++;
		element = element->next;
	}
	return (TRUE);
}

t_bool			parse_script_data(t_script_data *s_data)
{
	t_json_state	state;
	t_json_value	*val;
	t_json_array	*array;
	t_json_element	*element;
	
	int				i;

	state = (t_json_state){s_data->script_str, 0, s_data->script_len};
	if (!(val = parse_value(&state)))
		return (FALSE);
	if (!(array = json_to_array(val)))
		return (script_return_error("root element must be an array"));
	if (!(s_data->scripts = ft_memalloc(sizeof(t_script) * array->elems_count)))
		return (script_return_error("unable to alloc script"));	
	element = array->elements;
	i = -1;
	while (element)
	{
		i++;
		t_script	*script = &s_data->scripts[s_data->script_count];
		if (element->value->type != JSON_OBJECT)
			return (script_return_error("script mut be an object"));
		if (!parse_json_trigger(script, json_get_object((t_json_object *)element->value, "trigger")))
			return (script_return_error("object 'trigger' doesn't exist"));
		double *use_d;
		script->use = -1;
		if (!!(use_d = (json_get_number((t_json_object *)element->value, "use"))))
		{
			script->use = (int)*use_d;
			// if (script->trigger.type == TRIG_SPAWN)
				// script->use = 1;
		}
		if (!parse_json_actions(script, json_get_array((t_json_object *)element->value, "actions")))	
			return (script_return_error("array 'actions' doesn't exist"));
		s_data->script_count++;
		element = element->next;
	}
	json_free_value(val);
	return (TRUE);
}