/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_json_actions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 17:12:51 by lloncham          #+#    #+#             */
/*   Updated: 2020/03/04 17:13:30 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "script.h"
#include "ressource.h"
#include "file_utils.h"
#include "item.h"

t_bool					parse_json_action(t_action *action,
	t_json_object *object)
{
	t_json_string	*j_string;

	j_string = json_get_string(object, "type");
	if ((action->type = get_action_type(j_string->value)) == ACTION_NONE)
		return (script_return_error("Invalid action type"));
	if (action->type == ACTION_MESSAGE
		&& !parse_action_message(&action->data.message, object))
		return (script_return_error("Invalid action message"));
	else if (action->type == ACTION_TELEPORT
		&& !parse_action_teleport(&action->data.teleport, object))
		return (script_return_error("Invalid action teleport"));
	else if (action->type == ACTION_QUESTION
		&& !parse_action_question(&action->data.question, object))
		return (script_return_error("Invalid action question"));
	return (TRUE);
}

t_bool					parse_json_actions(t_script *script,
	t_json_array *array)
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
		if (!parse_json_action(&script->action[script->actions_count],
			(t_json_object *)element->value))
			return (script_return_error("Invalid action"));
		script->actions_count++;
		element = element->next;
	}
	return (TRUE);
}
