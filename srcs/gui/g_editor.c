/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_editor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 15:50:09 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/28 17:26:01 by llelievr         ###   ########.fr       */
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

t_vec2			get_grid_pos(t_vec2	grid_cell)
{
	return ((t_vec2){ 10 + (int)(grid_cell.x * CELLS_SPACING), 70 + (int)(grid_cell.y * CELLS_SPACING) });
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

t_bool		get_line_intersection(t_line a, t_line b, float *i_x, float *i_y)
{
	t_vec2	s1;
	t_vec2	s2;

	s1.x = a.b.x - a.a.x;     s1.y = a.b.y - a.a.y;
	s2.x = b.b.x - b.a.x;     s2.y = b.b.y - b.a.y;

	float s, t;
	s = (-s1.y * (a.a.x - b.a.x) + s1.x * (a.a.y - b.a.y)) / (-s2.x * s1.y + s1.x * s2.y);
	t = ( s2.x * (a.a.y - b.a.y) - s2.y * (a.a.x - b.a.x)) / (-s2.x * s1.y + s1.x * s2.y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		// Collision detected
		if (i_x != NULL)
			*i_x = a.a.x + (t * s1.x);
		if (i_y != NULL)
			*i_y = a.a.y + (t * s1.y);
		printf("COLLISION\n");
		return (TRUE);
	}

	return (FALSE); // No collision
}


t_bool			is_simple_polygon(t_room *room)
{
	int		i;
	int		j;
	t_line	line1;
	t_line	line2;

	// if (room->points->len < 5)
	// 	return (TRUE);
	printf("CHECK ---\n");
	i = -1;
	while (++i < room->points->len)
	{
		int next = (i + 1) % room->points->len;
		t_vec2	p0 = room->points->vertices[i];
		t_vec2	p1 = room->points->vertices[next];
		line1 = (t_line){ .a = p0, .b = p1 };
		j = -1;
		while (++j < room->points->len) 
		{
			int next = (j);
			int next2 = (j + 1) % room->points->len;
			t_vec2	p0 = room->points->vertices[next];
			t_vec2	p1 = room->points->vertices[next2];
			line2 = (t_line){ .a = p0, .b = p1 };
			printf("%f %f %f %f -- %f %f %f %f\n", line1.a.x, line1.a.y, line1.b.x, line1.b.y, line2.a.x, line2.a.y, line2.b.x, line2.b.y);
			if (line1.a.x == line2.a.x && line1.a.y == line2.a.y && line1.b.x == line2.b.x && line1.b.y == line2.b.y)
				continue;
			if (line1.b.x == line2.a.x && line1.b.y == line2.a.y)
				continue;
			if (line2.b.x == line1.a.x && line2.b.y == line1.a.y)
				continue;
			if (get_line_intersection(line1, line2, NULL, NULL))
				return (FALSE);
		}
	}
	return (TRUE);
}

void			g_editor_on_event(t_gui *self, SDL_Event *event, t_doom *doom)
{
	if (event->type == SDL_MOUSEMOTION)
	{
		t_vec2 pos = (t_vec2){ event->motion.x, event->motion.y };
		if (in_bounds((SDL_Rect){ 5, 65, doom->screen.width - 5, doom->screen.height - 65 }, pos))
			doom->editor.grid_cell = (t_vec2){ (int)(pos.x - 5) / CELLS_SPACING, (int)(pos.y - 65) / CELLS_SPACING };
		else
			doom->editor.grid_cell = (t_vec2){ -1, -1 };
	}
	else if (event->type == SDL_MOUSEBUTTONUP)
	{
		if (doom->editor.selected_tool == 3 && doom->editor.grid_cell.x != -1)
		{
			if (doom->editor.current_room == -1)
			{
				append_rooms_array(&doom->editor.rooms, (t_room){ .points = create_2dvertices_array(15) });
				doom->editor.current_room = doom->editor.rooms->len - 1;
			}

			t_room	*curr_room = &doom->editor.rooms->values[doom->editor.current_room];

			if (curr_room->points->len >= 3 && ( event->button.button == SDL_BUTTON_RIGHT || (curr_room->points->vertices[0].x == doom->editor.grid_cell.x && curr_room->points->vertices[0].y == doom->editor.grid_cell.y)))
			{
				curr_room->closed = TRUE;
				doom->editor.current_room = -1;
				doom->editor.line_start_cell = (t_vec2){ -1, -1 };
			}
			else
			{
				append_2dvertices_array(&curr_room->points, doom->editor.grid_cell);
				if (!is_simple_polygon(curr_room))
					splice_2dvertices_array(curr_room->points, curr_room->points->len - 1, 1);
				else
					doom->editor.line_start_cell = doom->editor.grid_cell;
			}

			// if (event->button.button == SDL_BUTTON_RIGHT)
			// 	doom->editor.line_start_cell = (t_vec2){ -1, -1 };
			// else if (doom->editor.line_start_cell.x != -1)
			// {
			// 	printf("ADD LINE\n");
			// 	doom->editor.line_start_cell = (t_vec2){ -1, -1 };
			// }
			// else
			// 	doom->editor.line_start_cell = doom->editor.grid_cell;
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


	int i = -1;
	while (++i <= (int)(doom->screen.height - 70) / CELLS_SPACING)
		draw_line(&doom->screen, (t_pixel){ 10, i * CELLS_SPACING + 70, 0xFF505050}, (t_pixel){ doom->screen.width - 10, i * CELLS_SPACING + 70 });
	i = -1;
	while (++i <= (int)(doom->screen.width - 20) / CELLS_SPACING)
		draw_line(&doom->screen, (t_pixel){ i * CELLS_SPACING + 10, 70, 0xFF505050}, (t_pixel){ i * CELLS_SPACING + 10, doom->screen.height - 10 });
	if (doom->editor.grid_cell.x != -1)
	{
		t_vec2 grid_pos = get_grid_pos(doom->editor.grid_cell);
		draw_circle(&doom->screen, (t_pixel){ grid_pos.x, grid_pos.y, 0xFFFF00FF }, 5);
	}
	if (doom->editor.line_start_cell.x != -1)
	{
		t_vec2	p0 = get_grid_pos(doom->editor.line_start_cell);
		t_vec2	p1 = get_grid_pos(doom->editor.grid_cell);
		draw_line(&doom->screen, (t_pixel){ p0.x, p0.y, 0xFF909090}, (t_pixel){ p1.x, p1.y, 0 });
	}

	i = -1;
	while (++i < doom->editor.rooms->len)
	{
		t_room *room = &doom->editor.rooms->values[i];
		if (room->points->len < 2)
			continue;
		int j = -1;
		if (!room->closed)
		{
			t_vec2	first_point = get_grid_pos(room->points->vertices[0]);
			draw_circle(&doom->screen, (t_pixel){ first_point.x, first_point.y, 0xFFFF00FF }, 5);
		}
		while (++j < room->points->len - !(room->closed))
		{
			int next = (j + 1) % room->points->len;
			t_vec2	p0 = get_grid_pos(room->points->vertices[j]);
			t_vec2	p1 = get_grid_pos(room->points->vertices[next]);
			draw_circle(&doom->screen, (t_pixel){ p0.x, p0.y, 0xFFFF00FF }, 2);
			draw_line(&doom->screen, (t_pixel){ p0.x, p0.y, 0xFFFF9090}, (t_pixel){ p1.x, p1.y, 0 });
		}
	}

	draw_rect(&doom->screen, (SDL_Rect){ 5, 5, S_WIDTH - 10, 55 }, 0xFFFFFFFF);
	render_components(doom, self);
	// show_new_components(doom);
}
