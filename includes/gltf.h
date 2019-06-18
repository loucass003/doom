/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gltf.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/18 11:49:41 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/18 17:52:51 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLTF_H
# define GLTF_H
# include "doom.h"

typedef	struct	s_gltf_node
{
	double		*childrens;
	size_t		childrens_count;
	t_mat4		matrix;
	t_bool		matrix_set;
	t_vec3		translation;
	t_vec4		rotation;
	t_vec3		scale;
}				t_gltf_node;

typedef	struct	s_scene
{
	double		*nodes;
	size_t		nodes_count;
}				t_scene;

typedef struct	s_gltf
{
	t_scene		*scenes;

}				t_gltf;

#endif
