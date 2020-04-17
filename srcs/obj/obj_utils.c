/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 20:15:24 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/14 20:18:14 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj.h"
#include "doom.h"

t_bool			free_obj(t_obj *obj, t_bool ret)
{
	(void)obj;
	return (ret);
}

t_bool			set_obj_working_dir(t_doom *doom, char *folder)
{
	if (doom->obj_working_dir)
		ft_memdel((void **)&doom->obj_working_dir);
	if (!(doom->obj_working_dir = (char *)ft_strnew(ft_strlen(folder))))
		return (FALSE);
	ft_strcpy(doom->obj_working_dir, folder);
	return (TRUE);
}

t_bool			init_obj_renderable_arrays(t_renderable *r)
{
	if (!(r->vertices = create_4dvertices_array(800)))
		return (free_renderable(r, FALSE, FALSE));
	if (!(r->vertex = create_2dvertices_array(800)))
		return (free_renderable(r, FALSE, FALSE));
	if (!(r->normals = create_3dvertices_array(800)))
		return (free_renderable(r, FALSE, FALSE));
	if (!(r->faces = create_faces_array(800)))
		return (free_renderable(r, FALSE, FALSE));
	if (!(r->materials = create_mtllist(3)))
		return (free_renderable(r, FALSE, FALSE));
	return (TRUE);
}
