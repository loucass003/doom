/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ellipsoid.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 22:02:47 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/09 22:41:11 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ELLIPSOID_H
# define ELLIPSOID_H

# include <libft.h>

typedef struct	s_ellipsoid
{
	t_vec3	radius;
}				t_ellipsoid;

t_bool	create_ellipsoid(t_renderable *r, int color, t_vec2 res, t_vec3 radius);

#endif