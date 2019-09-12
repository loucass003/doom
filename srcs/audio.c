/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 13:41:15 by lloncham          #+#    #+#             */
/*   Updated: 2019/09/12 15:33:43 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "doom.h"

void			play_music(t_audio *s, int ecouteur, int id)
{
	int droite;
	int gauche;
	int tout;

	droite = 0;
	gauche = 0;
	tout = 0;
	if (ecouteur == 0) // all
		tout = 1;
	if (ecouteur == 1) // droite
		droite = 1;
	if (ecouteur == 2) // gauche
		gauche = 1;
	alSourcei(s->source[id], AL_BUFFER, s->buffer[id]);
	alSourcei(s->source[id], AL_SOURCE_RELATIVE, AL_TRUE);
	alSourcef(s->source[id], AL_PITCH, 2); //RAPIDITÉ
	alSourcef(s->source[id], AL_GAIN, 1);
	alSource3f(s->source[id], AL_POSITION, droite, tout, gauche); // DEFINIR POS de l'ecouteur
	alSourcei(s->source[id], AL_LOOPING, AL_FALSE);
	alSourcePlay(s->source[id]);
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
	alListener3f(AL_POSITION, 20.f, 0.f, 20.f);
	alListener3f(AL_VELOCITY, 0.f, 0.f, 0.f);
	alListenerfv(AL_ORIENTATION, listenerOri);
	
	alGenSources(5, &doom->audio.source);
	play_music(&doom->audio, 1, 0);
	play_music(&doom->audio, 2, 1);
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
