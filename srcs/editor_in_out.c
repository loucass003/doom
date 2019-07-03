/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_in_out.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/25 16:32:13 by lloncham          #+#    #+#             */
/*   Updated: 2019/07/03 14:29:12 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_bool     in_the_poly(t_doom *doom, t_save *lst, t_vec2 point)
{
    t_save      *list;
    t_line_list *line;
    int         res;
    int         prev;

    list = lst;
    prev = 0;
    while (list && list->floor != doom->editor.floor)
        list = list->next;
    line = list->line;
    while (line)
    {
        res = (line->line.b.x - line->line.a.x) * (point.y - line->line.a.y) - (point.x - line->line.a.x) * (line->line.b.y - line->line.a.y);
        if (res == 0)
            return (TRUE);
        if (prev != 0 && (res / ft_abs(res)) != (prev / ft_abs(prev)))
        {
            doom->editor.alert[3] = 1;
            return (FALSE);
        }
        prev = res;
        line = line->next;
    }
    return (TRUE);
}