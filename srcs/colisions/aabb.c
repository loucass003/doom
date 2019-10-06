/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/14 17:20:03 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/06 03:05:25 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "collision.h"
#include <math.h>

void	find_max(float x0, float x1, float x2, t_vec2 *range)
{
	range->x = x0;
	range->y = x0;
	if (x1 < range->x)
		range->x = x1;
	if (x1 > range->y)
		range->y = x1;
	if (x2 < range->x)
		range->x = x2;
	if (x2 < range->y)
		range->y = x2;
}


t_bool plane_hit_aabb(t_vec3 n, t_vec3 v0, t_vec3 halfsize)	// -NJMP-
{
	int q;
	t_vec3_u min;
	t_vec3_u max;
	t_vec3_u vu;
	t_vec3_u nu;
	t_vec3_u hsu;
	float v;

	vu.v = v0;
	nu.v = n;
	for(q = 0; q < 3; q++)
	{
		v = vu.a[q];					// -NJMP-
		if(nu.a[q] > 0.0f)
		{
			min.a[q] = -hsu.a[q] - v;	// -NJMP-
			max.a[q] = hsu.a[q] - v;	// -NJMP-
		}
		else
		{
			min.a[q] = hsu.a[q] - v;	// -NJMP-
			max.a[q] = -hsu.a[q] - v;	// -NJMP-
		}
	}

  if(ft_vec3_dot(n, min.v) > 0.0f) 
  	return FALSE;
  if(ft_vec3_dot(n, max.v) >= 0.0f)
  	return TRUE;
  return FALSE;
}

t_bool	axis_test_x01(t_vec2 e[2], t_vec3 halfsize,t_vec2 *range, t_vec3 v[3])
{
	const float p0 = -e[0].x * v[0].y - e[0].y * v[0].z;
	const float p2 = -e[0].x * v[2].y - e[0].y * v[2].z;
	float rad;

	range->x = p0 < p2 ? p0 : p2;
	range->y = p0 < p2 ? p2 : p0;
	rad = e[1].x * halfsize.y + e[0].y * halfsize.z;
	if (range->x > rad || range->y < -rad)
		return (FALSE);
	return (TRUE);
}

t_bool	axis_test_x2(t_vec2 e[2], t_vec3 halfsize,t_vec2 *range, t_vec3 v[3])
{
	const float p0 = -e[0].x * v[0].y - e[0].y * v[0].z;
	const float p1 = -e[0].x * v[1].y - e[0].y * v[1].z;
	float rad;

	range->x = p0 < p1 ? p0 : p1;
	range->y = p0 < p1 ? p1 : p0;
	rad = e[1].x * halfsize.y + e[0].y * halfsize.z;
	if (range->x > rad || range->y < -rad)
		return (FALSE);
	return (TRUE);
}

t_bool	axis_test_y02(t_vec2 e[2], t_vec3 halfsize,t_vec2 *range, t_vec3 v[3])
{
	const float p2 = -e[0].x * v[2].x - e[0].y * v[2].z;
	const float p0 = -e[0].x * v[0].x - e[0].y * v[0].z;
	float rad;

	range->x = p0 < p2 ? p0 : p2;
	range->y = p0 < p2 ? p2 : p0;
	rad = e[1].x * halfsize.x + e[0].y * halfsize.z;
	if (range->x > rad || range->y < -rad)
		return (FALSE);
	return (TRUE);
}

t_bool	axis_test_y1(t_vec2 e[2], t_vec3 halfsize,t_vec2 *range, t_vec3 v[3])
{
	const float p1 = -e[0].x * v[1].x - e[0].y * v[1].z;
	const float p0 = -e[0].x * v[0].x - e[0].y * v[0].z;
	float rad;

	range->x = p0 < p1 ? p0 : p1;
	range->y = p0 < p1 ? p1 : p0;
	rad = e[1].x * halfsize.x + e[0].y * halfsize.z;
	if (range->x > rad || range->y < -rad)
		return (FALSE);
	return (TRUE);
}

t_bool	axis_test_z12(t_vec2 e[2], t_vec3 halfsize,t_vec2 *range, t_vec3 v[3])
{
	const float p1 = e[0].x * v[1].x - e[0].y * v[1].y;
	const float p2 = e[0].x * v[2].x - e[0].y * v[2].y;
	float rad;

	range->x = p2 < p1 ? p2 : p1;
	range->y = p2 < p1 ? p1 : p2;
	rad = e[1].x * halfsize.x + e[0].y * halfsize.y;
	if (range->x > rad || range->y < -rad)
		return (FALSE);
	return (TRUE);
}

t_bool	axis_test_z0(t_vec2 e[2], t_vec3 halfsize, t_vec2 *range, t_vec3 v[3])
{
	const float p0 = e[0].x * v[0].x - e[0].y * v[0].y;
	const float p1 = e[0].x * v[1].x - e[0].y * v[1].y;
	float rad;

	range->x = p0 < p1 ? p0 : p1;
	range->y = p0 < p1 ? p1 : p0;
	rad = e[1].x * halfsize.x + e[0].y * halfsize.y;
	if (range->x > rad || range->y < -rad)
		return (FALSE);
	return (TRUE);
}

t_collision		ray_hit_aabb(t_ray *ray, t_collide_aabb *collidable)
{
	t_vec3	dirfrac;

	dirfrac.x = 1.0f / ray->direction.x;
	dirfrac.y = 1.0f / ray->direction.y;
	dirfrac.z = 1.0f / ray->direction.z;
	float t1 = (collidable->min.x - ray->origin.x) * dirfrac.x;
	float t3 = (collidable->min.y - ray->origin.y) * dirfrac.y;
	float t5 = (collidable->min.z - ray->origin.z) * dirfrac.z;
	float t2 = (collidable->max.x - ray->origin.x) * dirfrac.x;
	float t4 = (collidable->max.y - ray->origin.y) * dirfrac.y;
	float t6 = (collidable->max.z - ray->origin.z) * dirfrac.z;

	float tmax = fmin(fmin(fmax(t1, t2), fmax(t3, t4)), fmax(t5, t6));
	if (tmax < 0)
		return (t_collision){ .collide = FALSE, .dist = -1 };
	float tmin = fmax(fmax(fmin(t1, t2), fmin(t3, t4)), fmin(t5, t6));
	if (tmin > tmax)
		return (t_collision){ .collide = FALSE, .dist = -1 };
	return ((t_collision) {
		.collide = TRUE,
		.dist = tmin,
		.who = (t_collidable){
			.type = COLLIDE_AABB,
			.data = { .aabb = *collidable }
		}
	});
}

t_collision 			aabb_hit_aabb(t_collide_aabb *a, t_collide_aabb *b)
{
	if (b->min.x >= a->max.x
		|| b->max.x <= a->min.x
		|| b->min.y >= a->max.y
		|| b->min.y >= a->max.y
		|| b->min.z >= a->max.z
		|| b->min.z >= a->max.z)
		return (t_collision){ .collide = FALSE, .dist = -1 };
    return ((t_collision) {
		.collide = TRUE,
		.who = (t_collidable){
			.type = COLLIDE_AABB,
			.data = { .aabb = *b }
		}
	});
};

#define X 0

#define Y 1

#define Z 2



#define CROSS(dest,v1,v2) \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0]; 



#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])



#define SUB(dest,v1,v2) \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2]; 



#define FINDMINMAX(x0,x1,x2,min,max) \
  min = max = x0;   \
  if(x1<min) min=x1;\
  if(x1>max) max=x1;\
  if(x2<min) min=x2;\
  if(x2>max) max=x2;



int planeBoxOverlap(float normal[3], float vert[3], float maxbox[3])	// -NJMP-

{

  int q;

  float vmin[3],vmax[3],v;

  for(q=X;q<=Z;q++)

  {

    v=vert[q];					// -NJMP-

    if(normal[q]>0.0f)

    {

      vmin[q]=-maxbox[q] - v;	// -NJMP-

      vmax[q]= maxbox[q] - v;	// -NJMP-

    }

    else

    {

      vmin[q]= maxbox[q] - v;	// -NJMP-

      vmax[q]=-maxbox[q] - v;	// -NJMP-

    }

  }

  if(DOT(normal,vmin)>0.0f) return 0;	// -NJMP-

  if(DOT(normal,vmax)>=0.0f) return 1;	// -NJMP-

  

  return 0;

}





/*======================== X-tests ========================*/

#define AXISTEST_X01(a, b, fa, fb)			   \
	p0 = a*v0[Y] - b*v0[Z];			       	   \
	p2 = a*v2[Y] - b*v2[Z];			       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return 0;



#define AXISTEST_X2(a, b, fa, fb)			   \
	p0 = a*v0[Y] - b*v0[Z];			           \
	p1 = a*v1[Y] - b*v1[Z];			       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return 0;



/*======================== Y-tests ========================*/

#define AXISTEST_Y02(a, b, fa, fb)			   \
	p0 = -a*v0[X] + b*v0[Z];		      	   \
	p2 = -a*v2[X] + b*v2[Z];	       	       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return 0;



#define AXISTEST_Y1(a, b, fa, fb)			   \
	p0 = -a*v0[X] + b*v0[Z];		      	   \
	p1 = -a*v1[X] + b*v1[Z];	     	       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return 0;


/*======================== Z-tests ========================*/



#define AXISTEST_Z12(a, b, fa, fb)			   \
	p1 = a*v1[X] - b*v1[Y];			           \
	p2 = a*v2[X] - b*v2[Y];			       	   \
        if(p2<p1) {min=p2; max=p1;} else {min=p1; max=p2;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
	if(min>rad || max<-rad) return 0;



#define AXISTEST_Z0(a, b, fa, fb)			   \
	p0 = a*v0[X] - b*v0[Y];				   \
	p1 = a*v1[X] - b*v1[Y];			           \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
	if(min>rad || max<-rad) return 0;



int triBoxOverlap(float boxcenter[3],float boxhalfsize[3],float triverts[3][3])

{



  /*    use separating axis theorem to test overlap between triangle and box */

  /*    need to test for overlap in these directions: */

  /*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */

  /*       we do not even need to test these) */

  /*    2) normal of the triangle */

  /*    3) crossproduct(edge from tri, {x,y,z}-directin) */

  /*       this gives 3x3=9 more tests */

   float v0[3],v1[3],v2[3];

//   float axis[3];

   float min,max,p0,p1,p2,rad,fex,fey,fez;		// -NJMP- "d" local variable removed

   float normal[3],e0[3],e1[3],e2[3];



   /* This is the fastest branch on Sun */

   /* move everything so that the boxcenter is in (0,0,0) */

   SUB(v0,triverts[0],boxcenter);

   SUB(v1,triverts[1],boxcenter);

   SUB(v2,triverts[2],boxcenter);



   /* compute triangle edges */

   SUB(e0,v1,v0);      /* tri edge 0 */

   SUB(e1,v2,v1);      /* tri edge 1 */

   SUB(e2,v0,v2);      /* tri edge 2 */



   /* Bullet 3:  */

   /*  test the 9 tests first (this was faster) */

   fex = fabsf(e0[X]);

   fey = fabsf(e0[Y]);

   fez = fabsf(e0[Z]);

   AXISTEST_X01(e0[Z], e0[Y], fez, fey);

   AXISTEST_Y02(e0[Z], e0[X], fez, fex);

   AXISTEST_Z12(e0[Y], e0[X], fey, fex);



   fex = fabsf(e1[X]);

   fey = fabsf(e1[Y]);

   fez = fabsf(e1[Z]);

   AXISTEST_X01(e1[Z], e1[Y], fez, fey);

   AXISTEST_Y02(e1[Z], e1[X], fez, fex);

   AXISTEST_Z0(e1[Y], e1[X], fey, fex);



   fex = fabsf(e2[X]);

   fey = fabsf(e2[Y]);

   fez = fabsf(e2[Z]);

   AXISTEST_X2(e2[Z], e2[Y], fez, fey);

   AXISTEST_Y1(e2[Z], e2[X], fez, fex);

   AXISTEST_Z12(e2[Y], e2[X], fey, fex);


	// printf("--------\n");
   /* Bullet 1: */

   /*  first test overlap in the {x,y,z}-directions */

   /*  find min, max of the triangle each direction, and test for overlap in */

   /*  that direction -- this is equivalent to testing a minimal AABB around */

   /*  the triangle against the AABB */



   /* test in X-direction */
//    printf("min = %f\n", min);

   FINDMINMAX(v0[X],v1[X],v2[X],min,max);

   if(min>boxhalfsize[X] || max<-boxhalfsize[X]) return 0;



   /* test in Y-direction */
	// printf("min = %f\n", min);

   FINDMINMAX(v0[Y],v1[Y],v2[Y],min,max);

   if(min>boxhalfsize[Y] || max<-boxhalfsize[Y]) return 0;



   /* test in Z-direction */

	// printf("min = %f\n", min);

   FINDMINMAX(v0[Z],v1[Z],v2[Z],min,max);

   if(min>boxhalfsize[Z] || max<-boxhalfsize[Z]) return 0;



   /* Bullet 2: */

   /*  test if the box intersects the plane of the triangle */

   /*  compute plane equation of triangle: normal*x+d=0 */

	

   CROSS(normal,e0,e1);

   // -NJMP- (line removed here)
	// printf("min = %f\n", min);

   if(!planeBoxOverlap(normal,v0,boxhalfsize)) return 0;	// -NJMP-



   return 1;   /* box and triangle overlaps */

}

t_collision		triangle_hit_aabb(t_collide_triangle *triangle,  t_collide_aabb *aabb)
{
// 	t_vec2 range;
// 	t_vec3 normal;
// 	t_vec3 v0 = ft_vec3_sub(triangle->points[0], aabb->center);
// 	t_vec3 v1 = ft_vec3_sub(triangle->points[1], aabb->center);
// 	t_vec3 v2 = ft_vec3_sub(triangle->points[2], aabb->center);

// 	t_vec3 e0 = ft_vec3_sub(v1, v0);
// 	t_vec3 e1 = ft_vec3_sub(v2, v1);
// 	t_vec3 e2 = ft_vec3_sub(v0, v2);

// 	t_vec3 fe = (t_vec3){ fabsf(e0.x), fabsf(e0.y), fabsf(e0.z) };
// 	if (!axis_test_x01((t_vec2[2]){{ e0.z, e0.y }, { fe.z, fe.y}}, aabb->halfsize, &range, triangle->points)
// 		|| !axis_test_y02((t_vec2[2]){{ e0.z, e0.x }, { fe.z, fe.x}}, aabb->halfsize, &range, triangle->points)
// 		|| !axis_test_z12((t_vec2[2]){{ e0.y, e0.x }, { fe.y, fe.x}}, aabb->halfsize, &range, triangle->points))
// 		return (t_collision){ .collide = FALSE, .dist = -1 };
// 	fe = (t_vec3){ fabsf(e1.x), fabsf(e1.y), fabsf(e1.z) };
// 	if (!axis_test_x01((t_vec2[2]){{ e1.z, e1.y }, { fe.z, fe.y}}, aabb->halfsize, &range, triangle->points)
// 		|| !axis_test_y02((t_vec2[2]){{ e1.z, e1.x }, { fe.z, fe.x}}, aabb->halfsize, &range, triangle->points)
// 		|| !axis_test_z0((t_vec2[2]){{ e1.y, e1.x }, { fe.y, fe.x}}, aabb->halfsize, &range, triangle->points))
// 		return (t_collision){ .collide = FALSE, .dist = -1 };
// 	fe = (t_vec3){ fabsf(e2.x), fabsf(e2.y), fabsf(e2.z) };
// 	if (!axis_test_x2((t_vec2[2]){{e2.z, e2.y}, {fe.z, fe.y}}, aabb->halfsize, &range, triangle->points)
// 		|| !axis_test_y1((t_vec2[2]){{ e2.z, e2.x }, { fe.z, fe.x}}, aabb->halfsize, &range, triangle->points)
// 		|| !axis_test_z12((t_vec2[2]){{ e2.y, e2.x }, { fe.y, fe.x}}, aabb->halfsize, &range, triangle->points))
// 		return (t_collision){ .collide = FALSE, .dist = -1 };
// 	find_max(v0.x, v1.x, v2.x, &range);
// 	if(range.x > aabb->halfsize.x || range.y < -aabb->halfsize.x) 
// 		return (t_collision){ .collide = FALSE, .dist = -1 };
// 	find_max(v0.y, v1.y, v2.y, &range);
// 	if(range.x > aabb->halfsize.y || range.y < -aabb->halfsize.y)
// 		return (t_collision){ .collide = FALSE, .dist = -1 };
//    	find_max(v0.z, v1.z, v2.z, &range);
//    	if(range.x > aabb->halfsize.z || range.y < -aabb->halfsize.z) 
// 		return (t_collision){ .collide = FALSE, .dist = -1 };
//    	normal = ft_vec3_cross(e0, e1);
// 	if(!plane_hit_aabb(normal, v0, aabb->halfsize)) 
// 		return (t_collision){ .collide = FALSE, .dist = -1 };
//    return (t_collision){ .collide = TRUE, .dist = 1 };
	t_vec3 half = ft_vec3_mul_s(ft_vec3_sub(aabb->max, aabb->min), 0.5);
	half.x = fabs(half.x);
	half.y = fabs(half.y);
	half.z = fabs(half.z);
	t_vec3 center = ft_vec3_add(aabb->min, half);

	if (triBoxOverlap((float [3]){ center.x, center.y, center.z }, (float [3]){ half.x, half.y, half.z }, (float [3][3]) {
		{ triangle->points[0].x, triangle->points[0].y, triangle->points[0].z },
		{ triangle->points[1].x, triangle->points[1].y, triangle->points[1].z },
		{ triangle->points[2].x, triangle->points[2].y, triangle->points[2].z }
	}))
	return (t_collision){ .collide = TRUE, .dist = 1 };
	else return (t_collision){ .collide = FALSE, .dist = 1 };
}
