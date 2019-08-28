/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_mapping.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 14:22:33 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/28 15:28:14 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void		init_lightning(t_doom *doom)
{
	int		x;
	int		y;
	int		i;

	i = -1;
	while (++i < 1)
	{
		y = -1;
		while (++y < S_HEIGHT)
		{
			x = -1;
			while (++x < S_WIDTH)
			{
				t_ray ray = (t_ray){
					.origin = 
				}
			}
		}
	}
}
