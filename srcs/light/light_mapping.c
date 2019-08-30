/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_mapping.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 14:22:33 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/30 08:46:31 by llelievr         ###   ########.fr       */
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
		.position = { 2.2, 1, 6 },
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


void		init_lightning(t_doom *doom)
{
	int		i;
	t_light	*light;
	

	create_lights(doom);
	// doom->main_context.lights = doom->lights;
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
					((x + 0.5) / S_WIDTH - 0.5), 
					((y_inv + 0.5) / S_HEIGHT - 0.5) * ((float)S_HEIGHT / (float)S_WIDTH), 
					1 
				}));
				float dist = INT_MAX;
				for (int i = 0; i < doom->polygons->len; i++)
				{
					t_polygon *poly = &doom->polygons->polygons[i];
					poly->pointed = FALSE;
					int triangles = floorf(poly->indices->len / 3.);
					for (int j = 0; j < triangles; j++)
					{
						
						t_collision colision = ray_hit_collidable(&ray, poly->collidables + j);
						if (colision.collide && colision.dist != -1 && colision.dist < dist)
						{
							dist = colision.dist;
							poly->pointed = TRUE;
							doom->main_context.pointed_polygon = poly;
							//doom->player.pointed_triangle = j;
						}
					}
				}
				if (dist == INT_MAX)
				{
					// doom->player.pointed_triangle = -1;
					doom->main_context.pointed_polygon = NULL;
				}
			}
		}
	}
}
