/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sounds.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/22 14:16:52 by louali            #+#    #+#             */
/*   Updated: 2020/01/22 14:17:46 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "audio.h"
#include "entity.h"

void			entity_sound(t_entity *s, int buffer, int source, int peach)
{
	alSourcei(s->sources[source], AL_BUFFER,
		s->packet.doom->audio.buffer[buffer]);
	alSourcei(s->sources[source], AL_SOURCE_RELATIVE, 0);
	alSourcef(s->sources[source], AL_PITCH, peach * 2.0);
	alSourcef(s->sources[source], AL_GAIN, 7);
	alSourcei(s->sources[source], AL_DISTANCE_MODEL,
		AL_LINEAR_DISTANCE_CLAMPED);
	alSource3f(s->sources[source], AL_POSITION, s->position.x,
		s->position.y, s->position.z);
	alSourcei(s->sources[source], AL_LOOPING, AL_FALSE);
	alSourcePlay(s->sources[source]);
}

void			player_sound(t_audio *s, int source, int buffer, float peach)
{
	alSourcei(s->source[source], AL_BUFFER, s->buffer[buffer]);
	alSourcei(s->source[source], AL_SOURCE_RELATIVE, 1);
	alSourcef(s->source[source], AL_PITCH, peach * 2.0);
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
	alSourcef(s->source[ids], AL_PITCH, 2);
	alSourcef(s->source[ids], AL_GAIN, 1 + (!glob * 14));
	alSourcei(s->source[ids], AL_DISTANCE_MODEL, AL_LINEAR_DISTANCE_CLAMPED);
	alSource3f(s->source[ids], AL_POSITION, pos.x, pos.y, pos.z);
	alSourcei(s->source[ids], AL_LOOPING, AL_FALSE);
	alSourcePlay(s->source[ids]);
	s->source_status[ids] = TRUE;
}
