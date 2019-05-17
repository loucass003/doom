/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 17:31:07 by lloncham          #+#    #+#             */
/*   Updated: 2019/05/17 14:53:12 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_bool	new_poly(t_poly **poly, t_line line)
{
	t_poly			*n;
	t_poly			*elem;
	t_line_list		*new;

	if (!(elem = (t_poly *)malloc(sizeof(t_poly))))
		return (FALSE);
	if (!(new = (t_line_list*)malloc(sizeof(t_line_list))))
		return (FALSE);
	new->line = line;
	new->next = NULL;
	elem->list = new;
	elem->next = NULL;
	if (!*poly)
		*poly = elem;
	else
	{
		n = *poly;
		while (n && n->next)
			n = n->next;
		n->next = elem;
	}
	return (TRUE);
}

t_bool	append_list2(t_poly **poly, t_line line)
{
	t_poly			*n;
	t_line_list		*elem;
	t_line_list		*cmp;

	if (!(elem = (t_line_list *)malloc(sizeof(t_line_list))))
		return (FALSE);
	elem->line = line;
	elem->next = NULL;
	n = *poly;
	while (n && n->next)
		n = n->next;
	cmp = n->list;
	while (cmp && cmp->next)
		cmp = cmp->next;
	cmp->next = elem;
	return (TRUE);
}

void	editor_mouse_motion(t_doom *doom, SDL_Event *event)
{
	int				x;
	int				y;
	unsigned int	index;

	x = event->motion.x / 20;
	y = event->motion.y / 20;
	if (x < (doom->screen.width - 180))
	{
		if (doom->editor.point[(y * doom->screen.width / 20) + x] == 0)
		{
			ft_bzero(doom->editor.point, sizeof(uint8_t)
			* (((doom->screen.width - 200) * doom->screen.height) / 20));
			doom->editor.point[(y * doom->screen.width / 20) + x] = 1;
		}
	}
}

void	editor_mousebuttonup(t_doom *doom, SDL_Event *event)
{
	if (doom->editor.click == 0 && (check_multi_point(doom, doom->editor.polygon
	, (int)event->button.x / 20, (int)event->button.y / 20) == TRUE))
	{
		doom->editor.line.a.x = (int)event->button.x / 20;
		doom->editor.line.a.y = (int)event->button.y / 20;
		doom->editor.first[0] = doom->editor.line.a.x;
		doom->editor.first[1] = doom->editor.line.a.y;
		doom->editor.click++;
	}
	else if (check_multi_point(doom, doom->editor.polygon
	, (int)event->button.x / 20, (int)event->button.y / 20) == TRUE)
	{
		set_alert_message(doom);
		if (doom->editor.click > 1)
		{
			doom->editor.line.a.x = doom->editor.line.b.x;
			doom->editor.line.a.y = doom->editor.line.b.y;
		}
		if (check_multi_line(doom, doom->editor.polygon, doom->editor.line.a.x
		, doom->editor.line.a.y, (int)event->button.x / 20
		, (int)event->button.y / 20) == FALSE)
			return ;
		if (check_secant_line(doom, doom->editor.polygon, doom->editor.line.a.x
		, (int)event->button.x / 20, doom->editor.line.a.y
		, (int)event->button.y / 20) == FALSE)
			return ;
		doom->editor.line.b.x = (int)event->button.x / 20;
		doom->editor.line.b.y = (int)event->button.y / 20;
		if (check_same_point(doom) == FALSE)
			return ;
		if (doom->editor.click == 1)
			new_poly(&doom->editor.polygon, doom->editor.line);
		else if (doom->editor.click > 1)
			append_list2(&doom->editor.polygon, doom->editor.line);
		check_poly_close(doom, doom->editor.polygon);
		doom->editor.click++;
	}
}