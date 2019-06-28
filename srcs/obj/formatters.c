/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   formatters.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/26 14:34:00 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/28 03:48:03 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj.h"

static int		get_group(t_obj *obj, char *name, size_t len)
{
	size_t		i;

	if (!name)
		return (-1);
	i = 0;
	while (i < obj->groups_count)
	{
		if (len == ft_strlen(obj->groups[i]) 
			&& ft_strncmp(obj->groups[i], name, len) == 0)
			return (i);
		i++;
	}
	return (-1);
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
	char		name[GROUPS_NAME_LEN];
	size_t		len;
	char		c;

	len = 0;
	io_next(reader);
	while ((c = io_peek(reader)) != -1 && c != '\n' && len < GROUPS_NAME_LEN)
	{
		io_next(reader);
		name[len++] = c;
	}
	if (c == '\n' && len == GROUPS_NAME_LEN)
	{
		ft_putendl("Max name length exeed");
		return (FALSE);
	}
	if ((obj->current_group = get_group(obj, name, len)) != -1)
		return (TRUE);
	if ((obj->current_group = obj->groups_count++) > GROUPS_MAX)
	{
		ft_putendl("Max group declaration exeed");
		return (FALSE);
	}
	ft_strncpy(obj->groups[obj->current_group], name, len);
	return (TRUE);
}