/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat4_inverse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/04 02:12:21 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/04 13:01:12 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

static void		cofactor(float A[4][4], float temp[4][4], int v[3]) 
{
	int		i;
	int		j; 
	int		row;
	int		col;

	i = 0;
	j = 0;
	row = -1;
	while (++row < v[2])
	{
		col = -1;
		while (++col < v[2])
		{
			if (row != v[0] && col != v[1]) 
			{
				temp[i][j++] = A[row][col];
				if (j == v[2] - 1)
				{
					j = 0;
					i++;
				}
			}
		}
	}
}

static float	determinant(float a[4][4], int n) 
{
	float	temp[4][4];
	float	d;
	int 	sign;
	int		f;

	d = 0;
	sign = 1;
	if (n == 1)
		return a[0][0]; 
	f = -1;
	while (++f < n)
	{
		cofactor(a, temp, (int[3]){0, f, n});
		d += (float)sign * a[0][f] * determinant(temp, n - 1); 
		sign = -sign;
	}
	return (d); 
} 

static void		adjoint(float a[4][4], float adj[4][4]) 
{
	float	temp[4][4];
	int		i;
	int		j;
	int		sign;

	sign = 1;
	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			cofactor(a, temp, (int[3]){i, j, 4});
			sign = ((i + j) % 2 == 0) ? 1 : -1;
			adj[j][i] = (float)sign * determinant(temp, 4 - 1);
		}
	}
} 

t_bool		mat4_inverse(t_mat4 a, t_mat4 *inverse)
{
	const float		det = determinant(a.a, 4); 
	float			adj[4][4];
	int				i;
	int				j;
	
	if (det == 0) 
		return (FALSE);
	adjoint(a.a, adj);
	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
			inverse->a[i][j] = adj[i][j] / (float)det; 
	}
	return (TRUE);
}