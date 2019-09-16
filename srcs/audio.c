/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 13:41:15 by lloncham          #+#    #+#             */
/*   Updated: 2019/09/16 16:58:36 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void			play_music(t_audio *s, t_vec3 pos, int id)
{
	alSourcei(s->source[id], AL_BUFFER, s->buffer[id]);
	alSourcei(s->source[id], AL_SOURCE_RELATIVE, AL_TRUE);
	alSourcef(s->source[id], AL_PITCH, 2); //RAPIDITÉ
	alSourcef(s->source[id], AL_GAIN, 1);
	alSource3f(s->source[id], AL_POSITION, pos.x, pos.y, pos.z);
	alSourcei(s->source[id], AL_LOOPING, AL_FALSE);
	alSourcePlay(s->source[id]);

	while (pos.x < 100)
	{
		pos.x += 0.03f;
		printf("pos.x = %f\n", pos.x);
		alSource3f(s->source[id], AL_POSITION, pos.x, pos.y, pos.z); // DEFINIR POS de l'ecouteur
	}
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
	Mix_CloseAudio();
}

t_bool			init_openal(t_doom *doom)
{
	ALfloat listenerOri[] = {1.f, 0.f, 1.f, 0.f, 1.f, 0.f};
	ALfloat listenerPos[]={20.f, 0.f, 20.f};
	ALfloat listenerVel[]={0.f, 0.f, 0.f};

	doom->audio.device = alcOpenDevice(NULL);
	if (!(doom->audio.context = alcCreateContext(doom->audio.device, NULL)))
		printf("context error\n");
	if (!alcMakeContextCurrent(doom->audio.context))
		printf("context current error\n");

	init_SDL_mixer(doom);
	
	alGenBuffers(5, &doom->audio.buffer);
	alBufferData(doom->audio.buffer[0], AL_FORMAT_MONO16, doom->audio.music[0]->abuf, doom->audio.music[0]->alen, 44100);
	alBufferData(doom->audio.buffer[1], AL_FORMAT_MONO16, doom->audio.music[1]->abuf, doom->audio.music[1]->alen, 44100);
	alBufferData(doom->audio.buffer[2], AL_FORMAT_MONO16, doom->audio.music[2]->abuf, doom->audio.music[2]->alen, 44100);
	alBufferData(doom->audio.buffer[3], AL_FORMAT_MONO16, doom->audio.music[3]->abuf, doom->audio.music[3]->alen, 44100);
	alBufferData(doom->audio.buffer[4], AL_FORMAT_MONO16, doom->audio.music[4]->abuf, doom->audio.music[4]->alen, 44100);
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);
	
	t_vec3 pos;
	pos.x = 1;
	pos.y = 1;
	pos.z = 1;
	alGenSources(5, &doom->audio.source);
	play_music(&doom->audio, pos, 0);
	// play_music(&doom->audio, 2, 1);
	// alSourceStop, alSourcePause, et alSourceRewind
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
