/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_mapping.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 14:22:33 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/10 13:22:28 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <math.h>
#include "light.h"
#include "collision.h"
#include "doom.h"
#include "render.h"
#include "octree.h"

 void		create_lights(t_doom *doom)
{
	if (!(doom->lights = create_lights_array(2)))
		return ;
	// append_lights_array(&doom->lights, (t_light) {
	// 	.position = { 2, 1.499, 8 },
	// 	.rotation = ((t_vec3){ M_PI - -M_PI_4,  M_PI, 0 })
	// });
	append_lights_array(&doom->lights,(t_light) {
		.position = { 1.5, 1.499, 1 },
		.rotation = ((t_vec3){ -M_PI_4, M_PI, 0 })
	});
}

t_ray			create_ray(t_light *light, t_vec3 direction)
{
	t_vec3 d = vec3_rotate(direction, (t_vec3){-light->rotation.x, -light->rotation.y, -light->rotation.z});
	return ((t_ray){
		.origin = light->position,
		.direction = d
	});
}

t_collision		hit_scene(t_doom *doom, t_ray *ray) 
{
	t_collision	min;
	t_collision	hit;
	int		i;
	int		j;

	min = (t_collision) {
		.collide = FALSE,
		.dist = -1.0
	};
	float dist = INT_MAX;
	i = -1;
	while (++i < doom->renderables->len)
	{
		t_renderable *r = &doom->renderables->values[i];
		if (!r->fixed)
			continue;
		if (r->octree)
		{
			//printf("OCTREE\n");
			t_ray copy;
			t_mat4 m = ft_mat4_mul(
				ft_mat4_rotation(ft_vec3_inv(r->rotation)),
				ft_mat4_scale(ft_vec3_inv(r->scale))
			);
			copy.origin = ft_vec3_sub(ray->origin, r->position);
			copy.direction = ft_mat4_mulv(m, ray->direction);
			copy.origin = ft_mat4_mulv(m, ray->origin);
			hit = ray_hit_collidable(&copy, &r->octree->box);
			hit.renderable = r;
			if (hit.collide && hit.dist >= 0 && hit.dist <= dist)
			{
				printf("HIT_OCTREE\n");
				dist = hit.dist;
				min = hit;
			}
		}
		else
		{
			j = -1;
			while (++j < r->faces->len)
			{
				hit = ray_hit_collidable(ray, &r->faces->values[j].collidable);
				hit.renderable = r;
				if (hit.collide && hit.dist >= 0 && hit.dist <= dist)
				{
					dist = hit.dist;
					min = hit;
				}
			}
		}
	}
	return (min);
}

// short		get_pixel(t_polygon *poly, int x, int y)
// {
// 	if (x < 0 || x >= poly->texture->w || y < 0 || y >= poly->texture->h)
// 		return -1;
// 	return poly->lightmap[y * (poly->texture->w) + x];
// }

// void get_adjacent(t_polygon *poly, short *pixels, int coef, int x, int y)
// {
// 	int		i;
// 	int		j;

// 	i = -1;
// 	while (++i < coef)
// 	{
// 		j = -1;
// 		while (++j < coef)
// 		{
// 			pixels[i * coef + j] = get_pixel(poly, x - coef / 2 + j, y - coef / 2 + i);
// 		}
// 	}
// }

// void		blur_poly_shading(t_polygon *poly)
// {
// 	int		coef = 3;
// 	short	pixels[3 * 3];
// 	int		x;
// 	int		y;
// 	int		add;
// 	int		valid;

// 	y = -1;
// 	while (++y < poly->texture->h)
// 	{
// 		x = -1;
// 		while (++x < poly->texture->w)
// 		{
// 			get_adjacent(poly, pixels, coef, x, y);
// 			valid = 0;
// 			add = 0;
// 			for (int i = 0; i < coef * coef; i++)
// 			{
// 				short pixel = pixels[i];
// 				if (pixel == -1)
// 					continue;
// 				valid++;
// 				add += pixel;
// 			}
// 			if (valid == 0)
// 				continue;
// 			poly->lightmap[y * (poly->texture->w) + x] = fmax(AMBIANT_LIGHT, fmin(255, (float)add / (float)valid));
// 		}
// 	}
// }


void		init_lightning(t_doom *doom)
{
	int		i;
	t_light	*light;

	printf("INIT_LIGHTS\n");
	create_lights(doom);
	float width = 200;
	float height = 200;
	i = -1;
	while (++i < doom->lights->len)
	{
		light = &doom->lights->lights[i];
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				if((x - 100)*(x - 100) +  (y - 100) * (y - 100) > 100*100)
				 	continue ;
				float x_inv = width - x;
				float y_inv = y;
				t_ray ray = create_ray(light, ft_vec3_norm((t_vec3){ 
					((x_inv + 0.5) / width - 0.5),
					((y_inv + 0.5) / height - 0.5) * (height / width),
					1
				}));
				t_collision hit = hit_scene(doom, &ray);
				if (hit.collide && hit.who.type == COLLIDE_AABB)
				{
					printf("HIT_OCTREE 2\n");
				}
				if (hit.collide && hit.who.type == COLLIDE_TRIANGLE && hit.renderable)
				{
					t_collide_triangle tri = (t_collide_triangle)hit.who.data.triangle;
					if (tri.parent_type == PARENT_COLLIDER_OBJ)
						continue;
					t_vec2 uv = hit.uv;
					t_mtl *mtl = hit.renderable->faces->values[tri.face].mtl;
					int x0 = (float)(mtl->texture_map->w) * (uv.x);
					int y0 = (float)(mtl->texture_map->h) * (1 - uv.y);
					int index = y0 * (mtl->texture_map->w) + x0;
					// float diffuseFactor = fmax(0.5, -ft_vec3_dot(tri.normal, ray.direction) * 1.2);
					float diffuseFactor = 1;
					if (!mtl->lightmap)
					{
						if(!(mtl->lightmap = (uint8_t *)malloc(mtl->texture_map->w * mtl->texture_map->h * sizeof(uint8_t))))
							return ;
						ft_memset(mtl->lightmap, AMBIANT_LIGHT, mtl->texture_map->w * mtl->texture_map->h * sizeof(uint8_t));
					}
					//printf("CALL\n");
					mtl->lightmap[index] = fmax(AMBIANT_LIGHT, fmin(255, diffuseFactor * 255));
				}
			}
		}
	}
	// for (int i = 0; i < doom->polygons->len; i++)
	// {
	// 	if (doom->polygons->polygons[i].lightmap)
	// 		blur_poly_shading(doom->polygons->polygons + i);
	// }
}

