/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_mapping.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 14:22:33 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/10 01:48:15 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <math.h>
#include "light.h"
#include "collision.h"
#include "doom.h"
#include "render.h"
#include "octree.h"
#include "maths/mat4.h"

 void		create_lights(t_doom *doom)
{
	if (!(doom->lights = create_lights_array(2)))
		return ;
	// append_lights_array(&doom->lights, (t_light) {
	// 	.position = { 2, 1.499, 8 },
	// 	.rotation = ((t_vec3){ M_PI - -M_PI_4,  M_PI, 0 })
	// });
	append_lights_array(&doom->lights,(t_light) {
		.position = { 20, 0, 70 },
		.dir = ((t_vec3){ -1, 0, 0 })
	});
}

// t_ray			create_ray(t_light *light, t_vec3 direction)
// {
// 	t_vec3 d = vec3_rotate(direction, (t_vec3){-light->rotation.x, light->rotation.y, -light->rotation.z});
// 	return ((t_ray){
// 		.origin = light->position,
// 		.direction = d
// 	});
// }

// t_collision		hit_scene(t_doom *doom, t_ray *ray) 
// {
// 	t_collision	min;
// 	t_collision	hit;
// 	int		i;
// 	int		j;

// 	min = (t_collision) {
// 		.collide = FALSE,
// 		.dist = -1.0
// 	};
// 	float dist = INT_MAX;
// 	i = -1;
// 	while (++i < doom->renderables->len)
// 	{
// 		t_renderable *r = &doom->renderables->values[i];
// 		if (!r->fixed)
// 			continue;
// 		if (FALSE && r->octree)
// 		{
// 			//printf("OCTREE\n");
			
// 			t_ray copy = (t_ray){};
// 			t_mat4 m;
// 			if (!mat4_inverse(ft_mat4_rotation(r->rotation), &m))
// 				continue;
// 			t_vec3 scale_inv = ft_vec3_div((t_vec3){1, 1, 1}, r->scale);
// 			copy.direction = ft_mat4_mulv(m, ray->direction);
// 			copy.direction = ft_vec3_mul(copy.direction, scale_inv);
// 			copy.direction = ft_vec3_norm(copy.direction);
// 			copy.origin = ft_vec3_sub(ray->origin, r->position);
// 			copy.origin = ft_mat4_mulv(m, copy.origin);
// 			copy.origin = ft_vec3_mul(copy.origin, scale_inv);
// 			hit = ray_hit_collidable(&copy, &r->octree->box);
// 			// printf("copy O -> %f %f %f, R -> %f %f %f\n", copy.origin.x, copy.origin.y, copy.origin.z, copy.direction.x, copy.direction.y, copy.direction.z);
// 			// printf("ray O -> %f %f %f, R -> %f %f %f\n", ray->origin.x, ray->origin.y, ray->origin.z, ray->direction.x, ray->direction.y, ray->direction.z);
// 			hit.renderable = r;
// 			hit.ray = copy;
// 			if (hit.collide)
// 			{
// 				t_vec3 point = copy.origin;
// 				point = ft_vec3_add(point, ft_vec3_mul_s(copy.direction, hit.dist));
// 				point = ft_vec3_mul(point, r->scale);
// 				point = ft_vec3_mul(point, r->position);
// 				hit.tmp_dist = hit.dist;
// 				hit.dist = ft_vec3_len(ft_vec3_sub(ray->origin, point));
// 				hit.point = point;
// 		//		printf("%f\n", hit.dist);
// 				if (hit.dist >= 0 && hit.dist <= dist)
// 				{
// 			//		printf("HIT_OCTREE\n");
// 					dist = hit.dist;
// 					min = hit;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			j = -1;
// 			while (++j < r->faces->len)
// 			{
// 				hit = ray_hit_collidable(ray, &r->faces->values[j].pp_collidable);
// 				hit.renderable = r;
// 				hit.ray = *ray;
// 				if (hit.collide && hit.dist >= 0 && hit.dist <= dist)
// 				{
// 					dist = hit.dist;
// 					min = hit;
// 				}
// 			}
// 		}
// 	}
// 	return (min);
// }

// short		get_pixel(t_mtl *mtl, int x, int y)
// {
// 	if (x < 0 || x >= mtl->texture_map->w || y < 0 || y >= mtl->texture_map->h)
// 		return -1;
// 	return mtl->lightmap[y * (mtl->texture_map->w) + x];
// }

// void get_adjacent(t_mtl *mtl, short *pixels, int coef, int x, int y)
// {
// 	int		i;
// 	int		j;

// 	i = -1;
// 	while (++i < coef)
// 	{
// 		j = -1;
// 		while (++j < coef)
// 		{
// 			uint8_t value = get_pixel(mtl, x - coef / 2 + j, y - coef / 2 + i);
// 			if (value == AMBIANT_LIGHT)
// 				value = fmax(0, value - 30);
// 			pixels[i * coef + j] = value;
// 		}
// 	}
// }

// void		blur_poly_shading(t_mtl *mtl)
// {
// 	int		coef = 3;
// 	short	pixels[3 * 3];
// 	int		x;
// 	int		y;
// 	int		add;
// 	int		valid;

// 	uint8_t	*copy = malloc(mtl->texture_map->w * mtl->texture_map->h * sizeof(uint8_t));
// 	ft_memcpy(copy, mtl->lightmap, mtl->texture_map->w * mtl->texture_map->h * sizeof(uint8_t));

// 	y = -1;
// 	while (++y < mtl->texture_map->h)
// 	{
// 		x = -1;
// 		while (++x < mtl->texture_map->w)
// 		{
// 			get_adjacent(mtl, pixels, coef, x, y);
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
// 			copy[y * (mtl->texture_map->w) + x] = fmax(AMBIANT_LIGHT, fmin(255, (float)add / (float)valid));
// 		}
// 	}
// 	free(mtl->lightmap);
// 	mtl->lightmap = copy;
// }


void		init_lightning(t_doom *doom)
{
	int		i;
	t_light	*light;

	printf("INIT_LIGHTS\n");
	create_lights(doom);
	// float width = 400;
	// float height = 400;
	// i = -1;
	// while (++i < doom->lights->len)
	// {
	// 	light = &doom->lights->lights[i];
	// 	for (int y = 0; y < height; y++)
	// 	{
	// 		for (int x = 0; x < width; x++)
	// 		{
	// 			if((x - 200)*(x - 200) +  (y - 200) * (y - 200) > 200*200)
	// 			 	continue ;
	// 			float x_inv = width - x;
	// 			float y_inv = y;
	// 			t_ray ray = create_ray(light, ft_vec3_norm((t_vec3){ 
	// 				((x_inv + 0.5) / width - 0.5),
	// 				((y_inv + 0.5) / height - 0.5) * (height / width),
	// 				1
	// 			}));
	// 			t_collision hit = hit_scene(doom, &ray);
	// 			// if (hit.collide && hit.who.type == COLLIDE_AABB)
	// 			// {
	// 			// //	printf("HIT_OCTREE 2\n");
	// 			// 	if (x == 200 && y == 200)
	// 			// 	{
	// 			// 		printf("copy O -> %f %f %f, R -> %f %f %f\n", hit.ray.origin.x, hit.ray.origin.y, hit.ray.origin.z, hit.ray.direction.x, hit.ray.direction.y, hit.ray.direction.z);
	// 			// 		printf("ray O -> %f %f %f, R -> %f %f %f\n", ray.origin.x, ray.origin.y, ray.origin.z, ray.direction.x, ray.direction.y, ray.direction.z);
	// 			// 		printf("point %f %f %f\n", hit.point.x, hit.point.y, hit.point.z);
	// 			// 		printf("tmp_dist %f\n", hit.tmp_dist);
	// 			// 		printf("dist %f\n", hit.dist);
	// 			// 	}
	// 			// }
	// 			if (hit.collide && hit.who.type == COLLIDE_TRIANGLE && hit.renderable)
	// 			{
	// 				t_collide_triangle tri = (t_collide_triangle)hit.who.data.triangle;
	// 				if (tri.parent_type == PARENT_COLLIDER_OBJ)
	// 					continue;
	// 				t_vec2 uv = hit.uv;
	// 				t_mtl *mtl = hit.renderable->faces->values[tri.face].mtl;
	// 				int x0 = (float)(mtl->texture_map->w - 1) * (uv.x);
	// 				int y0 = (float)(mtl->texture_map->h - 1) * (1 - uv.y);
	// 				int index = y0 * (mtl->texture_map->w) + x0;
	// 			//	float diffuseFactor = fmin(1, fmax(AMBIANT_LIGHT / 255, -ft_vec3_dot(tri.normal, ray.direction) * 4));
	// 				float diffuseFactor = 1;
	// 				if (!mtl->lightmap)
	// 				{
	// 					const int size = mtl->texture_map->w * mtl->texture_map->h * sizeof(uint8_t);
	// 					if(mtl->texture_map_set && !(mtl->lightmap = (uint8_t *)malloc(size)))
	// 						return ;
	// 					ft_memset(mtl->lightmap, AMBIANT_LIGHT, size);
	// 				}
	// 				//printf("CALL\n");
	// 				if (mtl->lightmap)
	// 					mtl->lightmap[index] = fmax(AMBIANT_LIGHT, fmin(255, diffuseFactor * 255));
	// 			}
	// 		}
	// 	}
	// }
	// //exit(0);
	// for (int i = 0; i < doom->renderables->len; i++)
	// {
	// 	t_renderable *r = &doom->renderables->values[i];
	// 	for (int j = 0; j < r->faces->len; j++)
	// 	{
	// 		if (r->faces->values[j].mtl->lightmap)
	// 			blur_poly_shading(r->faces->values[j].mtl);
	// 	}
	// }
}

