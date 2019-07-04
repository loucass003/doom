/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ear_clipping.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 15:51:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/04 13:22:45 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

#define EPSILON (1e-6)

static float	area(t_3dvertices *vertices)
{
/*	float	a;
	int		p;
	int		q;

	a = 0;
	p = vertices->len - 1;
	q = 0;
	while (q < vertices->len)
	{
		a += vertices->vertices[p].x * vertices->vertices[q].y
			- vertices->vertices[q].x * vertices->vertices[p].y;
		p = q++;
	}
	return (a * 0.5);*/
	int n = vertices->len;
	float A = 0.0;
	for (int p = n - 1, q = 0; q < n; p = q++) {
		t_vec3 pval = vertices->vertices[p];
		t_vec3 qval = vertices->vertices[q];
		A += pval.x * qval.y - qval.x * pval.y;
	}
	return (A * 0.5);
}

t_bool	inside_triangle(t_vec3 a, t_vec3 b, t_vec3 c, t_vec3 p)
{
	/*float	i = (c.x - b.x) * (p.y - b.y) - (c.y - b.y) * (p.x - b.x);
	float	j = (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
	float	k = (a.x - c.x) * (p.y - c.y) - (a.y - c.y) * (p.x - c.x);

	return (i >= 0 && j >= 0 && k >= 0);*/

	float i = fabs(ft_vec3_dot(ft_vec3_sub(c, b), ft_vec3_sub(p, b)));
	float j = fabs(ft_vec3_dot(ft_vec3_sub(b, a), ft_vec3_sub(p, a)));
	float k = fabs(ft_vec3_dot(ft_vec3_sub(a, c), ft_vec3_sub(p, c)));

	return (i < 0.999 && j < 0.999 && k < 0.999);
}

static t_bool	snip(t_3dvertices *vertices, int u, int v, int w, int n, int *V)
{
	int p;
	t_vec3 A = vertices->vertices[V[u]];
	t_vec3 B = vertices->vertices[V[v]];
	t_vec3 C = vertices->vertices[V[w]];
	float f = fabs(ft_vec3_dot(ft_vec3_sub(B, A), ft_vec3_sub(C, A)));
	if (f < 0.999)
		return FALSE;
	/*float t = ((B.x - A.x) * (C.y - A.y)) - ((B.y - A.y) * (C.x - A.x));
	if (EPSILON > t)
		return (FALSE);*/
	for (p = 0; p < n; p++) {
		if ((p == u) || (p == v) || (p == w))
			continue;
		t_vec3 P = vertices->vertices[V[p]];
		if (inside_triangle(A, B, C, P))
			return FALSE;
	}
	return TRUE;
}

void disp(t_mat4 a) 
{ 
	for (int i=0; i<4; i++) 
	{ 
		for (int j=0; j<4; j++) 
			printf("%f ", a.a[i][j]); 
		printf("\n"); 
	} 
} 


t_bool	triangulate_polygon(t_polygon *polygon)
{
	t_vec3 no = get_polygon_normal(polygon);
	for (int i = 0; i < polygon->vertices->len; i++)
		printf("point %f %f %f\n", polygon->vertices->vertices[i].x, polygon->vertices->vertices[i].y, polygon->vertices->vertices[i].z);
	printf("normal %f %f %f\n", no.x, no.y, no.z);

	const t_vec3 up = (t_vec3){0, 0, 1};
	t_vec3 U = ft_vec3_cross(no, up);
	if (ft_vec3_len(U) == 0)
		U = (t_vec3){0, 1, 0};
	printf("u %f %f %f len %f\n", U.x, U.y, U.z, ft_vec3_len(U));
	t_vec3 W = ft_vec3_cross(U, no);
	printf("w %f %f %f\n", W.x, W.y, W.z);

	t_mat4 p = (t_mat4)((t_mat4_data){
		U.x, W.x, no.x, 0,
		U.y, W.y, no.y, 0,
		U.z, W.z, no.z, 0,
		0,   0,   0,    1
	});

	disp(p);
	printf("\n");
	t_mat4 inv;
	if (!mat4_inverse(p, &inv))
		printf("ERROR\n");
	printf("\n");
	disp(inv);
	
	t_vec3 p_u = ft_mat4_mulv(inv, U);
	printf("P-1u %f %f %f\n", p_u.x, p_u.y, p_u.z);
	t_vec3 p_w = ft_mat4_mulv(inv, W);
	printf("P-1w %f %f %f\n", p_w.x, p_w.y, p_w.z);
	t_vec3 p_n = ft_mat4_mulv(inv, no);
	printf("P-1n %f %f %f\n", p_n.x, p_n.y, p_n.z);

	for (int i = 0; i < polygon->vertices->len; i++)
	{
		t_vec3 point = ft_mat4_mulv(inv, polygon->vertices->vertices[i]);
		printf("new point %f %f %f\n", point.x, point.y, point.z);
	}

	printf("\n");
	int n = polygon->vertices->len;
	if (n < 3 || !(polygon->indices = create_ints_array(polygon->vertices->len * 3)))
		return FALSE;
	int V[n];
	/*if (area(polygon->vertices) > 0) {
		for (int v = 0; v < n; v++)
			V[v] = v;
	}
	else {
		for (int v = 0; v < n; v++)
			V[v] = (n - 1) - v;
	}*/
	t_bool b = ft_vec3_dot(get_polygon_normal(polygon), get_polygon_normal(polygon)) > 0; //DAFUCK CA MARCHE xD
	if (b) {
		for (int v = 0; v < n; v++)
			V[v] = v;
	}
	else {
		for (int v = 0; v < n; v++)
			V[v] = (n - 1) - v;
	}
	int nv = n;
	int count = 2 * nv;
	for (int v = nv - 1; nv > 2; ) {
		if ((count--) <= 0)
			return TRUE;

		int u = v;
		if (nv <= u)
			u = 0;
		v = u + 1;
		if (nv <= v)
			v = 0;
		int w = v + 1;
		if (nv <= w)
			w = 0;

		if (snip(polygon->vertices, u, v, w, nv, V)) {
			int a, b, c, s, t;
			a = V[u];
			b = V[v];
			c = V[w];
			append_ints_array(&polygon->indices, a);
			append_ints_array(&polygon->indices, b);
			append_ints_array(&polygon->indices, c);
			for (s = v, t = v + 1; t < nv; s++, t++)
				V[s] = V[t];
			nv--;
			count = 2 * nv;
		}
	}
	return (TRUE);
}
