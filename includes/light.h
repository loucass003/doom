/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 04:25:19 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/12 14:00:14 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHT_H
# define LIGHT_H

#include <libft.h>

typedef enum		s_light_type
{
	LIGHT_POINT,
	LIGHT_SPOT,
}					t_light_type;


typedef struct		s_light
{
	t_light_type		type;
	t_vec3				position;
	t_vec3				dir;
	t_bool				fixed;
}					t_light;

typedef struct		s_lightmap
{
	size_t			width;
	size_t			height;
	int				common_hit;
	char			*map;
}					t_lightmap;

typedef struct		s_lights
{
	int				len;
	int				capacity;
	t_light			values[];
}					t_lights;

t_lights		*create_lights_array(int capacity);
t_lights		*append_lights_array(t_lights **arr, t_light light);
t_lights		*splice_lights_array(t_lights *arr,
						int index, int n);
t_lights		*copy_lights_array(t_lights *src,
						t_lights **dst);

#endif
