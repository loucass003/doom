/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 13:41:15 by lloncham          #+#    #+#             */
/*   Updated: 2019/09/10 15:59:58 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <al.h>
#include <alc.h>
#include "doom.h"


typedef struct s_audio
{
	Mix_Chunk *music;
}				t_audio;

void			init_SDL_mixer(t_doom *doom)
{
	ALuint		buffer; 
	ALuint		source;
	Mix_Chunk	*music;

	ALCdevice 	*device;
	ALCcontext 	*context;

	device = alcOpenDevice(NULL);
	if (!(context = alcCreateContext(device, NULL)))
		printf("context error\n");
	if (!alcMakeContextCurrent(context))
		printf("context current error\n");

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
		printf("%s", Mix_GetError());
	// Mix_AllocateChannels(2);
	// Mix_Volume(1, MIX_MAX_VOLUME / 2);
	if ((music = Mix_LoadWAV("song/song0.wav")) == NULL)
		printf("Merde\n");
	// Mix_VolumeChunk(music, MIX_MAX_VOLUME / 2);
	// Mix_PlayChannel(1, music, -1);
	Mix_CloseAudio();

	source = AL_FORMAT_MONO16;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, AL_FORMAT_STEREO16, music->abuf, music->alen, 44100);

	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer);
	alSourcePlay(source);
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