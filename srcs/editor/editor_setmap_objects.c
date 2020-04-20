/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_setmap_objects.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/21 00:24:16 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/21 00:25:30 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "render.h"
#include "doom.h"

t_bool		add_objects(t_editor *editor)
{
	t_object		*object;
	t_renderable	r;
	int				i;
	int				start_objects;

	start_objects = editor->doom->renderables->len;
	i = -1;
	while (++i < editor->objects->len)
	{
		if (editor->objects->values[i].type == OBJECT_NONE)
			continue ;
		if (!create_object_renderable(editor, i, &r)
			|| !append_renderables_array(&editor->doom->renderables, r))
			return (FALSE);
	}
	i = -1;
	while (++i < editor->objects->len)
	{
		if (object->type == OBJECT_NONE)
			continue ;
		object = &editor->objects->values[i];
		object->r = &editor->doom->renderables->values[start_objects + i];
		object->r->has_hitbox ? object->r->show_hitbox = FALSE : 0;
	}
	return (TRUE);
}
