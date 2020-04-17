/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 16:51:59 by lloncham          #+#    #+#             */
/*   Updated: 2020/04/17 23:33:04 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "script.h"
#include "ressource.h"
#include "file_utils.h"
#include "item.h"
#include "doom.h"

t_bool					parse_json_trigger(t_script *script,
	t_json_object *object)
{
	t_json_string	*j_string;

	if (!object)
		return (FALSE);
	if (!(j_string = json_get_string(object, "type")))
		return (FALSE);
	script->trigger.type = get_trigger_type(j_string->value);
	if (script->trigger.type == TRIG_NONE)
		return (script_return_error(j_string->value));
	if (script->trigger.type == TRIG_PICK_ITEM)
		return (trigger_pick_item(&script->trigger.data.pick_item, object));
	else if (script->trigger.type == TRIG_AREA)
		return (trigger_area(&script->trigger.data.area, object));
	return (TRUE);
}

t_bool					parse_action_message(t_action_message *message,
	t_json_object *object)
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
		if (!(message->textes[message->textes_count] = ft_memalloc(sizeof(char)
			* (string->value_len + 1))))
			return (script_return_error("Unnable to alloc strings"));
		ft_memcpy(message->textes[message->textes_count], string->value,
			string->value_len);
		message->textes_count++;
		element = element->next;
	}
	return (TRUE);
}

t_bool					parse_action_question(t_action_question *question,
	t_json_object *object)
{
	t_json_array	*array;
	t_json_element	*element;
	t_json_string	*string;

	if (!(array = json_get_array(object, "choice")))
		return (script_return_error("No text element or invalid text element"));
	if (!(question->quest = ft_memalloc(array->elems_count * sizeof(char *))))
		return (script_return_error("Unnable to alloc strings"));
	element = array->elements;
	while (element)
	{
		if (!(string = json_to_string(element->value)))
			return (script_return_error("Invalid question type(NOT A STRING)"));
		if (!(question->quest[question->quest_count] = ft_memalloc(sizeof(char)
			* (string->value_len + 1))))
			return (script_return_error("Unnable to alloc strings"));
		ft_memcpy(question->quest[question->quest_count], string->value,
			string->value_len);
		question->quest_count++;
		element = element->next;
	}
	if (!(array = json_get_array(object, "answer choice")))
		return (script_return_error("No text element or invalid text element"));
	if (!(question->answer = ft_memalloc(array->elems_count * sizeof(char *))))
		return (script_return_error("Unnable to alloc strings"));
	element = array->elements;
	while (element)
	{
		if (!(string = json_to_string(element->value)))
			return (script_return_error("Invalid question type(NOT A STRING)"));
		if (!(question->answer[question->answer_count] = ft_memalloc(
			sizeof(char) * (string->value_len + 1))))
			return (script_return_error("Unnable to alloc strings"));
		ft_memcpy(question->answer[question->answer_count], string->value,
			string->value_len);
		question->answer_count++;
		element = element->next;
	}
	return (TRUE);
}

t_bool					parse_action_teleport(t_action_teleport *teleport,
	t_json_object *object)
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

t_bool					parse_script_data(t_script_data *s_data)
{
	t_json_state	state;
	t_json_value	*val;
	t_json_array	*array;
	t_json_element	*element;

	state = (t_json_state){s_data->script_str, 0, s_data->script_len};
	if (!(val = parse_value(&state)))
		return (FALSE);
	if (!(array = json_to_array(val)))
		return (script_return_error("root element must be an array"));
	if (!(s_data->scripts = ft_memalloc(sizeof(t_script) * array->elems_count)))
		return (script_return_error("unable to alloc script"));
	s_data->script_count = 0;
	element = array->elements;
	while (element)
	{
		t_script	*script;

		script = &s_data->scripts[s_data->script_count];
		if (element->value->type != JSON_OBJECT)
			return (script_return_error("script mut be an object"));
		if (!parse_json_trigger(script,
			json_get_object((t_json_object *)element->value, "trigger")))
			return (script_return_error("object 'trigger' doesn't exist"));
		double *use_d;
		script->use = -1;
		if (!!(use_d = (json_get_number(
			(t_json_object *)element->value, "use"))))
			script->use = (int)*use_d;
		script->use_default = script->use;
		if (!parse_json_actions(script,
			json_get_array((t_json_object *)element->value, "actions")))
			return (script_return_error("array 'actions' doesn't exist"));
		s_data->script_count++;
		element = element->next;
	}
	json_free_value(val);
	return (TRUE);
}

void		reset_scripts(t_doom *doom)
{
	int				i;
	t_script_data	*s_data;
	t_script		*script;

	i = -1;
	s_data = doom->res_manager.ressources->values[26]->data.script_data;
	while (++i < s_data->script_count)
	{
		script = &s_data->scripts[i];
		script->use = script->use_default;
	}
}
