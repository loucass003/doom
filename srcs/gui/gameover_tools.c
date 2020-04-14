/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameover_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 19:26:33 by Lisa              #+#    #+#             */
/*   Updated: 2020/04/14 19:28:12 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gui.h"
#include "doom.h"

int					total_score(t_doom *doom)
{
	int	totalscore;

	if (doom->gameover.kill == 0 || doom->gameover.bullets == 0)
		totalscore = (doom->gameover.weapon * 30)
			+ (doom->gameover.totaldamage * 100.);
	else
		totalscore = ((doom->gameover.kill * 20) / doom->gameover.bullets)
			+ (doom->gameover.weapon * 30)
			+ (doom->gameover.totaldamage * 100.);
	return (totalscore);
}

t_bool				add_score(char *text, int score, t_doom *doom)
{
	t_score		*s;
	int			i;

	i = -1;
	while (++i < 5)
	{
		s = &doom->scores[i];
		if (s->score >= score)
			continue;
		add_score(s->text, s->score, doom);
		s->score = score;
		ft_bzero(s->text, 255);
		ft_memcpy(s->text, text, ft_strlen(text));
		break;
	}
	return (TRUE);
}
