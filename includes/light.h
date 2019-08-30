/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 04:25:19 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/30 06:48:34 by llelievr         ###   ########.fr       */
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

#endif