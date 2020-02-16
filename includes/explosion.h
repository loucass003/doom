/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   explosion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 13:59:44 by lloncham          #+#    #+#             */
/*   Updated: 2020/02/16 14:40:23 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPLOSION_H
# define EXPLOSION_H

typedef struct	s_explosion
{
	int			cell_x;
	int			cell_y;
	float		t0;
}				t_explosion;

t_bool			update_renderable_explosion(t_render_context *ctx,
					t_renderable *r);
t_bool			create_explosion_renderable(t_doom *doom, t_renderable *r);

#endif
