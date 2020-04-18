/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 01:15:48 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/18 23:27:30 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLLISION_H
# define COLLISION_H

# include <libft.h>
# include "renderable_of.h"
# include "maths/vec4.h"
# include "maths/plane.h"

typedef struct					s_ray
{
	t_vec3						origin;
	t_vec3						direction;
	struct s_ray				*to_local;
	struct s_doom				*doom;
}								t_ray;

typedef enum					e_collidable_type
{
	COLLIDE_ELLIPSOID = 2,
	COLLIDE_TRIANGLE = 1,
	COLLIDE_AABB = 0
}								t_collidable_type;

typedef	struct					s_collide_aabb
{
	t_vec3						min;
	t_vec3						max;
}								t_collide_aabb;

typedef	struct					s_collide_ellipsoid
{
	t_vec3						origin;
	t_vec3						radius;
}								t_collide_ellipsoid;

typedef struct					s_collide_triangle
{
	t_vec3						points[3];
	t_vec2						uv[3];
	t_vec3						normal;
	t_vec3						v0v1;
	t_vec3						v0v2;
	int							face;
	t_bool						double_sided;
}								t_collide_triangle;

typedef	union					u_collidable_data
{
	t_collide_aabb				aabb;
	t_collide_triangle			triangle;
	t_collide_ellipsoid			ellipsoid;
}								t_collidable_data;

typedef struct					s_collidable
{
	t_renderable_of				of;
	t_collidable_type			type;
	t_collidable_data			data;
}								t_collidable;

typedef struct					s_collision
{
	struct s_renderable			*renderable;
	t_bool						collide;
	float						dist;
	t_vec2						uv;
	t_collidable				who;
	t_ray						ray;
	t_vec3						point;
}								t_collision;

typedef struct					s_obb_box
{
	t_vec3						pos;
	t_vec3						x_axis;
	t_vec3						y_axis;
	t_vec3						z_axis;
	t_vec3						half_size;
}								t_obb_box;

typedef struct					s_physics_data
{
	t_vec3						r3_posision;
	t_vec3						r3_velocity;
	t_vec3						e_radius;
	t_vec3						e_position;
	t_vec3						e_velocity;
	t_vec3						e_norm_velocity;
	t_vec3						e_base_point;
	t_vec3						intersect_point;
	t_bool						found_colision;
	float						distance;
	double						t;
	t_vec3						a;
	t_vec3						b;
	t_vec3						c;
	t_plane						plane;
	t_bool						grounded;
	float						dt;
	size_t						depth;
	struct s_renderable			*r;
	struct s_doom				*doom;
}								t_physics_data;

typedef struct					s_physics_fields
{
	float						t0;
	float						t1;
	float						t;
	t_bool						found_collision;
	t_bool						in_plane;
	t_vec3						colision_point;
	float						a[3];
	float						velocity_sq_len;
	t_plane						plane;
}								t_physics_fields;

typedef struct					s_ray_test_data
{
	struct s_renderable			*r;
	t_ray						*ray;
	struct s_renderables		*renderables;
	int							i;
	t_collision					min;
	struct s_doom				*doom;
}								t_ray_test_data;

t_vec3							point_to_local(t_vec3 point, t_vec3 position,
									t_vec3 rotation, t_vec3 scale);
t_ray							to_local_ray(t_ray ray, t_vec3 position,
									t_vec3 rotation, t_vec3 scale);
t_collision						to_world_collision(t_ray original_ray,
									t_collision local_collision,
									t_vec3 prs[3]);
t_collision						ray_hit_collidable(t_ray *ray,
									t_collidable *collidable);
t_collision						ray_hit_aabb(t_ray *ray,
									t_collide_aabb *collidable);
t_collision						ray_hit_triangle(t_ray *ray,
									t_collide_triangle *collidable);
t_collision						aabb_hit_aabb(t_collide_aabb *a,
									t_collide_aabb *b);
t_vec3							vec3_rotate(t_vec3 v, t_vec3 rot);
t_collision						triangle_hit_aabb(t_collide_triangle *triangle,
									t_collide_aabb *aabb);
t_bool							get_obb_collision(t_obb_box a, t_obb_box b);
t_bool							ray_skip_renderable(struct s_renderable *r);
t_physics_data					*check_triangle(struct s_renderable *r,
									t_physics_data *packet, t_vec3 p[3]);
void							lowest_collision(t_physics_fields *fields,
									t_physics_data *packet, t_vec3 p);
void							edge_collision(t_physics_fields *f,
									t_physics_data *pa, t_vec3 p[2]);
t_bool							point_in_triangle_cross(const t_vec3 u,
									const t_vec3 v, const t_vec3 w,
									const t_vec3 vw);
t_bool							point_in_triangle(t_vec3 point, t_vec3 p1,
									t_vec3 p2, t_vec3 p3);
t_bool							lowest_root(t_vec3 v, float max, float *root);
float							clamp(float min, float max, float v);
void							swapf(float *a, float *b);
t_bool							axis_test(t_vec4 params, t_vec4 v, t_vec2 v3,
									t_vec2 *range);
t_bool							axis_test2(t_vec4 params, t_vec4 v, t_vec2 v3,
									t_vec2 *range);
void							findmax(t_vec3 v, t_vec2 *range);

#endif
