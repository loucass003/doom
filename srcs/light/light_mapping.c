/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_mapping.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 14:22:33 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/31 03:13:03 by llelievr         ###   ########.fr       */
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
	if (!(doom->lights = (t_light *)malloc(sizeof(t_light))))
		return ;
/* 	t_render_context	context;
	
	context = (t_render_context){ .type = CTX_SHADOW_MAPPING };
	if(!(context.buffer = (t_zbuff *)malloc((int)(S_WIDTH * S_HEIGHT) * sizeof(t_zbuff))))
		return ;
	t_camera *cam = malloc(sizeof(t_camera));
	context.camera = cam;
	cam->projection = projection_matrix(); */
	doom->lights[0] = (t_light) {
		.position = { 4, 1, 6 },
		.rotation = ((t_vec3){ 0, -M_PI_4, 0 })
		// .context = context
	};

	doom->rays = malloc(S_WIDTH * S_HEIGHT * sizeof(t_ray_collide));
	// cam->pos = doom->lights[0].position;
	// cam->rotation = doom->lights[0].rotation;
	// camera_update_maxtrix(cam);
}

t_ray			create_ray(t_light *light, t_vec3 direction)
{
	t_vec3 d = vec3_rotate(direction, (t_vec3){-light->rotation.x, -light->rotation.y, -light->rotation.z});
	// if (d.x == 0. && d.y == 0. && d.z == 0.)	
	// 	d = direction;
	//printf("DIR %f %f %f\n", d.x, d.y, d.x);
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
			//printf("HEY! %f\n", hit.dist);
			if (hit.collide && hit.dist > 0 && hit.dist < dist)
			{
				dist = hit.dist;
				min = hit;
			}
		}
	}
	return (min);
}

void		init_lightning(t_doom *doom)
{
	int		i;
	t_light	*light;

	create_lights(doom);
	i = -1;
	while (++i < 1)
	{
		light = &doom->lights[i];
		for (int y = 0; y < S_HEIGHT; y++)
		{
			for (int x = 0; x < S_WIDTH; x++)
			{
				int y_inv = y;
				t_ray ray = create_ray(light, ft_vec3_norm((t_vec3){ 
					(x - S_WIDTH_2) / S_WIDTH,
					(y_inv - S_HEIGHT_2) / S_HEIGHT,
					1
				}));
				t_collision hit = hit_scene(doom, &ray);
				if (hit.collide && hit.who.type == COLLIDE_TRIANGLE)
				{
					doom->rays[y * (int)S_WIDTH + x] = (t_ray_collide) {
						.ray = ray,
						.collision = hit
					};
					t_collide_triangle tri = (t_collide_triangle)hit.who.data.triangle;
					t_vec2 uv = hit.uv;
					int x = (tri.polygon->texture->w * 2 - 1) * (tri.polygon->type == P_WALL ? uv.x : 1. - uv.x);
					int y = (tri.polygon->texture->h * 2 - 1) * (tri.polygon->type == P_WALL ? uv.y : 1. - uv.y);
					int index = y * (tri.polygon->texture->w * 2) + x;
					tri.polygon->lightmap[index] = 255;
				}
				
			}
		}
	}
}
