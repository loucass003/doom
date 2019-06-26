/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   formatters.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/26 14:34:00 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/26 17:04:56 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj.h"

t_obj_prefix	*get_formatter(t_obj_prefix *prefixes, t_reader *reader)
{
	char		name[20];
	char		c;
	size_t		len;
	size_t		i;

	io_skip_ws(reader);
	len = 0;
	while (len < 20 && (c = io_peek(reader)) != -1 && c != ' ' && c != '\n')
	{
		io_next(reader);
		name[len++] = c;
	}
	if (c != ' ')
		return (NULL);
	i = 0;
	while (i < PREFIXES_COUNT)
	{
		if (ft_strlen(prefixes[i].prefix) == len && ft_strncmp(prefixes[i].prefix, name, len) == 0)
			return (prefixes + i);
		i++;
	}
	return (NULL);
}

t_bool			vertice_formatter(t_obj *obj, t_reader *reader)
{
	char		c;
	t_vec4_u	vertice;
	int			i;
	
	vertice.v.w = 1;
	i = 0;
	while ((c = io_peek(reader)) == ' ')
	{
		io_next(reader);
		if(!io_readfloat(reader, &vertice.a[i]))
			return (FALSE);
		if (++i >= 4)
			return (FALSE);
	}
	if (i < 3)
		return (FALSE);
	append_4dvertices_array(&obj->vertices, vertice.v);
	printf("VERTICE %f %f %f %f\n", vertice.v.x, vertice.v.y, vertice.v.z, vertice.v.w);
	return (TRUE);
}

t_bool			normal_formatter(t_obj *obj, t_reader *reader)
{
	char		c;
	t_vec3_u	normal;
	int			i;
	
	i = 0;
	while ((c = io_peek(reader)) == ' ')
	{
		io_next(reader);
		if(!io_readfloat(reader, &normal.a[i]))
			return (FALSE);
		if (++i >= 3)
			return (FALSE);
	}
	if (i != 3)
		return (FALSE);
	append_3dvertices_array(&obj->normals, normal.v);
	printf("NORMAL %f %f %f\n", normal.v.x, normal.v.y, normal.v.z);
	return (TRUE);
}

t_bool			vertex_formatter(t_obj *obj, t_reader *reader)
{
	char		c;
	t_vec2_u	vertex;
	int			i;
	
	i = 0;
	while ((c = io_peek(reader)) == ' ')
	{
		io_next(reader);
		if(!io_readfloat(reader, &vertex.a[i]))
			return (FALSE);
		if (++i >= 2)
			return (FALSE);
	}
	if (i != 2)
		return (FALSE);
	append_2dvertices_array(&obj->vertex, vertex.v);
	printf("VERTEX %f %f\n", vertex.v.x, vertex.v.y);
	return (TRUE);
}

t_bool			group_formatter(t_obj *obj, t_reader *reader)
{
	
}