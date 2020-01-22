/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 13:41:15 by lloncham          #+#    #+#             */
/*   Updated: 2020/01/22 14:17:13 by louali           ###   ########.fr       */
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
		printf("context error\n");
	if (!alcMakeContextCurrent(doom->audio.context))
		printf("context current error\n");
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

void			quit_openal(void)
{
	ALCcontext	*context;
	ALCdevice	*device;

	context = alcGetCurrentContext();
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}
