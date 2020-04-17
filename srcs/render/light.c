/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/17 23:22:00 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/17 23:22:31 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "light.h"
#include "doom.h"

float	get_light_intensity(t_face_data *fd, t_light *light,
	t_vec3 normal, t_vec4 point)
{
	t_vec3	dir;
	float	intensity;

	dir = ft_vec3_norm(ft_vec3_sub(vec4_to_3(point), light->position));
	intensity = ft_vec3_dot(dir, ft_vec3_inv(normal));
	if (light->type == LIGHT_SPOT)
		intensity = 1.0 / ((1.0 - ((ft_vec3_dot(dir, light->dir) + 1.0)
			/ 2.0)) * 10.0) * 0.2;
	return ((intensity) * (15 / ft_vec3_len(ft_vec3_sub(vec4_to_3(point),
		light->position))) * light->intensity);
}

float	get_lights_intensity(t_face_data *fd, t_vec3 normal, t_vec4 point,
	t_face *face)
{
	uint8_t	ambiant;
	int		i;
	float	sum;

	if (!fd->ctx->doom->lights || (fd->r && (fd->r->no_light)))
		return (255);
	ambiant = AMBIANT_LIGHT;
	if (fd->r->of.type == RENDERABLE_MAP && face->room_index != -1)
		ambiant = fd->ctx->doom->editor.
			rooms->values[face->room_index].ambiant_light;
	i = -1;
	sum = 0;
	while (++i < fd->ctx->doom->lights->len)
	{
		sum += get_light_intensity(fd, fd->ctx->doom->lights->values + i,
			normal, point);
	}
	return (clamp(ambiant, 255, sum));
}

void	prepare_points_light(float it[3], t_face *face, t_face_data *fd)
{
	it[0] = get_lights_intensity(fd,
		fd->r->pp_normals[face->normals_index[0] - 1],
		fd->r->pp_vertices[face->vertices_index[0] - 1], face);
	it[1] = get_lights_intensity(fd,
		fd->r->pp_normals[face->normals_index[1] - 1],
		fd->r->pp_vertices[face->vertices_index[1] - 1], face);
	it[2] = get_lights_intensity(fd,
		fd->r->pp_normals[face->normals_index[2] - 1],
		fd->r->pp_vertices[face->vertices_index[2] - 1], face);
}
