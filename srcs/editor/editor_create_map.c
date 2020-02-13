/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_create_map.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 13:41:47 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/13 11:57:04 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "editor.h"
#include "doom.h"

t_bool			post_process_map(t_editor *editor,
t_renderable *r, t_bool replace)
{
	post_process_renderable(editor->doom, r, TRUE, replace);
	r->scale = (t_vec3){ 1, 1, 1 };
	r->dirty = TRUE;
	r->visible = TRUE;
	r->wireframe_color = 0xFFFF0000;
	return (TRUE);
}

t_bool			create_map(t_renderable *r, t_editor *editor)
{
	if (!create_renderable(r, RENDERABLE_MAP))
		return (FALSE);
	create_map_points_and_floor(editor, r);
	create_walls(editor, r);
	post_process_map(editor, r, TRUE);
	r->dirty = TRUE;
	return (TRUE);
}

t_renderable	*get_map(t_editor *editor)
{
	if (editor->map_renderable == -1)
		return (NULL);
	return (&editor->doom->renderables->values[editor->map_renderable]);
}
