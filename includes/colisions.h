/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colisions.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/11 16:48:44 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/11 17:02:17 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLISIONS_H
# define COLISIONS_H
# include "doom.h"

typedef struct		s_obb_box
{

}					t_obb_box;

typedef struct		s_aabb_box
{
	t_vec3			min;
	t_vec3			max;
}					t_aabb_box;

#endif
