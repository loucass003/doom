/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/15 17:42:59 by Lisa              #+#    #+#             */
/*   Updated: 2020/04/15 17:46:44 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void			init_gameover(t_doom *doom)
{
	t_gameover		*gameover;

	gameover = &doom->gameover;
	gameover->kill = 0;
	gameover->bullets = 0;
	gameover->weapon = 0;
	gameover->totaldamage = 0;
}
