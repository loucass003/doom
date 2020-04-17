/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 01:15:20 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/16 14:32:36 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUDIO_H
# define AUDIO_H

# include <libft.h>
# include <SDL.h>
# include <al.h>
# include <alc.h>
# include "constants.h"
# include "entity.h"

typedef struct			s_audio
{
	ALuint				buffer[13];
	ALuint				source[MAX_SOUNDS];
	t_bool				source_status[MAX_SOUNDS];
	ALCdevice			*device;
	ALCcontext			*context;
}						t_audio;

typedef struct			s_riff_header
{
	uint32_t			chunk_size;
	int8_t				format[4];
}						t_riff_header;

typedef struct			s_wav_fmt_chunk
{
	uint32_t			subchunk_size;
	uint16_t			format;
	uint16_t			chanels;
	uint32_t			sample_rate;
	uint32_t			byte_rate;
	uint16_t			block_align;
	uint16_t			bits_per_sample;
}						t_wav_fmt_chunk;

typedef struct			s_wav_format
{
	t_riff_header		head;
	t_wav_fmt_chunk		fmt;
	uint32_t			buffer_size;
	uint8_t				*buffer;
}						t_wav_format;

typedef struct			s_sound
{
	ALuint				buffer_id;
	uint32_t			sample_rate;
	uint16_t			bits_per_sample;
	uint32_t			buffer_size;
	Uint8				buffer[];
}						t_sound;

t_bool					init_openal(struct s_doom *doom);
void					quit_openal(struct s_doom *doom);
void					entity_sound(struct s_entity *s, int buffer, int source,
							float peach);
void					player_sound(t_audio *s, int source, int buffer,
							float peach);
t_bool					wav_return_error(char *error);
t_bool					load_wav(char *path, t_wav_format *wav);
t_bool					free_wav(t_wav_format **wav);
t_bool					set_default_sounds(struct s_doom *doom);
int						get_source(t_audio *s);
void					play_music(t_audio *s, t_vec3 pos, int idb,
	t_bool glob);

#endif
