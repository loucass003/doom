/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_mapping.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 14:22:33 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/30 16:36:33 by llelievr         ###   ########.fr       */
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
		.position = { 1, 1, 6 },
		.rotation = ((t_vec3){ 0, 0, 0 })
		// .context = context
	};
	// cam->pos = doom->lights[0].position;
	// cam->rotation = doom->lights[0].rotation;
	// camera_update_maxtrix(cam);
}

t_ray			create_ray(t_light *light, t_vec3 direction)
{
	t_vec3 d = vec3_rotate(direction, (t_vec3){-light->rotation.x, light->rotation.y, 0});
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
			if (hit.collide && hit.dist < dist)
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
		for (int y = 0; y < S_HEIGHT_2; y+=2)
		{
			for (int x = 0; x < S_WIDTH_2; x+=2)
			{
				int y_inv = y;
				t_ray ray = create_ray(light, ft_vec3_norm((t_vec3){ 
					((x + 0.5) / S_WIDTH - 0.5),
					((y_inv + 0.5) / S_HEIGHT - 0.5) * (S_WIDTH / S_HEIGHT),
					1
				}));
				t_collision hit = hit_scene(doom, &ray);
				if (hit.collide && hit.who.type == COLLIDE_TRIANGLE)
				{
				//	printf("CALL\n");
					t_collide_triangle tri = (t_collide_triangle)hit.who.data.triangle;
					t_vec2 uv = hit.uv;
					int x = (tri.polygon->texture->w - 1) * uv.x;
					int y = (tri.polygon->texture->h - 1) * uv.y;
					int index = y * tri.polygon->texture->w + x;
					ft_memset(tri.polygon->lightmap + index, 255, sizeof(uint8_t));
				}
				
			}
		}
	}
}
