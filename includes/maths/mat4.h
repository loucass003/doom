#ifndef MAT4_H
# define MAT4_H

#include <libft.h>

t_bool 				mat4_inverse(t_mat4 a, t_mat4 *inverse);
t_vec3				rotation_matrix_to_euler(t_mat4 m);

#endif
