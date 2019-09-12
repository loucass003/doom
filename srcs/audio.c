/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 13:41:15 by lloncham          #+#    #+#             */
/*   Updated: 2019/09/12 01:58:04 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "doom.h"

// void			init_SDL_mixer(t_doom *doom)
// {
// 	ALuint		buffer; 
// 	ALuint		source;
// 	Mix_Chunk	*music;

// 	ALCdevice 	*device;
// 	ALCcontext 	*context;

// 	device = alcOpenDevice(NULL);
// 	if (!(context = alcCreateContext(device, NULL)))
// 		printf("context error\n");
// 	if (!alcMakeContextCurrent(context))
// 		printf("context current error\n");

// 	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
// 		printf("%s", Mix_GetError());
// 	// Mix_AllocateChannels(2);
// 	// Mix_Volume(1, MIX_MAX_VOLUME / 2);
// 	if ((music = Mix_LoadWAV("song/song5.wav")) == NULL)
// 		printf("Merde\n");
// 	// Mix_VolumeChunk(music, MIX_MAX_VOLUME / 2);
// 	// Mix_PlayChannel(1, music, -1);
// 	Mix_CloseAudio();

// 	alGenBuffers(1, &buffer);
// 	alBufferData(buffer, AL_FORMAT_MONO16, music->abuf, music->alen, 44100);

// 	// ALfloat listenerOri[] = {0.f, 0.f, 1.f, 0.f, 1.f, 0.f};

// 	// alListener3f(AL_POSITION, 20.f, 0.f, 20.f);
// 	// alListener3f(AL_VELOCITY, 0.f, 0.f, 0.f);
// 	// alListenerfv(AL_ORIENTATION, listenerOri);

// 	alGenSources(1, &source);
// 	alSourcei(source, AL_BUFFER, buffer);
// 	// alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE);

// 	alSourcef(source, AL_PITCH, 2); //RAPIDITÉ
// 	alSourcef(source, AL_GAIN, 1);
// 	alSource3f(source, AL_POSITION, 1, 0, 1); // DEFINIR POS de l'ecouteur
// 	alSourcei(source, AL_LOOPING, AL_FALSE);

// 	alSourcePlay(source);

// 	// alSourceStop, alSourcePause, et alSourceRewind
// }

void			init_SDL_mixer(t_doom *doom)
{
	ALfloat listenerOri[] = {0.f, 0.f, 1.f, 0.f, 1.f, 0.f};

	doom->audio.device = alcOpenDevice(NULL);
	if (!(doom->audio.context = alcCreateContext(doom->audio.device, NULL)))
		printf("context error\n");
	if (!alcMakeContextCurrent(doom->audio.context))
		printf("context current error\n");

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
		printf("%s", Mix_GetError());
	if ((doom->audio.music[0] = Mix_LoadWAV("song/song5.wav")) == NULL)
		printf("Merde\n");
	if ((doom->audio.music[1] = Mix_LoadWAV("song/song4.wav")) == NULL)
		printf("Merde\n");
	Mix_CloseAudio();

	alGenBuffers(2, &doom->audio.buffer);
	int i = 0;
	while (i++ < 2)
		alBufferData(doom->audio.buffer[i], AL_FORMAT_MONO16, doom->audio.music[i]->abuf, doom->audio.music[i]->alen, 44100);
	alListener3f(AL_POSITION, 20.f, 0.f, 20.f);
	alListener3f(AL_VELOCITY, 0.f, 0.f, 0.f);
	alListenerfv(AL_ORIENTATION, listenerOri);
	alGenSources(1, &doom->audio.source);
	alSourcei(doom->audio.source, AL_BUFFER, doom->audio.buffer[1]);
	alSourcei(doom->audio.source, AL_SOURCE_RELATIVE, AL_TRUE);
	alSourcef(doom->audio.source, AL_PITCH, 2); //RAPIDITÉ
	alSourcef(doom->audio.source, AL_GAIN, 1);
	alSource3f(doom->audio.source, AL_POSITION, 1, 1, 1); // DEFINIR POS de l'ecouteur
	alSourcei(doom->audio.source, AL_LOOPING, AL_FALSE);
	alSourcePlay(doom->audio.source);

	// alSourceStop, alSourcePause, et alSourceRewind
}

t_bool			init_openal(t_doom *doom)
{
	// ALCdevice *device;
	// ALCcontext *context;

	// device = alcOpenDevice(NULL);
	// if (!(context = alcCreateContext(device, NULL)))
	// 	printf("context error\n");
	// if (!alcMakeContextCurrent(context))
	// 	printf("context current error\n");
	init_SDL_mixer(doom);
	return (TRUE);
}


void			load_wav(ALCchar *device) 
{

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
