/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 15:26:39 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/24 14:23:56 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJ_H
# define OBJ_H

# define OBJ_BUFFER 10
# include "doom.h" 

typedef struct	s_string_buffer
{
	size_t		len;
	size_t		capacity;
	char		*content;
}				t_string_buffer;

typedef struct	s_reader
{
	char		*buffer[OBJ_BUFFER];
	size_t		pos;
	size_t		len;
	size_t		fd;
}				t_reader;

typedef struct	s_obj
{
	t_vec4		*vertices;
	size_t		vertices_count;
	int			*indices;
	size_t		indices_count;
	t_vec2		*vertex;
	size_t		vertex_count;
	t_vec3		normals;
	size_t		normals_count;
}				t_obj;

typedef struct	s_obj_prefix
{
	char		*prefix;
	void		(*formatter)(t_obj *obj, t_reader *state);
}				t_obj_prefix;

char			io_peek(t_reader *r);
t_bool			io_expect(t_reader *r, const char *str);
t_bool			create_str_buffer(t_string_buffer *buffer, size_t capacity);
t_bool			append_str_buffer(t_string_buffer *buffer, char c);
void			skip_ws(t_reader *r);

#endif
