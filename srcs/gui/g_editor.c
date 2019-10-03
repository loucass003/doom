/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_editor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 15:50:09 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/03 12:12:36 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "gui.h"
#include "doom.h"

void			set_var(t_doom *doom)
{
	doom->editor.lignes = 0;
	doom->editor.curseur = 0;
	doom->editor.poly = 0;
	doom->editor.icone = 0;
	doom->editor.sup = 0;
	doom->editor.secteur = 0;
	doom->editor.porte = 0;
	doom->editor.set_start = 0;
}

static void		action_performed(t_component *cmp, t_doom *doom)
{
	set_var(doom);
	if (cmp == doom->guis[doom->current_gui].components[0])
		set_map(doom);
	else if (cmp == doom->guis[doom->current_gui].components[1]) //start
		doom->editor.set_start = (doom->editor.set_start == 0) ? 1 : 0;
	else if (cmp == doom->guis[doom->current_gui].components[2]) //sup ligne
		doom->editor.sup = (doom->editor.sup == 0) ? 1 : 0;
	else if (cmp == doom->guis[doom->current_gui].components[4]) //objet
		doom->editor.icone = (doom->editor.icone == 0) ? 1 : 0;
	else if (cmp == doom->guis[doom->current_gui].components[5]) //poly
		doom->editor.poly = (doom->editor.poly == 0) ? 1 : 0;
	else if (cmp == doom->guis[doom->current_gui].components[6]) //modif
		doom->editor.curseur = (doom->editor.curseur == 0) ? 1 : 0;
	else if (cmp == doom->guis[doom->current_gui].components[7]) //erase
		erase_all(doom);
	else if (cmp == doom->guis[doom->current_gui].components[8]) //lignes
	{
		if (doom->editor.lignes == 0)
			doom->editor.click = 0;
		doom->editor.lignes = (doom->editor.lignes == 0) ? 1 : 0;
	}
	else if (cmp == doom->guis[doom->current_gui].components[9])
		doom->editor.secteur = (doom->editor.secteur == 0) ? 1 : 0;
	else if (cmp == doom->guis[doom->current_gui].components[10])
		doom->editor.porte = (doom->editor.porte == 0) ? 1 : 0;
}

void			g_editor_button(t_gui *self, t_doom *doom)
{
	if (!alloc_components(self, 11))
		return ;
	doom->editor.set_start_pos[0] = 0;
	doom->editor.set_start_pos[1] = 0;
	self->components[0] = create_button((SDL_Rect){6, 9, 50, 50}, "icons/icons8-jouer-80.png", NULL);
	self->components[1] = create_button((SDL_Rect){56, 9, 50, 50}, "icons/pin.png", NULL);
	self->components[2] = create_button((SDL_Rect)
	{106, 9, 50, 50}, "icons/icons8-effacer-96.png", NULL);
	self->components[3] = create_menu((SDL_Rect)
	{156, 9, 50, 50}, "wall", doom);
	self->components[4] = create_menu((SDL_Rect)
	{206, 9, 50, 50}, "obj", doom);
	self->components[5] = create_button((SDL_Rect)
	{256, 9, 50, 50}, "icons/poly.png", NULL);
	self->components[6] = create_button((SDL_Rect)
	{306, 9, 50, 50}, "icons/curseur3.png", NULL);
	self->components[7] = create_button((SDL_Rect)
	{356, 9, 50, 50}, "icons/croix.png", NULL);
	self->components[8] = create_button((SDL_Rect)
	{406, 9, 50, 50}, "icons/icons8-stylo-64.png", NULL);
	self->components[9] = create_button((SDL_Rect)
	{456, 9, 50, 50}, "icons/icons8-modifier-la-ligne-80.png", NULL);
	self->components[10] = create_button((SDL_Rect)
	{506, 9, 50, 50}, "icons/icons8-porte-64.png", NULL);
	int i = 0;
	while (i <= 10)
		self->components[i++]->perform_action = action_performed;
}

void			g_editor_on_enter(t_gui *self, t_doom *doom)
{
	doom->editor.polygon = create_walls_array(20);
	doom->editor.lines = create_walls_array(10);
	doom->editor.objects = create_objects_array(10);
	if (!(doom->editor.point = (uint8_t *)malloc(sizeof(uint8_t)
		* (((doom->screen.width) * doom->screen.height - 600) / 20))))
		return ; //TODO: ERROR
	ft_bzero(doom->editor.point, sizeof(uint8_t)
		* (((doom->screen.width) * doom->screen.height - 600) / 20));
	set_alert_message(doom);
	set_var(doom);
	g_editor_button(self, doom);
}

void			g_editor_on_leave(t_gui *self, t_doom *doom)
{

}

void			g_editor_on_event(t_gui *self, SDL_Event *event, t_doom *doom)
{
	if (event->type == SDL_MOUSEMOTION)
		editor_mouse_motion(doom, event);
	if (event->type == SDL_MOUSEBUTTONUP && event->button.y > (doom->screen.height - 600))
		editor_mousebuttonup(doom, event->button.x, event->button.y);
}

void			g_editor_render(t_gui *self, t_doom *doom)
{
	int				x;
	int				y;


	y = 2;
	while (y++ <= doom->screen.height / 20)
	{
		x = 10;
		while (x++ < 1270)
		{
			doom->screen.pixels[(y * 20 + 10) * doom->screen.width
			+ x] = doom->editor.point[(y * (doom->screen.width
			/ 20)) + x] == 1 ? 0xFFFF0000 : 0xFF505050;
		}
	}
	x = 1;
	while (x++ <= doom->screen.width / 20)
	{
		y = 70;
		while (y++ < 630)
			{
				doom->screen.pixels[y * doom->screen.width
				+ (x * 20) + 10] = doom->editor.point[(y * (doom->screen.width
				/ 20)) + x] == 1 ? 0xFFFF0000 : 0xFF505050;
			}
	}
	// y = -1;
	// while (++y <= doom->screen.height / 20)
	// {
	// 	x = -1;
	// 	if (y >= (doom->screen.height - 600) / 20)
	// 	{
	// 		while (++x <= doom->screen.width / 20)
	// 		{
	// 				doom->screen.pixels[(y * 20 + 10) * doom->screen.width
	// 				+ (x * 20) + 10] = doom->editor.point[(y * (doom->screen.width
	// 				/ 20)) + x] == 1 ? 0xFFFF0000 : 0xFFFFFFFF;
	// 		}
	// 	}
	// }
	if (doom->editor.curseur == 1)
		all_visual(doom, doom->editor.last_mouse.x / 20, doom->editor.last_mouse.y / 20);
	if (doom->editor.click > 1 && (doom->editor.poly == 1 || doom->editor.porte == 1 || doom->editor.secteur == 1 || doom->editor.lignes == 1))
		visual_line(doom, (t_line){doom->editor.line.b.x, doom->editor.line.b.y, (int)doom->editor.last_mouse.x / 20, (int)doom->editor.last_mouse.y / 20});
	if (doom->editor.click == 1 && (doom->editor.poly == 1 || doom->editor.porte == 1 || doom->editor.secteur == 1 || doom->editor.lignes == 1))
		visual_line(doom, (t_line){doom->editor.line.a.x, doom->editor.line.a.y, (int)doom->editor.last_mouse.x / 20, (int)doom->editor.last_mouse.y / 20});
	write_alert_message(doom);
	print_all(doom);
	if (doom->editor.sup == 1)
	{
		if (doom->editor.polygon || doom->editor.lines)
			draw_line(&doom->screen, (t_pixel){doom->linetodel.line.a.x * 20 
			+ 10, doom->linetodel.line.a.y * 20 + 10, 0xFFFF0000}, (t_pixel){doom->linetodel.line.b.x 
			* 20 + 10, doom->linetodel.line.b.y * 20 + 10});
		else
			doom->linetodel.line = (t_line) {0, 0, 0, 0};
	}
	display_comp(doom, self);
	show_new_components(doom);
}
