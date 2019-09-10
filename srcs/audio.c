/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 13:41:15 by lloncham          #+#    #+#             */
/*   Updated: 2019/09/10 13:29:20 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <al.h>
#include <alc.h>
#include "doom.h"


typedef struct s_audio
{
	Mix_Chunk *sound;
	Mix_Chunk *autre;
}				t_audio;

void			init_SDL_mixer()
{
	ALuint		buffer; 
	ALuint		source;
	Mix_Chunk	*sound;

	ALCdevice *device;
	ALCcontext *context;

	device = alcOpenDevice(NULL);
	if (!(context = alcCreateContext(device, NULL)))
		printf("context error\n");
	if (!alcMakeContextCurrent(context))
		printf("context current error\n");

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
		printf("%s", Mix_GetError());
	Mix_AllocateChannels(2);
	Mix_Volume(1, MIX_MAX_VOLUME / 2);
	if ((sound = Mix_LoadWAV("song/song0.wav")) == NULL)
		printf("Merde\n");
	Mix_VolumeChunk(sound, MIX_MAX_VOLUME / 2);
	// Mix_PlayChannel(1, sound, -1);


	source = AL_FORMAT_MONO16;
	alGenBuffers(1, &buffer); 
	alBufferData(buffer, AL_FORMAT_STEREO16, sound->abuf, sound->alen, 44100);
	alGenSources(1, &source); 
	alSourcei(source, AL_BUFFER, buffer); 
	alSourcePlay(source); 
	printf("ici\n");
	// alGenSources(1, &sourceID); 
	// alSourcef(source, AL_PITCH, 1.0f); 
	// alSourcef(source, AL_GAIN, 1.0f); 

	// if(loop) 
	// 	alSourcei(source[num], AL_LOOPING, AL_TRUE); 
	// else 
	// 	alSourcei(source[num], AL_LOOPING, AL_FALSE); 
	// Mix_CloseAudio();
}

t_bool			init_openal(t_doom *doom)
{
	// ALCdevice *device;
	// ALCcontext *context;

	// device = alcOpenDevice(NULL);
	// if (!(context = alcCreateContext(device, NULL)))
	// {
	// 	printf("context error\n");
	// 	return (FALSßßßßE);
	// }
	// if (!alcMakeContextCurrent(context))
	// {
	// 	printf("context current error\n");
	// 	return (FALSE);
	// }
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