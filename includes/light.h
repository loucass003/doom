/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 04:25:19 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/02 18:31:12 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHT_H
# define LIGHT_H

#include "render.h"
#include <libft.h>

typedef struct		s_light
{
	t_vec3				position;
	t_vec3				rotation;
	t_render_context	context;
}					t_light;

typedef struct		s_lights
{
	int				len;
	int				capacity;
	t_light			lights[];
}					t_lights;

t_lights		*create_lights_array(int capacity);
t_lights		*append_lights_array(t_lights **arr, t_light light);
t_lights		*splice_lights_array(t_lights *arr,
						int index, int n);
t_lights		*copy_lights_array(t_lights *src,
						t_lights **dst);

#endif