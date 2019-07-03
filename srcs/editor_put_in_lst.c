/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_put_in_lst.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 13:51:56 by lloncham          #+#    #+#             */
/*   Updated: 2019/07/03 14:19:42 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void    save_object(t_doom *doom, int x, int y, SDL_Surface *obj)
{
	t_obj   		*elem;
	t_obj   		*new;

	if (check_obj_legit(doom, x/20, y/20) == FALSE)
		return ;
	if (!(elem = (t_obj *)malloc(sizeof(t_obj))))
		return ;
	elem->pos.x = x / 20;
	elem->pos.y = y / 20;
	elem->floor = doom->editor.floor;
	elem->img = obj;
	elem->next = NULL;
	if (!doom->editor.obj)
		doom->editor.obj = elem;
	else
	{
		new = doom->editor.obj;
		while (new && new->next)
			new = new->next;
		new->next = elem;
	}
}

t_line_list	*complete_list(t_doom *doom, t_line line)
{
	t_line_list		*elem;

	if (!(elem = (t_line_list*)malloc(sizeof(t_line_list))))
		return (0);
	elem->line = line;
	elem->texture = doom->editor.texture;
	elem->next = NULL;
	return (elem);
}

t_bool	new_poly(t_doom *doom, t_save **lst, t_line line)
{
	t_save			*tmp;
	t_save			*stock;
	t_line_list		*lines;
	t_line_list		*poly;

	if (!(tmp = (t_save *)malloc(sizeof(t_save))))
		return (FALSE);
	lines = complete_list(doom, line);
	tmp->floor = doom->editor.floor;
	tmp->line = lines;
	tmp->next = NULL;
	if (!*lst)
		*lst = tmp;
	else 
	{
		stock = *lst;
		while (stock->floor != doom->editor.floor && stock->next)
			stock = stock->next;
		poly = stock->line;
		if (stock->floor == doom->editor.floor)
		{
			while (poly && poly->next)
				poly = poly->next;
			poly->next = lines;
		}
		else
			stock->next = tmp;
	}
	return (TRUE);
}

void save_in_lst(t_doom *doom)
{
	if (doom->editor.poly == 1)
		new_poly(doom, &doom->editor.polygon, doom->editor.line);
	if (doom->editor.lignes == 1)
		new_poly(doom, &doom->editor.lines, doom->editor.line);
	if (doom->editor.porte == 1)
		new_poly(doom, &doom->editor.door, doom->editor.line);
	if (doom->editor.secteur == 1)
		new_poly(doom, &doom->editor.sector, doom->editor.line);
}
