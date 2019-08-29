/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 13:41:15 by lloncham          #+#    #+#             */
/*   Updated: 2019/08/29 02:42:13 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <al.h>
#include <alc.h>
#include <sndfile.h>
#include "doom.h"

t_bool			init_openal(t_doom *doom)
{
	ALCdevice *device;
	ALCcontext *context;
	const ALCchar *alldevice;

	alldevice = alcGetString(NULL, ALC_DEVICE_SPECIFIER);

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
	return (TRUE);
}

void			load_wav(ALCchar *device) 
{
	SF_INFO fileinfo;
	SNDFILE *file;
	const char *filename;
	int	fs;

	filename = "song/song0.wav";
	if (!(file = sf_open(filename, SFM_READ, &fileinfo)))
	{
		printf("open audio fail\n");
		return;
	}
	sf_close(file);
	fs = fileinfo.samplerate;
	printf("Sample Rate = %d Hz\n", fs);
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