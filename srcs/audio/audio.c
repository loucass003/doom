/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 13:41:15 by lloncham          #+#    #+#             */
/*   Updated: 2020/02/16 17:22:24 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "audio.h"
#include "entity.h"

int				get_source(t_audio *s)
{
	int		i;

	i = 0;
	while (i < MAX_OPEN_SOUNDS)
	{
		if (s->source_status[i] == FALSE)
			return (i);
		++i;
	}
	return (-1);
}

t_bool			init_openal(t_doom *doom)
{
	doom->audio.device = alcOpenDevice(NULL);
	if (!(doom->audio.context = alcCreateContext(doom->audio.device, NULL)))
		return (FALSE);
	if (!alcMakeContextCurrent(doom->audio.context))
		return (FALSE);
	alGenSources(MAX_SOUNDS, doom->audio.source);
	return (TRUE);
}

t_bool			set_default_sounds(t_doom *doom)
{
	doom->audio.buffer[0] = doom->res_manager.ressources->values[11]
		->data.sound->buffer_id;
	doom->audio.buffer[1] = doom->res_manager.ressources->values[12]
		->data.sound->buffer_id;
	doom->audio.buffer[2] = doom->res_manager.ressources->values[13]
		->data.sound->buffer_id;
	doom->audio.buffer[3] = doom->res_manager.ressources->values[14]
		->data.sound->buffer_id;
	doom->audio.buffer[4] = doom->res_manager.ressources->values[15]
		->data.sound->buffer_id;
	doom->audio.buffer[5] = doom->res_manager.ressources->values[16]
		->data.sound->buffer_id;
	doom->audio.buffer[6] = doom->res_manager.ressources->values[17]
		->data.sound->buffer_id;
	doom->audio.buffer[7] = doom->res_manager.ressources->values[18]
		->data.sound->buffer_id;
	doom->audio.buffer[8] = doom->res_manager.ressources->values[19]
		->data.sound->buffer_id;
	return (TRUE);
}

void			quit_openal(t_doom *doom)
{
	alDeleteSources(MAX_SOUNDS, (const ALuint *)doom->audio.source);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(doom->audio.context);
	alcCloseDevice(doom->audio.device);
}
