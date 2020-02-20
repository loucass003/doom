/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 17:12:46 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/20 17:10:28 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "light.h"

int			create_default_light(t_doom *doom)
{
	t_light	light;

	light = (t_light){
		.type = LIGHT_POINT,
		.position = (t_vec3){ 0, 0, 0 },
		.dir = ((t_vec3){ 0, 0, 1 }),
		.intensity = 255,
		.model_visible = TRUE,
	};
	if (!(append_lights_array(&doom->lights, light)))
		return (-1);
	return (doom->lights->len - 1);
}

void		init_lightning(t_doom *doom)
{
	if (!(doom->lights = create_lights_array(2)))
		return ;
	append_lights_array(&doom->lights, (t_light) {
		.position = { 0, 0, 0 },
		.dir = ((t_vec3){ -1, 0, 0 }),
		.type = LIGHT_POINT,
		.intensity = 255
	});
}
