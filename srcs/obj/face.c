/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   face.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/25 19:30:54 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/15 17:58:45 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj.h"

t_faces			*create_faces_array(int capacity)
{
	const size_t	size = sizeof(t_faces) + (capacity * sizeof(t_face));
	t_faces			*arr;

	if (capacity <= 0)
		return (NULL);
	if (!(arr = (t_faces*)malloc(size)))
		return (NULL);
	ft_bzero(arr, size);
	arr->capacity = capacity;
	return (arr);
}

t_faces			*append_faces_array(t_faces **arr, t_face i)
{
	t_faces			*new;
	size_t			old_size;

	if ((*arr)->len == (*arr)->capacity)
	{
		old_size = sizeof(t_faces) + ((*arr)->capacity * sizeof(t_face));
		(*arr)->capacity *= 2;
		if (!(new = create_faces_array((*arr)->capacity)))
			return (NULL);
		ft_memcpy(new, *arr, old_size);
		free(*arr);
		*arr = new;
	}
	(*arr)->values[(*arr)->len++] = i;
	return (*arr);
}

t_bool		read_face_part(t_reader *r, size_t i, t_face *face)
{
	size_t	j;
	char	v;

	j = 0;
	while ((v = io_peek(r)) == '/' || j == 0)
	{
		if (v == '/')
			io_next(r);
		if (io_peek(r) == '/')
		{
			j++;
			continue;
		}
		if (j == 0 && !io_readnum(r, &face->vertices_index[i]))
			return (FALSE);
		if (j == 1 && (face->vertex_set = TRUE)
			&& !io_readnum(r, &face->vertex_index[i]))
			return (FALSE);
		if (j == 2 && (face->normals_set = TRUE)
			&& !io_readnum(r, &face->normals_index[i]))
			return (FALSE);
		j++;
	}
	return (TRUE);
}

t_bool		face_formatter(t_obj *obj, t_reader *reader)
{
	t_face	face;
	char	c;
	size_t	i;

	ft_bzero(&face, sizeof(t_face));
	i = 0;
	while ((c = io_peek(reader)) == ' ')
	{
		io_next(reader);
		if (!read_face_part(reader, i, &face))
			return (FALSE);
		if (++i > 3)
			break;
	}
	if (i != 3)
	{
		ft_putstr("f: Faces need to be triangle\n");
		return (FALSE);
	}
	if (obj->current_mtl != -1)
		face.mtl = &obj->materials->values[obj->current_mtl];
	append_faces_array(&obj->faces, face);
	return (TRUE);
}
