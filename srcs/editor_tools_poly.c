/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tools_poly.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 17:33:38 by lloncham          #+#    #+#             */
/*   Updated: 2019/07/03 17:26:39 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void	print_lst(t_doom *doom, t_save *list)
{
	t_save		*tmp;
	t_line_list *cmp;

	tmp = list;
	while (tmp != NULL)
	{
		printf("FLOOR %d\n", tmp->floor);
		printf("FLOOR %d\n", doom->editor.floor);
		cmp = tmp->line;
		while (cmp != NULL)
		{
			printf("x = %f - y = %f  ||  ", cmp->line.a.x, cmp->line.a.y);
			printf("x = %f - y = %f\n\n", cmp->line.b.x, cmp->line.b.y);
			cmp = cmp->next;
		}
		tmp = tmp->next;
	}
	printf("\n\n");
}


int		ft_listlen(t_save *poly)
{
	t_save		*tmp;
	int			i;

	i = 0;
	tmp = poly;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}
