/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 17:12:46 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/17 17:55:01 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "light.h"

int		create_default_light(t_doom *doom)
{
	const t_light	light = (t_light){
		.type = LIGHT_POINT,
		.position = (t_vec3){ 0, 0, 0 },
		.intensity = 255,
		.model_visible = TRUE,
	};

	if (!(append_lights_array(&doom->lights, light)))
		return (-1);
	return (doom->lights->len - 1);
}
