/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 01:15:48 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/30 16:13:20 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLLISION_H
# define COLLISION_H

# include <libft.h>

typedef struct s_polygon	t_polygon;

typedef struct		s_ray
{
	t_vec3			origin;
	t_vec3			direction;
}					t_ray;

typedef enum		e_collidable_type
{
	COLLIDE_TRIANGLE = 1,
	COLLIDE_AABB = 0
}					t_collidable_type;

typedef	struct		s_collide_aabb
{
	t_vec3			center;
	t_vec3			halfsize;
}					t_collide_aabb;

typedef struct		s_collide_triangle
{
	t_vec3			points[3];
	t_polygon		*polygon;
	int				triangle;
}					t_collide_triangle;

typedef union 		u_collidable_data
{
	t_collide_aabb		aabb;
	t_collide_triangle	triangle;
}					t_collidable_data;

typedef struct		s_collidable
{
	t_collidable_type	type;
	t_collidable_data	data;
}					t_collidable;

typedef struct		s_collision
{
	t_bool			collide;
	float			dist;
	t_vec3			normal;
	t_vec2			uv;
	t_collidable	who;
}					t_collision;

typedef struct		s_obb_box
{
	t_vec3			pos;
	t_vec3			x_axis;
	t_vec3			y_axis;
	t_vec3			z_axis;
	t_vec3			half_size;
}					t_obb_box;


t_collision			ray_hit_collidable(t_ray *ray, t_collidable *collidable);
t_collision			ray_hit_aabb(t_ray *ray, t_collide_aabb *collidable);
t_collision			ray_hit_triangle(t_ray *ray, t_collide_triangle *collidable);
t_vec3				vec3_rotate(t_vec3 v, t_vec3 rot);
t_collision			triangle_hit_aabb(t_collide_triangle *triangle,  t_collide_aabb *aabb);
t_bool				get_obb_collision(t_obb_box a, t_obb_box b);

#endif
