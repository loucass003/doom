/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_erase_line.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 13:18:39 by lloncham          #+#    #+#             */
/*   Updated: 2019/05/20 16:21:28 by lloncham         ###   ########.fr       */
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

void	erase_one_line(t_poly *tmp1, int *line)
{
	t_line_list *prev;
	t_line_list *tmp;

	tmp = tmp1->list;
	if (tmp->line.a.x == line[0] && tmp->line.a.y == line[1]
	&& tmp->line.b.x == line[2] && tmp->line.b.y == line[3] && tmp)
	{
		tmp1->list = tmp->next;
		free(tmp);
		tmp = NULL;
		return ;
	}
	prev = tmp;
	while (tmp)
	{
		if (tmp->line.a.x == line[0] && tmp->line.a.y == line[1]
		&& tmp->line.b.x == line[2] && tmp->line.b.y == line[3])
		{
			prev->next = tmp->next;
			free(tmp);
			tmp = NULL;
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

void	erase_line(t_doom *doom, t_poly *poly, int *line)
{
	t_poly		*tmp1;
	t_poly		*prev1;

	tmp1 = poly;
	prev1 = tmp1;
	doom->editor.sup = 0;
	while (tmp1)
	{
		erase_one_line(tmp1, line);
		if (tmp1->list == NULL)
		{
			if (tmp1 == poly)
			{
				// printf("%p\n", tmp1->next);
				poly = tmp1->next;
				free(tmp1);
				// tmp1 = NULL;
				return ;
			}
			else
			{
				prev1->next = tmp1->next;
				free(tmp1);
				// tmp1 = NULL;
				return ;
			}
		}
		prev1 = tmp1;
		tmp1 = tmp1->next;
		print_lst(poly);
	}
}
