/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressource_sound.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 16:55:56 by louali            #+#    #+#             */
/*   Updated: 2020/01/13 18:09:24 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ressource.h"
#include <SDL_mixer.h>
#include <al.h>
#include "write_structs.h"

t_bool      gen_audio_buffer(t_sound *s)
{
    alGenBuffers(1, &s->buffer_id);
	alBufferData(s->buffer_id, AL_FORMAT_MONO16, s->abuf, s->alen, 44100);
    return (TRUE);
}

t_bool		load_sound(t_doom *doom, t_ressource *r, char *path)
{
    Mix_Chunk   *mix_chunk;

   
	if ((mix_chunk = Mix_LoadWAV(path)) == NULL)
		printf("Unable to load sound %s\n", Mix_GetError());
     if (!(r->data.sound = malloc(sizeof(t_sound) + mix_chunk->alen)))
		return (FALSE);
    ft_memcpy(r->data.sound->abuf, mix_chunk->abuf, mix_chunk->alen);
    r->data.sound->alen = mix_chunk->alen;
    gen_audio_buffer(r->data.sound);
    r->loaded = TRUE;
	return (TRUE);
}

t_bool		write_sound(t_ressource_manager *rm, t_sound *sound)
{
	const t_wr_songs	wr_song = (t_wr_songs){ 
		.alen = sound->alen
	};

	dp_write(rm, &wr_song, sizeof(t_wr_songs));
	dp_write(rm, sound->abuf, sound->alen);
    return (TRUE);
}

t_bool		read_songs(t_ressource_manager *r, t_sound **sound)
{
	t_wr_songs wr_songs;
    t_sound    *s;

	if (!io_memcpy(&r->reader, &wr_songs, sizeof(t_wr_songs)))
		return (FALSE);
    if (!(s = malloc(sizeof(t_sound))))
		return (FALSE);
    if (!io_memcpy(&r->reader, s->abuf, wr_songs.alen))
		return (FALSE);
    gen_audio_buffer(s);
    *sound = s;
	return (TRUE);
}