/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 10:06:09 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/26 16:24:15 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IO_H
# define IO_H

# include <libft.h>
# define READ_BUFFER (4096)

typedef struct	s_reader
{
	uint8_t		buffer[READ_BUFFER];
	size_t		pos;
	size_t		len;
	size_t		total_read;
	int			fd;
}				t_reader;

t_bool			io_peek(t_reader *r, char *c);
void			io_next(t_reader *r);
t_bool			io_expect(t_reader *r, const char *str);
t_bool			io_skip_until(t_reader *r, char *str);
t_bool			io_readfloat(t_reader *r, float *out);
t_bool			io_readnum(t_reader *r, int32_t *out);
void			io_skip_ws(t_reader *r);
void			io_skip_empty(t_reader *r);
t_bool			io_memcpy(t_reader *r, void *dst, size_t n);

#endif
