/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_obj.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/10 15:44:57 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/25 10:53:16 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "maths/vec4.h"
#include "maths/triangle.h"
#include "obj.h"
#include "render.h"
#include "doom.h"

float	get_light_intensity(t_render_context *ctx, t_vec3 normal, t_vec4 point)
{
	int		i;
	float	sum;
	int		valid;
	t_light	*light;
	
	i = -1;
	sum = 0;
	valid = 0;
	while (++i < ctx->doom->lights->len)
	{
		light = &ctx->doom->lights->values[i];
		// float d = ft_vec3_dot(normal, ft_vec3_sub(light->position, vec4_to_3(point)));
		// if (d > 5)
		// 	continue;
		valid++;
		light->rotation.y -=light->rotation.y;
		sum += ft_max(AMBIANT_LIGHT, fmin(1, fmax(0, ft_vec3_dot(normal, ft_vec3_sub(light->rotation, light->position)))) * 255);
	}
	if (valid == 0)
		return 25;
	return sum / (float)valid;
}
