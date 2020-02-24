/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressource_sound.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 16:55:56 by louali            #+#    #+#             */
/*   Updated: 2020/02/24 14:52:30 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ressource.h"
#include <SDL_mixer.h>
#include <al.h>
#include "write_structs.h"

t_bool		gen_audio_buffer(t_sound *s)
{
	ALenum	format;

	alGenBuffers(1, &s->buffer_id);
	format = AL_FORMAT_MONO16;
	if (s->bits_per_sample == 8)
		format = AL_FORMAT_MONO8;
	alBufferData(s->buffer_id, format, s->buffer, s->buffer_size,
		s->sample_rate);
	return (TRUE);
}

t_bool		load_sound(t_ressource *r, char *path)
{
	t_wav_format	wav;

	if (!load_wav(path, &wav))
		return (FALSE);
	if (!(r->data.sound = malloc(sizeof(t_sound) + wav.buffer_size)))
		return (FALSE);
	ft_memcpy(r->data.sound->buffer, wav.buffer, wav.buffer_size);
	r->data.sound->buffer_size = wav.buffer_size;
	r->data.sound->sample_rate = wav.fmt.sample_rate;
	r->data.sound->bits_per_sample = wav.fmt.bits_per_sample;
	gen_audio_buffer(r->data.sound);
	r->loaded = TRUE;
	return (TRUE);
}

t_bool		write_sound(t_ressource_manager *rm, t_sound *sound)
{
	t_wr_songs	wr_song;

	wr_song = (t_wr_songs){
		.buffer_size = sound->buffer_size,
		.sample_rate = sound->sample_rate,
		.bits_per_sample = sound->bits_per_sample};
	dp_write(rm, &wr_song, sizeof(t_wr_songs));
	dp_write(rm, sound->buffer, sound->buffer_size);
	return (TRUE);
}

t_bool		read_songs(t_ressource_manager *r, t_sound **sound)
{
	t_wr_songs	wr_songs;
	t_sound		*s;

	if (!io_memcpy(&r->reader, &wr_songs, sizeof(t_wr_songs)))
		return (FALSE);
	if (!(s = malloc(sizeof(t_sound) + wr_songs.buffer_size)))
		return (FALSE);
	s->buffer_size = wr_songs.buffer_size;
	s->sample_rate = wr_songs.sample_rate;
	s->bits_per_sample = wr_songs.bits_per_sample;
	if (!io_memcpy(&r->reader, s->buffer, wr_songs.buffer_size))
		return (FALSE);
	gen_audio_buffer(s);
	*sound = s;
	return (TRUE);
}

void		free_sound(t_sound **s_addr)
{
	ft_memdel((void **)s_addr);
}
