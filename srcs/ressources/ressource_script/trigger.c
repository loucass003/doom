/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trigger.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 16:50:13 by lloncham          #+#    #+#             */
/*   Updated: 2020/02/21 18:16:41 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "script.h"
#include "doom.h"

void		trigger_event(t_doom *doom, t_trigger trigger)
{
	SDL_Event	event;
	t_trigger	*t;

	if (doom->main_context.type != CTX_NORMAL)
		return ;
	if (doom->trigger_event == (uint32_t)(-1))
		return ;
	if (!(t = malloc(sizeof(t_trigger))))
		return ;
	ft_memcpy(t, &trigger, sizeof(t_trigger));
	ft_bzero(&event, sizeof(event));
	event.type = doom->trigger_event;
	event.user.data1 = t;
	SDL_PushEvent(&event);
}

void		trigger_actions(t_doom *doom, t_script *script, int j)
{
	t_action		*action;
	t_vec3			rot;

	action = &script->action[j];
	if (action->type == ACTION_MESSAGE)
	{
		doom->message.counter_m = 0;
		doom->message.open = TRUE;
		doom->message.message_data = &action->data.message;
	}
	else if (action->type == ACTION_TELEPORT)
	{
		rot = doom->player.entity.rotation;
		if (action->data.teleport.is_rotation_set == TRUE)
			rot = action->data.teleport.rotation;
		teleport(&doom->player.entity, action->data.teleport.pos, rot);
	}
	else if (action->type == ACTION_QUESTION)
	{
		doom->message.counter_q = -1;
		doom->message.question_data = &action->data.question;
	}
}

void		trigger_script(t_doom *doom, t_trigger trigger)
{
	int				i;
	int				j;
	t_script_data	*s_data;
	t_script		*script;

	i = -1;
	s_data = doom->res_manager.ressources->values[26]->data.script_data;
	while (++i < s_data->script_count)
	{
		script = &s_data->scripts[i];
		if (script->use == 0
			|| ft_memcmp(&script->trigger, &trigger, sizeof(t_trigger)) != 0)
			continue ;
		if (script->use > 0)
			script->use--;
		j = -1;
		while (++j < script->actions_count)
			trigger_actions(doom, script, j);
	}
}
