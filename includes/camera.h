/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 17:40:59 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/22 13:39:56 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

#include <libft.h>
#include "collision.h"

typedef struct		s_camera
{
	t_vec3			pos;
	t_vec3			rotation;
	t_vec3			forward;
	t_mat4			matrix;
	t_mat4			frustum_matrix;
	t_mat4			projection;
	t_vec4 			frustum[6];
}					t_camera;

void				camera_update_maxtrix(t_camera *camera);
t_mat4				projection_matrix();
t_bool				aabb_vs_frustrum(t_collide_aabb aabb, t_vec4 *planes);
void				compute_frustum_planes(t_mat4 m, t_vec4 *planes);
t_mat4				look_at(t_vec3 from, t_vec3 to);
void				frustum_to_local(t_camera *camera, struct s_renderable *r);


#endif
