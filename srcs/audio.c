/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 13:41:15 by lloncham          #+#    #+#             */
/*   Updated: 2019/08/29 17:10:37 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <al.h>
#include <alc.h>
#include "doom.h"

void			init_SDL_mixer()
{
	ALuint buffer; 
	ALuint source;
	int audio_rate = 22050; 
	Uint16 audio_format = AUDIO_S16SYS; 
	int audio_channels = 2; 
	int audio_buffers = 4096; 

	Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers);
	Mix_Chunk *sound = Mix_LoadWAV("song/song0.wav");
	printf("okok\n");
	if (sound == NULL)
	{
		printf("Merde\n");
		return;
	}
	// alGenBuffers(1, buffer); 
	// alGenSources(1, source); 
	// alBufferData(buffer, AL_FORMAT_STEREO16, sound->abuf, sound->alen, 44100); 

	// alGenSources(1, &sourceID); 
	// alSourcei(source, AL_BUFFER, buffer); 
	// alSourcef(source, AL_PITCH, 1.0f); 
	// alSourcef(source, AL_GAIN, 1.0f); 

	// if(loop) 
	// 	alSourcei(source[num], AL_LOOPING, AL_TRUE); 
	// else 
	// 	alSourcei(source[num], AL_LOOPING, AL_FALSE); 
	alSourcePlay(source); 
}

t_bool			init_openal(t_doom *doom)
{
	ALCdevice *device;
	ALCcontext *context;

	device = alcOpenDevice(NULL);
	if (!(context = alcCreateContext(device, NULL)))
	{
		printf("context error\n");
		return (FALSE);
	}
	if (!alcMakeContextCurrent(context))
	{
		printf("context current error\n");
		return (FALSE);
	}
	init_SDL_mixer();
	return (TRUE);
	
}


void			load_wav(ALCchar *device) 
{
	// SF_INFO fileinfo;
	// SNDFILE *file;
	// const char *filename;
	// int	fs;

	// filename = "song/song0.wav";
	// if (!(file = sf_open(filename, SFM_READ, &fileinfo)))
	// {
	// 	printf("open audio fail\n");
	// 	return;
	// }
	// sf_close(file);
	// fs = fileinfo.samplerate;
	// printf("Sample Rate = %d Hz\n", fs);
	// ALsizei filenb = (ALsizei)(fileinfo.channels * fileinfo.frames);
	// ALsizei onefile = (ALsizei)(fileinfo.samplerate);
	// if (st_read_short(file, ,filenb) < filenb)
	// 	return;
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