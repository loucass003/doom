/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 17:31:07 by lloncham          #+#    #+#             */
/*   Updated: 2019/08/20 14:53:28 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void	visual_point(t_doom *doom, int x, int y) //allume les points en rouge
{
	if (x < (int)(doom->screen.width - 180))
	{
		if (doom->editor.point[(y * doom->screen.width / 20) + x] == 0)
		{
			ft_bzero(doom->editor.point, sizeof(uint8_t)
			* (((doom->screen.width - 200) * doom->screen.height) / 20));
			doom->editor.point[(y * doom->screen.width / 20) + x] = 1;
		}
	}
}

void	editor_mouse_motion(t_doom *doom, SDL_Event *event)
{
//	t_save	*lst;
	
	doom->editor.last_mouse.x = event->motion.x;
	doom->editor.last_mouse.y = event->motion.y;
	doom->editor.set_sup[0] = event->motion.x / 20;
	doom->editor.set_sup[1] = event->motion.y / 20;
	visual_point(doom, doom->editor.last_mouse.x / 20, doom->editor.last_mouse.y / 20);
	// if (doom->editor.sup == 1 && doom->editor.polygon) // PROBLEME ICI
	// {
	// 	lst = doom->editor.polygon;
	// 	while (lst->floor != doom->editor.floor && lst->next != NULL)
	// 		lst = lst->next;
	// 	if (lst->line)
	// 		mouseonline(doom, lst->line, doom->editor.set_sup);
	// }
	// if (doom->editor.sup == 1 && doom->editor.lines)
	// {
	// 	lst = doom->editor.lines;
	// 	while (lst->floor != doom->editor.floor && lst->next != NULL)
	// 		lst = lst->next;
	// 	if (lst->line)
	// 		mouseonline(doom, lst->line, doom->editor.set_sup);
	// }
	// if (doom->editor.sup == 1 && doom->editor.door)
	// {
	// 	lst = doom->editor.door;
	// 	while (lst->floor != doom->editor.floor && lst->next != NULL)
	// 		lst = lst->next;
	// 	if (lst->line)
	// 		mouseonline(doom, lst->line, doom->editor.set_sup);
	// }
	// if (doom->editor.sup == 1 && doom->editor.sector)
	// {
	// 	lst = doom->editor.sector;
	// 	while (lst->floor != doom->editor.floor && lst->next != NULL)
	// 		lst = lst->next;
	// 	if (lst->line)
	// 		mouseonline(doom, lst->line, doom->editor.set_sup);
	// }
}

void	editor_mouse_draw(t_doom *doom, int x, int y) //on save les donnees apres avoir verifié que les points peuvent etre la
{
	if (doom->editor.click == 0)
	{
		if (doom->editor.lignes == 1 || doom->editor.secteur == 1 || doom->editor.porte == 1)
			if (in_the_poly(doom, doom->editor.polygon, (t_vec2){x / 20, y / 20}) == FALSE)
				return;
		doom->editor.line.a.x = x / 20;
		doom->editor.line.a.y = y / 20;
		doom->editor.click++;
	}
	else if (doom->editor.click >= 1)
	{
		if (doom->editor.click > 1)
		{
			doom->editor.line.a.x = doom->editor.line.b.x;
			doom->editor.line.a.y = doom->editor.line.b.y; 
			if (check_secant_line(doom, doom->editor.polygon, (t_line){{.x = doom->editor.line.a.x, .y = doom->editor.line.a.y}, {.x = x / 20, .y = y / 20}}) == FALSE)
				return ;
		}
		if (doom->editor.lignes == 1 || doom->editor.secteur == 1 || doom->editor.porte == 1)
			if (in_the_poly(doom, doom->editor.polygon, (t_vec2){x / 20, y / 20}) == FALSE)
				return;
    	doom->editor.line.b.x = x / 20;
		doom->editor.line.b.y = y / 20;
		if (doom->editor.poly == 1 && check_same_point(doom) == FALSE)
			return ;
		
		save_in_lst(doom);
		if (doom->editor.polygon)
			check_poly_close(doom, doom->editor.polygon);
		doom->editor.click++;
		if (doom->editor.poly != 1)
			doom->editor.click = 0;
	}
}

void	editor_mousebuttonup(t_doom *doom, int x, int y)
{
	if (doom->open == 0)
	{
		if (doom->editor.curseur == 1)//modification de points
		{
			if (doom->editor.save_modif[2] == 0)
			{
				doom->editor.save_modif[0] = x / 20;
				doom->editor.save_modif[1] = y / 20;
				doom->editor.save_modif[2] = 1;
			}
			else if (doom->editor.save_modif[2] == 1)
				modify_all(doom, x, y);
		}
		else if (doom->editor.set_start != 0) //determiner point de depart
		{
			if (in_the_poly(doom, doom->editor.polygon, (t_vec2){x / 20, y / 20}) == FALSE)
				return;
			doom->editor.set_start_pos[0] = x / 20;
			doom->editor.set_start_pos[1] = y / 20;
		}
		else if (doom->editor.sup >= 1)//suppression de point
			save_line_to_erase(doom, x / 20, y / 20);
		if (doom->editor.icone == 1)
		{
			if (in_the_poly(doom, doom->editor.polygon, (t_vec2){x / 20, y / 20}) == FALSE)
				return;
			save_object(doom, x, y, doom->editor.objet);
		}
		else if (doom->editor.poly == 1 || doom->editor.lignes == 1 || doom->editor.secteur == 1 || doom->editor.porte == 1)
			editor_mouse_draw(doom, x, y);
	}
}
