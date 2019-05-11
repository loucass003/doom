/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygon.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 22:39:14 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/11 16:39:00 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#define NEAR (0.1)

t_bool	prepare_polygon(t_polygon *poly)
{
	if (!poly->proj_vertices 
		&& !(poly->proj_vertices = create_2dvertices_array(
				poly->vertices->len * 2)))
		return (FALSE); //TODO: ERROR
	if (!poly->proj_vertices_buf 
		&& !(poly->proj_vertices_buf = create_2dvertices_array(
				poly->proj_vertices->capacity)))
		return (FALSE);
	if (poly->proj_vertices)
		poly->proj_vertices->len = 0;
	if (poly->proj_vertices_buf)
		poly->proj_vertices_buf->len = 0;
	return (TRUE);
}

void	fill_bottom_tri(t_doom *doom, int c, t_vec2 v1, t_vec2 v2, t_vec2 v3)
{
	const float	invslope1 = (v2.x - v1.x) / (v2.y - v1.y);
	const float	invslope2 = (v3.x - v1.x) / (v3.y - v1.y);
	t_vec2		curr;
	int			s;

	curr = (t_vec2){v1.x, v1.x};
	s = v1.y - 1;
	while (++s <= v2.y)
	{
		draw_line(&doom->screen, (t_pixel){curr.x, s, c}, (t_pixel){curr.y, s});
		curr = ft_vec2_add(curr, (t_vec2){ invslope1, invslope2 });
	}
}

void	fill_top_tri(t_doom *doom, int c, t_vec2 v1, t_vec2 v2, t_vec2 v3)
{
	const float	invslope1 = (v3.x - v1.x) / (v3.y - v1.y);
	const float	invslope2 = (v3.x - v2.x) / (v3.y - v2.y);
	t_vec2		curr;
	int			s;

	curr = (t_vec2){v3.x, v3.x};
	s = v3.y + 1;
	while (--s > v1.y)
	{
		draw_line(&doom->screen, (t_pixel){curr.x, s, c}, (t_pixel){curr.y, s});
		curr = ft_vec2_sub(curr, (t_vec2){ invslope1, invslope2 });
	}
}

void	swap(t_vec2 *a, t_vec2 *b)
{
	const t_vec2 tmp = *a;

	*a = *b;
	*b = tmp;
}

void	fill_triangle(t_doom *doom, int color, t_vec2 v1, t_vec2 v2, t_vec2 v3)
{
	if (v1.y > v2.y)
		swap(&v1, &v2);
	if (v1.y > v3.y)
		swap(&v1, &v3);
	if (v2.y > v3.y)
		swap(&v2, &v3);

	if (v2.y == v3.y)
		fill_bottom_tri(doom, color, v1, v2, v3);
	else if (v1.y == v2.y)
		fill_top_tri(doom, color, v1, v2, v3);
	else
	{
		t_vec2 v4 = {(int)(v1.x + ((float)(v2.y - v1.y) / (float)(v3.y - v1.y)) * (v3.x - v1.x)), v2.y};
		fill_bottom_tri(doom, color, v1, v2, v4);
		fill_top_tri(doom, color, v2, v4, v3);
	}
}

void	render_polygon(t_doom *doom, t_polygon *poly)
{
	fill_triangle(doom, 0xFF, (t_vec2){ 10, 100 }, (t_vec2){ 100, 50 }, (t_vec2){ 5, 200});

	t_vec2	last_b = (t_vec2){ NAN, NAN };

	if (!prepare_polygon(poly))
		return ; // TODO: ERROR
	int i = -1;
	while (++i < poly->vertices->len)
	{
		int next = (i + 1) % poly->vertices->len;
		// t_vec3 a = ft_mat4_mulv(rotation, ft_vec3_sub(*(t_vec3 *)(poly->vertices->values[i]), doom->player.pos));
	    // t_vec3 b = ft_mat4_mulv(rotation, ft_vec3_sub(*(t_vec3 *)(poly->vertices->values[next]), doom->player.pos));
		t_vec3 a = ft_mat4_mulv(doom->player.matrix, *(t_vec3 *)(poly->vertices->values[i]));
		t_vec3 b = ft_mat4_mulv(doom->player.matrix, *(t_vec3 *)(poly->vertices->values[next]));
		
		if (a.z < NEAR && b.z < NEAR)
			continue;

		if (a.z < NEAR) {
			t_vec3 dir = ft_vec3_sub(a, b);
			float t = (NEAR - b.z) / (dir.z);
			a = ft_vec3_add(ft_vec3_mul(dir, (t_vec3) { t, t, t }), b);
		}

		if (b.z < NEAR) {
			t_vec3 dir = ft_vec3_sub(a, b);
			float t = (NEAR - a.z) / (dir.z);
			b = ft_vec3_add(ft_vec3_mul(dir, (t_vec3) { t, t, t }), a);
		}

		a = ft_mat4_mulv(doom->player.projection, a);
		b = ft_mat4_mulv(doom->player.projection, b);

	
		t_pixel p = (t_pixel){
			((a.x + 0.5) / a.z) * S_WIDTH,
			S_HEIGHT_2 + (a.y / a.z) * S_HEIGHT + (poly->type == P_FLOOR), 
			poly->type == P_FLOOR ? 0x00FF00 : 0xFF0000
		};
		t_pixel p2 = (t_pixel){
			((b.x + 0.5) / b.z) * S_WIDTH,
			S_HEIGHT_2 + (b.y / b.z) * S_HEIGHT + (poly->type == P_FLOOR),
			0xFF0000
		};
		t_vec2 p_b = (t_vec2){
			((b.x + 0.5) / b.z) * S_WIDTH,
			S_HEIGHT_2 + (b.y / b.z) * S_HEIGHT
		};
		t_vec2 p_a = (t_vec2){
			((a.x + 0.5) / a.z) * S_WIDTH,
			S_HEIGHT_2 + (a.y / a.z) * S_HEIGHT
		};
	/*	append_2dvertices_array(&poly->proj_vertices, p_b);
		append_2dvertices_array(&poly->proj_vertices, p_a);
		printf("%f %f - %f %f\n", p_a.x, p_a.y, p_b.x, p_b.y);*/
		draw_line(&doom->screen, p, p2);
	}
	i = -1;
	while (++i < poly->proj_vertices->len)
	{
		int next = (i + 1) % poly->proj_vertices->len;
		t_vec2 a = poly->proj_vertices->vertices[i];
		t_vec2 b = poly->proj_vertices->vertices[next];
	//	printf("%f %f - %f %f\n", a.x, a.y, b.x, b.y);
	//	draw_line(&doom->screen, (t_pixel){ a.x, a.y, poly->type == P_FLOOR ? 0x00FF00 : 0xFF0000 }, (t_pixel){b.x, b.y, 0});
	}
}
