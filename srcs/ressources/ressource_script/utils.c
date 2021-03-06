/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/15 03:43:19 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/18 01:18:07 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "script.h"
#include "doom.h"

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

void					free_action(t_action *action)
{
	int	j;

	j = -1;
	if (action->type == ACTION_MESSAGE)
	{
		while (++j < action->data.message.textes_count)
			free(action->data.message.textes[j]);
		ft_memdel((void **)&action->data.message.textes);
	}
	else if (action->type == ACTION_QUESTION)
	{
		while (++j < action->data.question.quest_count)
			free(action->data.question.quest[j]);
		ft_memdel((void **)&action->data.question.quest);
		j = -1;
		while (++j < action->data.question.answer_count)
			free(action->data.question.answer[j]);
		ft_memdel((void **)&action->data.question.answer);
	}
}

void					free_script(t_script_data **s_addr)
{
	t_script_data	*sd;
	t_script		*script;
	int				i;
	int				j;

	if (!(*s_addr))
		return ;
	sd = *s_addr;
	ft_memdel((void **)&sd->script_str);
	i = -1;
	while (++i < sd->script_count)
	{
		j = -1;
		script = &sd->scripts[i];
		while (++j < script->actions_count)
			free_action(&script->action[j]);
	}
	ft_memdel((void **)&sd->scripts);
	ft_memdel((void **)s_addr);
}

void					reset_scripts(t_doom *doom)
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
