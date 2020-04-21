/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 15:37:12 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/16 21:08:18 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONVERT_H
# define CONVERT_H

# include <stdint.h>
# define LOG2 0.30102999566
# define LOG2_8 0.33333333333

typedef struct	s_int_str {
	char	str[(uint8_t)(sizeof(uintmax_t) * 8 * LOG2) + 2];
	uint8_t	len;
}				t_int_str;

typedef struct	s_int_str16 {
	char	str[sizeof(uintmax_t) * 2 + 2];
	uint8_t	len;
}				t_int_str16;

typedef struct	s_int_str8 {
	char	str[(uint8_t)(sizeof(uintmax_t) * 8 * LOG2_8) + 2];
	uint8_t	len;
}				t_int_str8;

t_int_str		ft_int_to_str(intmax_t nb);
t_int_str		ft_uint_to_str(uintmax_t nb);
t_int_str16		ft_uint_to_str16(uintmax_t nb, uint8_t maj);
t_int_str8		ft_uint_to_str8(uintmax_t nb);
#endif
