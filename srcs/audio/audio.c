/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 13:41:15 by lloncham          #+#    #+#             */
/*   Updated: 2020/04/18 01:13:45 by llelievr         ###   ########.fr       */
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
	t_ressources	*res;

	res = doom->res_manager.ressources;
	doom->audio.buffer[0] = res->values[11]->data.sound->buffer_id;
	doom->audio.buffer[1] = res->values[12]->data.sound->buffer_id;
	doom->audio.buffer[2] = res->values[13]->data.sound->buffer_id;
	doom->audio.buffer[3] = res->values[14]->data.sound->buffer_id;
	doom->audio.buffer[4] = res->values[15]->data.sound->buffer_id;
	doom->audio.buffer[5] = res->values[16]->data.sound->buffer_id;
	doom->audio.buffer[6] = res->values[17]->data.sound->buffer_id;
	doom->audio.buffer[7] = res->values[18]->data.sound->buffer_id;
	doom->audio.buffer[8] = res->values[19]->data.sound->buffer_id;
	doom->audio.buffer[9] = res->values[27]->data.sound->buffer_id;
	doom->audio.buffer[10] = res->values[28]->data.sound->buffer_id;
	doom->audio.buffer[11] = res->values[29]->data.sound->buffer_id;
	doom->audio.buffer[12] = res->values[30]->data.sound->buffer_id;
	return (TRUE);
}

void			quit_openal(t_doom *doom)
{
	alDeleteSources(MAX_SOUNDS, (const ALuint *)doom->audio.source);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(doom->audio.context);
	alcCloseDevice(doom->audio.device);
}
