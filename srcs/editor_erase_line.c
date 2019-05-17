/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_erase_line.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 13:18:39 by lloncham          #+#    #+#             */
/*   Updated: 2019/05/17 16:37:01 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void	save_line_to_erase(t_doom *doom, SDL_Event *event)
{
	if (doom->editor.sup == 1)
	{
		doom->editor.set_sup[0] = event->button.x / 20;
		doom->editor.set_sup[1] = event->button.y / 20;
	}
	else
	{
		doom->editor.set_sup[2] = event->button.x / 20;
		doom->editor.set_sup[3] = event->button.y / 20;
		erase_line(doom, doom->editor.polygon, doom->editor.set_sup);
	}
	doom->editor.sup++;
}

void	erase_line(t_doom *doom, t_poly *poly, int *line)
{
	t_poly		*tmp1;
	t_line_list *tmp;
	t_line_list *prev;

	tmp1 = poly;
	doom->editor.sup = 0;
	while (tmp1)
	{
		tmp = tmp1->list;
		prev = tmp;
		if (tmp->line.a.x == line[0] && tmp->line.a.y == line[1]
		&& tmp->line.b.x == line[2] && tmp->line.b.y == line[3] && tmp)
		{
			prev = tmp->next;
			free(tmp);
			return ;
		}
		while (tmp)
		{
			if (tmp->line.a.x == line[0] && tmp->line.a.y == line[1]
			&& tmp->line.b.x == line[2] && tmp->line.b.y == line[3])
			{
				prev->next = tmp->next;
				free(tmp);
				return ;
			}
			prev = tmp;
			tmp = tmp->next;
		}
		tmp1 = tmp1->next;
	}
}
