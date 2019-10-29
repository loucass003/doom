/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_editor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 15:50:09 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/29 18:49:31 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "gui.h"
#include "doom.h"

# define CELLS_SPACING (10)

// void			set_var(t_doom *doom)
// {
// 	doom->editor.lignes = 0;
// 	doom->editor.curseur = 0;
// 	doom->editor.poly = 0;
// 	doom->editor.icone = 0;
// 	doom->editor.sup = 0;
// 	doom->editor.secteur = 0;
// 	doom->editor.porte = 0;
// 	doom->editor.set_start = 0;
// }


static void		action_performed(t_component *cmp, t_doom *doom)
{
	int			i;
	t_button	*btn;

	btn = cmp;
	i = -1;
	while (++i < 8)
	{
		btn = doom->guis[doom->current_gui].components->values[i];
		if (cmp == btn)
		{
			doom->editor.selected_tool = i;
			btn->selected = TRUE;
		}
		else
			btn->selected = FALSE;
	}
	
	if (doom->editor.current_room != -1)
	{
		t_room	*curr_room = &doom->editor.rooms->values[doom->editor.current_room];
		curr_room->closed = TRUE;
		doom->editor.current_room = -1;
		doom->editor.line_start_cell = (t_vec2){ -1, -1 };
	}
	//set_var(doom);
	// if (cmp == doom->guis[doom->current_gui].components->values[0])
	// 	set_map(doom);
	// else if (cmp == doom->guis[doom->current_gui].components->values[1]) //start
	// 	doom->editor.set_start = (doom->editor.set_start == 0) ? 1 : 0;
	// else if (cmp == doom->guis[doom->current_gui].components->values[2]) //sup ligne
	// 	doom->editor.sup = (doom->editor.sup == 0) ? 1 : 0;
	// else if (cmp == doom->guis[doom->current_gui].components->values[4]) //objet
	// 	doom->editor.icone = (doom->editor.icone == 0) ? 1 : 0;
	// else if (cmp == doom->guis[doom->current_gui].components->values[5]) //poly
	// 	doom->editor.poly = (doom->editor.poly == 0) ? 1 : 0;
	// else if (cmp == doom->guis[doom->current_gui].components->values[6]) //modif
	// 	doom->editor.curseur = (doom->editor.curseur == 0) ? 1 : 0;
	// else if (cmp == doom->guis[doom->current_gui].components->values[7]) //erase
	// 	erase_all(doom);
	// else if (cmp == doom->guis[doom->current_gui].components->values[8]) //lignes
	// {
	// 	if (doom->editor.lignes == 0)
	// 		doom->editor.click = 0;
	// 	doom->editor.lignes = (doom->editor.lignes == 0) ? 1 : 0;
	// }
	// else if (cmp == doom->guis[doom->current_gui].components->values[9])
	// 	doom->editor.secteur = (doom->editor.secteur == 0) ? 1 : 0;
	// else if (cmp == doom->guis[doom->current_gui].components->values[10])
	// 	doom->editor.porte = (doom->editor.porte == 0) ? 1 : 0;
}

void			g_editor_button(t_gui *self, t_doom *doom)
{
	doom->editor.line_start_cell = (t_vec2){ -1, -1 };
	doom->editor.set_start_pos[0] = 0;
	doom->editor.set_start_pos[1] = 0;
	append_components_array(&self->components, create_button((SDL_Rect){8, 9, 50, 50}, "icons/icons8-jouer-80.png", NULL));
	append_components_array(&self->components, create_button((SDL_Rect){60, 9, 50, 50}, "icons/pin.png", NULL));
	append_components_array(&self->components, create_button((SDL_Rect){112, 9, 50, 50}, "icons/icons8-effacer-96.png", NULL));
	append_components_array(&self->components, create_button((SDL_Rect){164, 9, 50, 50}, "icons/poly.png", NULL));
	append_components_array(&self->components, create_button((SDL_Rect){216, 9, 50, 50}, "icons/curseur3.png", NULL));
	append_components_array(&self->components, create_button((SDL_Rect){268, 9, 50, 50}, "icons/croix.png", NULL));
	append_components_array(&self->components, create_button((SDL_Rect){320, 9, 50, 50}, "icons/icons8-stylo-64.png", NULL));
	append_components_array(&self->components, create_button((SDL_Rect){372, 9, 50, 50}, "icons/icons8-modifier-la-ligne-80.png", NULL));
	int i = 0;
	while (i < 8)
		self->components->values[i++]->perform_action = action_performed;
}

void			g_editor_on_enter(t_gui *self, t_doom *doom)
{
	doom->editor.rooms = create_rooms_array(5);
	doom->editor.points = create_2dvertices_array(50);
	doom->editor.current_room = -1;
	// doom->editor.polygon = create_walls_array(20);
	// doom->editor.lines = create_walls_array(10);
	// doom->editor.objects = create_objects_array(10);
	// if (!(doom->editor.point = (uint8_t *)malloc(sizeof(uint8_t)
	// 	* (((doom->screen.width) * doom->screen.height - 600) / 20))))
	// 	return ; //TODO: ERROR
	// ft_bzero(doom->editor.point, sizeof(uint8_t)
	// 	* (((doom->screen.width) * doom->screen.height - 600) / 20));
	// set_alert_message(doom);
	// set_var(doom);
	g_editor_button(self, doom);
}

void			g_editor_on_leave(t_gui *self, t_doom *doom)
{

}

int			orientation(t_vec2 p, t_vec2 q, t_vec2 r) 
{
	int val = (q.y - p.y) * (r.x - q.x) - 
				(q.x - p.x) * (r.y - q.y); 

	if (val == 0) return 0;
	return (val > 0) ? 1: 2;
} 

t_bool		seg_intersect(t_line p, t_line q, t_bool colinear_check)
{ 
    int o1 = orientation(p.a, p.b, q.a); 
    int o2 = orientation(p.a, p.b, q.b); 
    int o3 = orientation(q.a, q.b, p.a); 
    int o4 = orientation(q.a, q.b, p.b); 

	if ((q.a.x == p.b.x && q.a.y == p.b.y) || (q.b.x == p.a.x && q.b.y == p.a.y))
		return (FALSE);
	if (o1 != o2 && o3 != o4)
		return TRUE;
	return FALSE;
}

t_bool		get_line_intersection(t_line a, t_line b, t_bool strict)
{
	t_vec2	s1;
	t_vec2	s2;

	s1.x = a.b.x - a.a.x;     s1.y = a.b.y - a.a.y;
	s2.x = b.b.x - b.a.x;     s2.y = b.b.y - b.a.y;

	float s, t;
	s = (-s1.y * (a.a.x - b.a.x) + s1.x * (a.a.y - b.a.y)) / (-s2.x * s1.y + s1.x * s2.y);
	t = ( s2.x * (a.a.y - b.a.y) - s2.y * (a.a.x - b.a.x)) / (-s2.x * s1.y + s1.x * s2.y);

	/* if (strict && s >= 0 && s <= 1 && t >= 0 && t <= 1)
		return (TRUE);
	else */ if (s > 0 && s < 1 && t > 0 && t < 1)
		return (TRUE);
	return (FALSE);
}


t_bool			room_intersect(t_editor *editor, t_room *room, t_bool strict)
{
	int		i;
	int		j;
	int		k;
	t_line	line1;
	t_line	line2;

	i = -1;
	while (++i < room->indices->len - 1)
	{
		t_vec2 p0 = editor->points->vertices[room->indices->values[i]];
		t_vec2 p1 = editor->points->vertices[room->indices->values[i + 1]];
		line1 = (t_line){ .a = p0, .b = p1 };
		j = -1;
		while (++j < editor->rooms->len)
		{
			t_room	*room2 = &editor->rooms->values[j];
			k = -1;
			while (++k < room2->indices->len - !(room2->closed))
			{
				t_vec2 p0 = editor->points->vertices[room2->indices->values[k]];
				t_vec2 p1 = editor->points->vertices[room2->indices->values[(k + 1) % room2->indices->len]];
				line2 = (t_line){ .a = p0, .b = p1 };
				if (line1.a.x == line2.a.x && line1.a.y == line2.a.y && line1.b.x == line2.b.x && line1.b.y == line2.b.y)
					continue;
				if (line1.b.x == line2.a.x && line1.b.y == line2.a.y)
					continue;
				if (line2.b.x == line1.a.x && line2.b.y == line1.a.y)
					continue;
				if (get_line_intersection(line1, line2, strict || room == room2))
					return (TRUE);
			}
		}
	}
	return (FALSE);
}

t_bool			is_in_range(t_vec2 pos, t_vec2 test)
{
	return ((test.x - pos.x) * (test.x - pos.x) + (test.y - pos.y) * (test.y - pos.y) < 9 * 9);
}

t_vec2			get_point_on_seg(t_vec2 p0, t_vec2 p1, t_vec2 pos)
{
	const	t_vec2	b = ft_vec2_sub(p1, p0);
	const	t_vec2	p = ft_vec2_sub(pos, p0);
	float	f;
	float	l;
	
	l = ft_vec2_len(b);
	f = clamp(0, 1, ft_vec2_dot(p, b) / l / l);
	return ((t_vec2){ p0.x + f * b.x, p0.y + f * b.y });
}

t_bool			is_point_on_seg(t_vec2 project, t_vec2 pos)
{
	return (ft_vec2_len(ft_vec2_sub(project, pos)) < 6);
}

t_vec2			get_close_point(t_editor *editor, t_vec2 pos)
{
	int k;
	int j;

	j = -1;
	while (++j < editor->rooms->len)
	{
		t_room	*room2 = &editor->rooms->values[j];
		k = -1;
		while (++k < room2->indices->len)
		{
			t_vec2 p0 = editor->points->vertices[room2->indices->values[k]];
			t_vec2 p1 = editor->points->vertices[room2->indices->values[(k + 1) % room2->indices->len]];
			if (is_in_range(p0, pos))
			{
				editor->grid_cell_grab = GG_POINT;
				return p0;
			}
			if (is_in_range(p1, pos))
			{
				editor->grid_cell_grab = GG_POINT;
				return p1;
			}
			t_vec2	project = get_point_on_seg(p0, p1, pos);
			if (is_point_on_seg(project, pos))
			{
				editor->grid_cell_grab = GG_LINE;
				editor->close_seg = (t_vec2){ room2->indices->values[k], room2->indices->values[k + 1] };
				return project;
			}
		}
	}
	editor->grid_cell_grab = GG_NONE;
	return (pos);
}



void			g_editor_on_event(t_gui *self, SDL_Event *event, t_doom *doom)
{
	if (event->type == SDL_MOUSEMOTION)
	{
		t_vec2 pos = (t_vec2){ event->motion.x, event->motion.y };
		if (in_bounds((SDL_Rect){ 5, 65, doom->screen.width - 5, doom->screen.height - 65 }, pos))
			doom->editor.grid_cell = get_close_point(&doom->editor, pos);
		else
			doom->editor.grid_cell = (t_vec2){ -1, -1 };
	}
	else if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		if (doom->editor.selected_tool == 3 && doom->editor.grid_cell.x != -1)
		{
			if (doom->editor.current_room == -1)
			{
				append_rooms_array(&doom->editor.rooms, (t_room){ .indices = create_ints_array(15) });
				doom->editor.current_room = doom->editor.rooms->len - 1;
			}

			t_room	*curr_room = &doom->editor.rooms->values[doom->editor.current_room];
			if (curr_room->indices->len >= 3)
			{
				t_vec2 vert0 = doom->editor.points->vertices[curr_room->indices->values[0]];
				if (event->button.button == SDL_BUTTON_RIGHT || (vert0.x == doom->editor.grid_cell.x && vert0.y == doom->editor.grid_cell.y))
				{
					curr_room->closed = TRUE;
					doom->editor.current_room = -1;
					doom->editor.line_start_cell = (t_vec2){ -1, -1 };
					printf("CLOSE\n");
					return ;
				}
			}
			
			int	index = vertices2d_indexof(doom->editor.points, doom->editor.grid_cell);
			
			if (doom->editor.grid_cell_grab == GG_NONE || doom->editor.grid_cell_grab == GG_LINE)
			{
				printf("NEW POINT\n");
				append_2dvertices_array(&doom->editor.points, doom->editor.grid_cell);
				index = doom->editor.points->len - 1;
				if (doom->editor.grid_cell_grab == GG_NONE)
				{
					append_ints_array(&curr_room->indices, index);
					if (room_intersect(&doom->editor, curr_room, TRUE))
					{
						splice_2dvertices_array(doom->editor.points, index, 1);
						splice_ints_array(curr_room->indices, curr_room->indices->len - 1, 1);
					}
					else
						doom->editor.line_start_cell = doom->editor.grid_cell;
				}
				else if (doom->editor.grid_cell_grab == GG_LINE)
				{
					printf("SPLIT LINE\n");
					append_ints_array(&curr_room->indices, index);
					if (room_intersect(&doom->editor, curr_room, FALSE))
					{
						printf("INTERSECT! 2\n");
						splice_2dvertices_array(doom->editor.points, index, 1);
						splice_ints_array(curr_room->indices, curr_room->indices->len - 1, 1);
						return ;
					}
					else
						doom->editor.line_start_cell = doom->editor.grid_cell;
					printf("INDEX %d\n", index);
					int		i;

					i = -1;
					while (++i < doom->editor.rooms->len)
					{
						t_room	*room0 = &doom->editor.rooms->values[i];
						int		index0 = ints_indexof(room0->indices, doom->editor.close_seg.x);
						int		index1 = ints_indexof(room0->indices, doom->editor.close_seg.y);
						if (index0 != -1 && index1 != -1)
						{
							append_ints_array(&room0->indices, -20);
							ft_memmove(room0->indices->values + index1 + 1, room0->indices->values + index1, (room0->indices->len - (index1 + 1)) * sizeof(int));
							room0->indices->values[index1] = index;
							printf("FOUND LINE %d\n", room0->indices->len - (index1 + 1));
						}
					}
					
				}
			}
			else if (doom->editor.grid_cell_grab == GG_POINT && index != -1)
			{
				append_ints_array(&curr_room->indices, index);
				if (room_intersect(&doom->editor, curr_room, TRUE))
				{
					printf("INTERSECT! 1 \n");
					splice_ints_array(curr_room->indices, curr_room->indices->len - 1, 1);
				}
				else
					doom->editor.line_start_cell = doom->editor.grid_cell;
			}
			
			// if (doom->editor.grid_cell_grab == GG_POINT || doom->editor.grid_cell_grab == GG_NONE)
			// {
			// 	if (index == -1)
			// 	{
			// 		append_2dvertices_array(&doom->editor.points, doom->editor.grid_cell);
			// 		index = doom->editor.points->len - 1;
			// 		append_ints_array(&curr_room->indices, index);
			// 		if (!room_intersect(&doom->editor, curr_room))
			// 		{
			// 			splice_2dvertices_array(doom->editor.points, index, 1);
			// 			splice_ints_array(curr_room->indices, curr_room->indices->len - 1, 1);
			// 		}
			// 		else
			// 			doom->editor.line_start_cell = doom->editor.grid_cell;
			// 	}
			
				
			// }
		}
	}

	// if (event->type == SDL_MOUSEMOTION)
	// 	editor_mouse_motion(doom, event);
	// if (event->type == SDL_MOUSEBUTTONUP && event->button.y > (doom->screen.height - 600))
	// 	editor_mousebuttonup(doom, event->button.x, event->button.y);
}

void			g_editor_render(t_gui *self, t_doom *doom)
{
	int				x;
	int				y;

	//printf("POINTS %d\n", doom->editor.points->len);

	int i = -1;
	while (++i <= (int)(doom->screen.height - 70) / CELLS_SPACING)
		draw_line(&doom->screen, (t_pixel){ 10, i * CELLS_SPACING + 70, 0xFF505050}, (t_pixel){ doom->screen.width - 10, i * CELLS_SPACING + 70 });
	i = -1;
	while (++i <= (int)(doom->screen.width - 20) / CELLS_SPACING)
		draw_line(&doom->screen, (t_pixel){ i * CELLS_SPACING + 10, 70, 0xFF505050}, (t_pixel){ i * CELLS_SPACING + 10, doom->screen.height - 10 });
	if (doom->editor.grid_cell.x != -1)
	{
		t_vec2 grid_pos = doom->editor.grid_cell;
		int		color = 0xFFFF00FF;
		if (doom->editor.grid_cell_grab == GG_POINT)
			color = 0xFFFFFF00;
		else if (doom->editor.grid_cell_grab == GG_LINE)
			color = 0xFF00FFFF;
		draw_circle(&doom->screen, (t_pixel){ grid_pos.x, grid_pos.y, color }, 5);
	}
	
	if (doom->editor.line_start_cell.x != -1)
	{
		t_vec2	p0 = doom->editor.line_start_cell;
		t_vec2	p1 = doom->editor.grid_cell;
		draw_line(&doom->screen, (t_pixel){ p0.x, p0.y, 0xFF909090}, (t_pixel){ p1.x, p1.y, 0 });
	}

	i = -1;
	while (++i < doom->editor.rooms->len)
	{
		t_room *room = &doom->editor.rooms->values[i];
		if (room->indices->len < 2)
			continue;
		int j = -1;
		if (!room->closed)
		{
			t_vec2	first_point = doom->editor.points->vertices[room->indices->values[0]];
			draw_circle(&doom->screen, (t_pixel){ first_point.x, first_point.y, 0xFFFFF00F }, 6);
		}
		while (++j < room->indices->len - !(room->closed))
		{
			int next = (j + 1) % room->indices->len;
			t_vec2	p0 = doom->editor.points->vertices[room->indices->values[j]];
			t_vec2	p1 = doom->editor.points->vertices[room->indices->values[next]];
			draw_line(&doom->screen, (t_pixel){ p0.x, p0.y, 0xFFFF9090}, (t_pixel){ p1.x, p1.y, 0 });
			draw_circle(&doom->screen, (t_pixel){ p0.x, p0.y, 0xFFFF00FF }, 2);
			// if (is_in_range(p0, doom->editor.grid_cell))
			// 	draw_circle(&doom->screen, (t_pixel){ p0.x, p0.y, 0xFFFFF00F }, 6);
		}
	}

	draw_rect(&doom->screen, (SDL_Rect){ 5, 5, S_WIDTH - 10, 55 }, 0xFFFFFFFF);
	render_components(doom, self);
	// show_new_components(doom);
}
