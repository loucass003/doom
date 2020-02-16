/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat4.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 16:55:25 by lloncham          #+#    #+#             */
/*   Updated: 2020/02/16 16:57:18 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAT4_H
# define MAT4_H

# include <libft.h>

t_bool				mat4_inverse(t_mat4 a, t_mat4 *inverse);
t_vec3				rotation_matrix_to_euler(t_mat4 m);
t_mat4				mat4_transpose(t_mat4 a);

#endif
