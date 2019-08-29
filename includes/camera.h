/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 17:40:59 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/29 17:41:20 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

#include "libft.h"

typedef struct		s_camera
{
	t_vec3			pos;
	t_vec3			rotation;
	t_mat4			matrix;
	t_mat4			projection;
}					t_camera;

void				camera_update_maxtrix(t_camera *camera);
t_mat4				projection_matrix();

#endif
