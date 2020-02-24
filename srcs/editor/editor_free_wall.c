/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_free_wall.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 15:00:32 by louali            #+#    #+#             */
/*   Updated: 2020/02/24 15:39:00 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

void			free_walls(t_walls **walls)
{
	int		i;
	t_wall	*wall;

	if (!*walls)
		return ;
	i = -1;
	while (++i < (*walls)->len)
	{
		wall = &(*walls)->values[i];
		ft_memdel((void **)&wall->start_rooms_range);
		ft_memdel((void **)&wall->end_rooms_range);
		ft_memdel((void **)&wall->wall_sections);
	}
	ft_memdel((void **)walls);
}

t_wall_section	init_wall_section(t_editor *editor)
{
	return ((t_wall_section){
		.normal_type = 1,
		.texture = get_default_texture(&editor->doom->res_manager, TRUE),
		.collisions = TRUE,
		.invisible = FALSE,
		.uv_repeat = (t_vec2){ 1, 1 }
	});
}
