/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trigger.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 16:50:13 by lloncham          #+#    #+#             */
/*   Updated: 2020/02/11 18:32:36 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "script.h"
#include "doom.h"

void        trigger_event(t_doom *doom, t_trigger trigger)
{
    SDL_Event   event;
    t_trigger   *t;

    if (doom->trigger_event == (uint32_t)-1)
        return ;
    if (!(t = malloc(sizeof(t_trigger))))
        return ;
    ft_memcpy(t, &trigger, sizeof(t_trigger));
    ft_bzero(&event, sizeof(event));
    event.type = doom->trigger_event;
    event.user.data1 = t;
    SDL_PushEvent(&event);
}

void        trigger_script(t_doom *doom, t_trigger trigger)
{
    int             i;
    int             j;
    t_script_data   *s_data;
    t_script        *script;
    t_action        *action;

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
        {
            action = &script->action[j];
            if (action->type == ACTION_MESSAGE)
            {
                // message->open = TRUE;
                // message->counter = 0;
                // message->message_data = action->data.message;
            }
        }
    }
}