/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 13:41:15 by lloncham          #+#    #+#             */
/*   Updated: 2020/01/17 14:41:59 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

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

void			entity_sound(t_entity *s, int buffer, int source, int peach)
{
	alSourcei(s->sources[source], AL_BUFFER, s->packet.doom->audio.buffer[buffer]);
	alSourcei(s->sources[source], AL_SOURCE_RELATIVE, 0);
	alSourcef(s->sources[source], AL_PITCH, peach); //RAPIDITÉ
	alSourcef(s->sources[source], AL_GAIN, 15);
	alSourcei(s->sources[source], AL_DISTANCE_MODEL, AL_LINEAR_DISTANCE_CLAMPED);
	alSource3f(s->sources[source], AL_POSITION, s->position.x, s->position.y, s->position.z);
	alSourcei(s->sources[source], AL_LOOPING, AL_FALSE);
	alSourcePlay(s->sources[source]);
}

void			player_sound(t_audio *s, int source, int buffer, float peach)
{
	alSourcei(s->source[source], AL_BUFFER, s->buffer[buffer]);
	alSourcei(s->source[source], AL_SOURCE_RELATIVE, 1);
	alSourcef(s->source[source], AL_PITCH, peach); //RAPIDITÉ
	alSourcef(s->source[source], AL_GAIN, 1);
	alSource3f(s->source[source], AL_POSITION, 0, 0, 0);
	alSourcei(s->source[source], AL_LOOPING, AL_FALSE);
	alSourcePlay(s->source[source]);
	s->source_status[source] = TRUE;
}

void			play_music(t_audio *s, t_vec3 pos, int idb, t_bool glob)
{
	int		ids;

	if ((ids = get_source(s)) == -1)
		return ;
	alSourcei(s->source[ids], AL_BUFFER, s->buffer[idb]);
	alSourcei(s->source[ids], AL_SOURCE_RELATIVE, glob);
	alSourcef(s->source[ids], AL_PITCH, 2); //RAPIDITÉ
	alSourcef(s->source[ids], AL_GAIN, 1 + (!glob * 14));
	alSourcei(s->source[ids], AL_DISTANCE_MODEL, AL_LINEAR_DISTANCE_CLAMPED);
	alSource3f(s->source[ids], AL_POSITION, pos.x, pos.y, pos.z);
	alSourcei(s->source[ids], AL_LOOPING, AL_FALSE);
	alSourcePlay(s->source[ids]);
	s->source_status[ids] = TRUE;
}


t_bool			init_openal(t_doom *doom)
{
	doom->audio.device = alcOpenDevice(NULL);
	if (!(doom->audio.context = alcCreateContext(doom->audio.device, NULL)))
		printf("context error\n");
	if (!alcMakeContextCurrent(doom->audio.context))
		printf("context current error\n");
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
		printf("%s\n", Mix_GetError());
	alGenSources(MAX_SOUNDS, doom->audio.source);
	return (TRUE);
}

t_bool			set_default_sounds(t_doom *doom)
{
	doom->audio.buffer[0] = doom->res_manager.ressources->values[11]->data.sound->buffer_id;
	doom->audio.buffer[1] = doom->res_manager.ressources->values[12]->data.sound->buffer_id;
	doom->audio.buffer[2] = doom->res_manager.ressources->values[13]->data.sound->buffer_id;
	doom->audio.buffer[3] = doom->res_manager.ressources->values[14]->data.sound->buffer_id;
	doom->audio.buffer[4] = doom->res_manager.ressources->values[15]->data.sound->buffer_id;
	doom->audio.buffer[5] = doom->res_manager.ressources->values[16]->data.sound->buffer_id;
	doom->audio.buffer[6] = doom->res_manager.ressources->values[17]->data.sound->buffer_id;
	doom->audio.buffer[7] = doom->res_manager.ressources->values[18]->data.sound->buffer_id;
}

void            quit_openal()
{
    ALCcontext  *context;
    ALCdevice   *device;
	
    context = alcGetCurrentContext();
    device = alcGetContextsDevice(context);

    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
}
