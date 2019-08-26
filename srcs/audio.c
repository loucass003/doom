/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 13:41:15 by lloncham          #+#    #+#             */
/*   Updated: 2019/08/26 11:30:44 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_bool			init_openal(t_doom *doom)
{
	ALCdevice *device;
	ALCcontext *context;

	device = alcOpenDevice(NULL); //remplacer NULL
	context = alcCreateContext(device, NULL);
	if (!context)
		return (FALSE);
	if (!alcMakeContextCurrent(context))
		return (FALSE);
	return (TRUE);
}

void			load_wav()
{
	const ALCchar *alldevice;

	alldevice = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
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