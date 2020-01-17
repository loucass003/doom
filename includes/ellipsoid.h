/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ellipsoid.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 22:02:47 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/17 17:39:53 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ELLIPSOID_H
# define ELLIPSOID_H

# include <libft.h>
# include "render.h"

typedef struct	s_ellipsoid
{
	t_vec3	radius;
}				t_ellipsoid;

t_bool	create_ellipsoid(struct s_doom *doom, t_renderable *r, t_vec2 res, t_vec3 radius);

#endif
