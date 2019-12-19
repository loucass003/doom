/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/14 17:20:03 by llelievr          #+#    #+#             */
/*   Updated: 2019/12/19 18:56:59 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "collision.h"
#include <math.h>

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

t_collision 			aabb_hit_aabb(t_collide_aabb *b, t_collide_aabb *a)
{
	// if (a->min.x < b->min.x)
	// {
	// 	t_collide_aabb tmp = *a;
	// 	*a = *b;
	// 	*b = tmp;
	// }

 	if(a->min.x > b->max.x)
		return (t_collision){ .collide = FALSE, .dist = -1 };
	if(a->min.y > b->max.y)
		return (t_collision){ .collide = FALSE, .dist = -1 };
	if(a->min.z > b->max.z)
		return (t_collision){ .collide = FALSE, .dist = -1 };
	if(a->max.x < b->min.x)
		return (t_collision){ .collide = FALSE, .dist = -1 };
	if(a->max.y < b->min.y)
		return (t_collision){ .collide = FALSE, .dist = -1 };
	if(a->max.z < b->min.z)
		return (t_collision){ .collide = FALSE, .dist = -1 };
    return ((t_collision) {
		.collide = TRUE,
		.who = (t_collidable){
			.type = COLLIDE_AABB,
			.data = { .aabb = *a }
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



   /* Bullet 1: */

   /*  first test overlap in the {x,y,z}-directions */

   /*  find min, max of the triangle each direction, and test for overlap in */

   /*  that direction -- this is equivalent to testing a minimal AABB around */

   /*  the triangle against the AABB */



   /* test in X-direction */

   FINDMINMAX(v0[X],v1[X],v2[X],min,max);

   if(min>boxhalfsize[X] || max<-boxhalfsize[X]) return 0;



   /* test in Y-direction */

   FINDMINMAX(v0[Y],v1[Y],v2[Y],min,max);

   if(min>boxhalfsize[Y] || max<-boxhalfsize[Y]) return 0;



   /* test in Z-direction */

   FINDMINMAX(v0[Z],v1[Z],v2[Z],min,max);

   if(min>boxhalfsize[Z] || max<-boxhalfsize[Z]) return 0;



   /* Bullet 2: */

   /*  test if the box intersects the plane of the triangle */

   /*  compute plane equation of triangle: normal*x+d=0 */

   CROSS(normal,e0,e1);

   // -NJMP- (line removed here)

   if(!planeBoxOverlap(normal,v0,boxhalfsize)) return 0;	// -NJMP-



   return 1;   /* box and triangle overlaps */

}


t_collision		triangle_hit_aabb(t_collide_triangle *triangle,  t_collide_aabb *aabb)
{

	t_vec3 half = ft_vec3_mul_s(ft_vec3_sub(aabb->max, aabb->min), 0.5);
	half.x = fabs(half.x);
	half.y = fabs(half.y);
	half.z = fabs(half.z);
	t_vec3 center = ft_vec3_add(aabb->min, half);

	if (triBoxOverlap((float [3]){ center.x, center.y, center.z }, (float [3]){ half.x * 1.01, half.y * 1.01, half.z * 1.01 }, (float [3][3]) {
		{ triangle->points[0].x, triangle->points[0].y, triangle->points[0].z },
		{ triangle->points[1].x, triangle->points[1].y, triangle->points[1].z },
		{ triangle->points[2].x, triangle->points[2].y, triangle->points[2].z }
	}))
	return (t_collision){ .collide = TRUE, .dist = 1 };
	else return (t_collision){ .collide = FALSE, .dist = 1 };
}
