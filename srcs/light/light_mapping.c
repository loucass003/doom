/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_mapping.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 14:22:33 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/02 19:34:36 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <math.h>
#include "light.h"
#include "collision.h"
#include "doom.h"
#include "render.h"

void		create_lights(t_doom *doom)
{
	if (!(doom->lights = create_lights_array(2)))
		return ;
	append_lights_array(&doom->lights, (t_light) {
		.position = { 2, 1.2, 8 },
		.rotation = ((t_vec3){ M_PI - -M_PI_4, 0, 0 })
	});
	append_lights_array(&doom->lights,(t_light) {
		.position = { 2.4, 1.2, 1 },
		.rotation = ((t_vec3){ -M_PI_4, 0, 0 })
	});

//	doom->rays = malloc(S_WIDTH * S_HEIGHT * sizeof(t_ray_collide));
}

t_ray			create_ray(t_light *light, t_vec3 direction)
{
	t_vec3 d = vec3_rotate(direction, (t_vec3){-light->rotation.x, light->rotation.y, -light->rotation.z});
	return ((t_ray){
		.origin = light->position,
		.direction = d
	});
}

t_collision		hit_scene(t_doom *doom, t_ray *ray) {
	t_collision	min;
	t_collision	hit;
	int		i;
	int		j;

	i = -1;
	min = (t_collision) {
		.collide = FALSE,
		.dist = -1.0
	};
	float dist = INT_MAX;
	while (++i < doom->polygons->len)
	{
		t_polygon *poly = &doom->polygons->polygons[i];
		int triangles = floorf(poly->indices->len / 3.);
		j = -1;
		while (++j < triangles)
		{
			hit = ray_hit_collidable(ray, poly->collidables + j);
			if (hit.collide && hit.dist >= 0 && hit.dist <= dist)
			{
				dist = hit.dist;
				min = hit;
			}
		}
	}
	return (min);
}

short		get_pixel(t_polygon *poly, int x, int y)
{
	if (x < 0 || x >= poly->texture->w || y < 0 || y >= poly->texture->h)
		return -1;
	return poly->lightmap[y * (poly->texture->w) + x];
}

void get_adjacent(t_polygon *poly, short *pixels, int coef, int x, int y)
{
	int		i;
	int		j;

	i = -1;
	while (++i < coef)
	{
		j = -1;
		while (++j < coef)
		{
			pixels[i * coef + j] = get_pixel(poly, x - coef / 2 + j, y - coef / 2 + i);
		}
	}
}

void		blur_poly_shading(t_polygon *poly)
{
	int		coef = 7;
	short	pixels[7 * 7];
	int		x;
	int		y;
	int		add;
	int		valid;



	y = -1;
	while (++y < poly->texture->h)
	{
		x = -1;
		while (++x < poly->texture->w)
		{
			get_adjacent(poly, pixels, coef, x, y);
			valid = 0;
			add = 0;
			for (int i = 0; i < coef * coef; i++)
			{
				short pixel = pixels[i];
				if (pixel == -1)
					continue;
				valid++;
				add += pixel;
			}
			if (valid == 0)
				continue;
			poly->lightmap[y * (poly->texture->w) + x] = fmax(50, fmin(255, (float)add / (float)valid));
		}
	}
}


void		init_lightning(t_doom *doom)
{
	int		i;
	t_light	*light;

	create_lights(doom);
	float width = 600;
	float height = 600;
	i = -1;
	while (++i < doom->lights->len)
	{
		light = &doom->lights->lights[i];
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				if((x - 300)*(x - 300) +  (y - 300) * (y - 300) > 300*300)
				 	continue ;
				float x_inv = width - x;
				float y_inv = y;
				t_ray ray = create_ray(light, ft_vec3_norm((t_vec3){ 
					((x_inv + 0.5) / width - 0.5),
					((y_inv + 0.5) / height - 0.5) * (height / width),
					1
				}));
				t_collision hit = hit_scene(doom, &ray);
				if (hit.collide && hit.who.type == COLLIDE_TRIANGLE)
				{
					t_collide_triangle tri = (t_collide_triangle)hit.who.data.triangle;
					t_vec2 uv = hit.uv;
					float w = (1. - uv.x - uv.y);
					int x0 = (float)(tri.polygon->texture->w) * (uv.x);
					int y0 = (float)(tri.polygon->texture->h) * (1 - uv.y);
					int index = y0 * (tri.polygon->texture->w) + x0;
					float diffuseFactor = fmax(0.3, -ft_vec3_dot(tri.normal, ray.direction));
					tri.polygon->lightmap[index] = fmin(255, diffuseFactor * 255);
					// doom->rt.pixels[y * (int)doom->rt.width + x] = ft_color_i((t_color){ (1. - uv.x - uv.y) * 255,  uv.y * 255, uv.x * 255, 255 });
				}
				else
					;
					// doom->rt.pixels[y * (int)doom->rt.width + x] = 0xFF252525;
			}
		}
	}
	for (int i = 0; i < doom->polygons->len; i++)
	{
		blur_poly_shading(doom->polygons->polygons + i);
	}
}

