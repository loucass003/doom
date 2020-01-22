/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wav_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/18 16:42:15 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/22 14:19:01 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <fcntl.h>
#include <unistd.h>
#include "io.h"
#include "audio.h"

static t_bool		read_riff_header(t_reader *r, t_riff_header *head)
{
	if (!io_skip_until(r, "RIFF"))
		return (wav_return_error("No riff chunk"));
	if (!io_memcpy(r, head, sizeof(t_riff_header)))
		return (wav_return_error(ERR_INV_READ_SIZE));
	if (ft_memcmp(head->format, "WAVE", 4) != 0)
		return (wav_return_error("Invalid file format, not a wav file"));
	return (TRUE);
}

static t_bool		read_fmt_chunk(t_reader *r, t_wav_fmt_chunk *fmt)
{
	if (!io_skip_until(r, "fmt "))
		return (wav_return_error("No fmt cunk"));
	if (!io_memcpy(r, fmt, sizeof(t_wav_fmt_chunk)))
		return (wav_return_error(ERR_INV_READ_SIZE));
	if (fmt->subchunk_size != 16)
		return (wav_return_error("Invalid file format, not a pcm file"));
	return (TRUE);
}

static t_bool		read_data_chunk(t_reader *r, t_wav_format *wav)
{
	if (!io_skip_until(r, "data"))
		return (wav_return_error("No data chunk"));
	if (!io_memcpy(r, &wav->buffer_size, sizeof(uint32_t)))
		return (wav_return_error(ERR_INV_READ_SIZE));
	if (!(wav->buffer = malloc(sizeof(uint8_t) * wav->buffer_size)))
		return (wav_return_error("Unable to alloc buffer"));
	if (!io_memcpy(r, wav->buffer, sizeof(uint8_t) * wav->buffer_size))
		return (wav_return_error(ERR_INV_READ_SIZE));
	return (TRUE);
}

t_bool				load_wav(char *path, t_wav_format *wav)
{
	t_reader		r;

	r = (t_reader) { .pos = 0 };
	if ((r.fd = open(path, O_RDONLY, 0666)) == -1)
		return (wav_return_error("Unable to open file"));
	ft_bzero(wav, sizeof(t_wav_format));
	if (!read_riff_header(&r, &wav->head))
		return (wav_return_error("Invalid RIFF header"));
	if (!read_fmt_chunk(&r, &wav->fmt))
		return (wav_return_error("Invalid fmt chunk"));
	if (!read_data_chunk(&r, wav))
	{
		free_wav(&wav);
		return (wav_return_error("Invalid data chunk"));
	}
	close(r.fd);
	return (TRUE);
}

t_bool				free_wav(t_wav_format **wav)
{
	if ((*wav)->buffer)
		ft_memdel((void **)&(*wav)->buffer);
	*wav = NULL;
	return (TRUE);
}
