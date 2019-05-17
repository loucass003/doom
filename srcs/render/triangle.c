/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 01:17:41 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/17 01:25:13 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void	draw_triangle(t_doom *doom, t_triangle3d triangle)
{
	draw_line(&doom->screen, (t_pixel){triangle.a.x, triangle.a.y, 0xFFFFFF}, (t_pixel){triangle.b.x, triangle.b.y});
	draw_line(&doom->screen, (t_pixel){triangle.b.x, triangle.b.y, 0xFFFFFF}, (t_pixel){triangle.c.x, triangle.c.y});
	draw_line(&doom->screen, (t_pixel){triangle.c.x, triangle.c.y, 0xFFFFFF}, (t_pixel){triangle.a.x, triangle.a.y});
}