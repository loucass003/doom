/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quat.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 14:15:52 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/07 15:09:32 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUAT_H
# define QUAT_H

typedef struct	s_quat
{
	float		x;
	float		y;
	float 		z;
	float		w;
}				t_quat;

t_quat		quat_identity();
t_quat		quat_normalize(t_quat q);
t_quat		quat_rotation_to(t_vec3 from, t_vec3 to);
t_mat4		quat_to_matrix(t_quat q);

#endif