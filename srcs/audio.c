/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 13:41:15 by lloncham          #+#    #+#             */
/*   Updated: 2019/11/07 19:36:23 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

int				get_source(t_audio *s)
{
	int		i;

	i = 0;
	while (i < MAX_SOUNDS)
	{
		if (s->source_status[i] == FALSE)
			return (i);
		++i;
	}
	return (-1);
}

void			play_music(t_audio *s, t_vec3 pos, int idb, t_bool glob)
{
	int		ids;

	if ((ids = get_source(s)) == -1)
		return ;
	// alGenSources(1, &s->sources[id].source);
	// printf("%d\n", ids);
	alSourcei(s->source[ids], AL_BUFFER, s->buffer[idb]);
	alSourcei(s->source[ids], AL_SOURCE_RELATIVE, glob);
	// alSourcei(s->source[ids], AL_MAX_DISTANCE, 200);
	alSourcef(s->source[ids], AL_PITCH, 2); //RAPIDITÉ
	alSourcef(s->source[ids], AL_GAIN, 1);
	alSource3f(s->source[ids], AL_POSITION, pos.x, pos.y, pos.z);
	alSourcei(s->source[ids], AL_LOOPING, AL_FALSE);
	alSourcePlay(s->source[ids]);
	s->source_status[ids] = TRUE;
	
	
	// while (pos.x < 100)
	// {
	// 	pos.x += 0.03f;
	// 	printf("pos.x = %f\n", pos.x);
	// 	alSource3f(s->source[ids], AL_POSITION, pos.x, pos.y, pos.z); // DEFINIR POS de l'ecouteur
	// }
}

void			init_SDL_mixer(t_doom *doom)
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
		printf("%s", Mix_GetError());
	if ((doom->audio.music[0] = Mix_LoadWAV("song/song5.wav")) == NULL)
		printf("Merde\n");
	if ((doom->audio.music[1] = Mix_LoadWAV("song/song3.wav")) == NULL)
		printf("Merde\n");
	if ((doom->audio.music[2] = Mix_LoadWAV("song/song1.wav")) == NULL)
		printf("Merde\n");
	if ((doom->audio.music[3] = Mix_LoadWAV("song/song0.wav")) == NULL)
		printf("Merde\n");
	if ((doom->audio.music[4] = Mix_LoadWAV("song/song4.wav")) == NULL)
		printf("Merde\n");
	if ((doom->audio.music[5] = Mix_LoadWAV("song/wilhelm.wav")) == NULL)
		printf("Merde\n");
	if ((doom->audio.music[6] = Mix_LoadWAV("song/9mm.wav")) == NULL)
		printf("Merde\n");
	if ((doom->audio.music[7] = Mix_LoadWAV("song/380.wav")) == NULL)
		printf("Merde\n");
	Mix_CloseAudio();
}

t_bool			init_openal(t_doom *doom)
{
	doom->audio.device = alcOpenDevice(NULL);
	if (!(doom->audio.context = alcCreateContext(doom->audio.device, NULL)))
		printf("context error\n");
	if (!alcMakeContextCurrent(doom->audio.context))
		printf("context current error\n");

	init_SDL_mixer(doom);
	alGenBuffers(8, doom->audio.buffer);
	alBufferData(doom->audio.buffer[0], AL_FORMAT_MONO16, doom->audio.music[0]->abuf, doom->audio.music[0]->alen, 44100);
	alBufferData(doom->audio.buffer[1], AL_FORMAT_MONO16, doom->audio.music[1]->abuf, doom->audio.music[1]->alen, 44100);
	alBufferData(doom->audio.buffer[2], AL_FORMAT_MONO16, doom->audio.music[2]->abuf, doom->audio.music[2]->alen, 44100);
	alBufferData(doom->audio.buffer[3], AL_FORMAT_MONO16, doom->audio.music[3]->abuf, doom->audio.music[3]->alen, 44100);
	alBufferData(doom->audio.buffer[4], AL_FORMAT_MONO16, doom->audio.music[4]->abuf, doom->audio.music[4]->alen, 44100);
	alBufferData(doom->audio.buffer[5], AL_FORMAT_MONO16, doom->audio.music[5]->abuf, doom->audio.music[5]->alen, 44100);
	alBufferData(doom->audio.buffer[6], AL_FORMAT_MONO16, doom->audio.music[6]->abuf, doom->audio.music[6]->alen, 44100);
	alBufferData(doom->audio.buffer[7], AL_FORMAT_MONO16, doom->audio.music[7]->abuf, doom->audio.music[7]->alen, 44100);
	// alListenerfv(AL_POSITION, listenerPos);
	// alListenerfv(AL_VELOCITY, listenerVel);
	// alListenerfv(AL_ORIENTATION, listenerOri);
	
	t_vec3 pos;
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	alGenSources(MAX_SOUNDS, doom->audio.source);
	return (TRUE);
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
