/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ellipsoid.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 16:31:50 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/05 18:51:54 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collision.h"


t_collision		ellipsoid_hit_triangle(t_collide_ellipsoid *ellipsoid, t_collide_triangle *tri)
{
	t_plane		tri_plane;
	t_vec3		points[3];
	int			i;

 	i = -1;
	while (++i < 3)
		points[i] = ft_vec3_div(tri->points[i], ellipsoid->radius);
	t_plane p0 = plane_new(points[0], ft_vec3_cross(ft_vec3_norm(ft_vec3_sub(points[1], points[0])), tri->normal));
	if (distance_to_plane(p0, ellipsoid->origin) >= 0)
		return (t_collision){ .collide = FALSE, .dist = -1 };
	t_plane p1 = plane_new(points[1], ft_vec3_cross(ft_vec3_norm(ft_vec3_sub(points[2], points[1])), tri->normal));
	if (distance_to_plane(p1, ellipsoid->origin) >= 0)
		return (t_collision){ .collide = FALSE, .dist = -1 };
	t_plane p2 = plane_new(points[2], ft_vec3_cross(ft_vec3_norm(ft_vec3_sub(points[0], points[2])), tri->normal));
	if (distance_to_plane(p2, ellipsoid->origin) >= 0)
		return (t_collision){ .collide = FALSE, .dist = -1 };
	tri_plane = plane_new(points[0], tri->normal);
	float dist = distance_to_plane(tri_plane, ellipsoid->origin);
	if (dist <= -1)
	{
		return (t_collision){ .collide = TRUE, .dist = -1 };
	}
	return (t_collision){ .collide = FALSE, .dist = -1 };
}

// t_bool Triangle_CheckPoint(t_vec3 point, t_collide_triangle triangle ) {
//     // fast barycentric coordinates method 
//     t_vec3 vp = ft_vec3_sub(point, triangle->a );

//     float dot02 = Vec3_Dot( triangle->ca, vp);
//     float dot12 = Vec3_Dot( triangle->ba, vp);

//     float u = (triangle->baDotba * dot02 - triangle->caDotba * dot12) * triangle->invDenom;
//     float v = (triangle->caDotca * dot12 - triangle->caDotba * dot02) * triangle->invDenom;

//     return (u >= 0.0f) && (v >= 0.0f) && (u + v < 1.0f);
// }

// t_collision		ellipsoid_hit_triangle(t_collide_ellipsoid ellipsoid, t_collide_triangle tri)
// {
// 	t_vec3		points[3];
// 	int			i;


// 	while (++i < 3)
// 		tri.points[i] = ft_vec3_div(tri.points[i], ellipsoid.radius);
// 	 // build plane from triangle 
//     t_plane plane = triangle_to_plane(points[0], points[1], points[2]);

//     // find distance from center of the sphere to the plane 
//     float distance = distance_to_plane(plane, ellipsoid.origin);

//     // check distance 
//     if( distance <= 1 ) 
// 	{
//         // project center of the sphere to the plane 
//         // plane's normal vector is normalized! 
//         t_vec3 intersectionPoint = ft_vec3_sub(ellipsoid.origin, ft_vec3_mul_s( plane.normal, distance ));

//         // check, that projected point lies in triangle 
//         if( Triangle_CheckPoint( intersectionPoint, tri ) ) {
//             return (t_collision){ .collide = TRUE, .dist = -1 };
//         } else {       
//             //check each triangle edge intersection with the sphere                 
//             if( Intersection_EdgeSphere( &tri->abRay, sphere, intersectionPoint )) {
//                 return (t_collision){ .collide = TRUE, .dist = -1 };
//             }
//             if( Intersection_EdgeSphere( &tri->bcRay, sphere, intersectionPoint )) {
//                 return (t_collision){ .collide = TRUE, .dist = -1 };
//             }
//             if(	Intersection_EdgeSphere( &tri->caRay, sphere, intersectionPoint )) {
//                 return (t_collision){ .collide = TRUE, .dist = -1 };
//             }
//         }
//     }
//     // no intersection 
//     return (t_collision){ .collide = FALSE, .dist = -1 };
// }
