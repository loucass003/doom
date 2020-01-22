/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 02:48:07 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/22 13:41:16 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_UTILS_H
# define FILE_UTILS_H
# include "editor.h"

char				*path_join(char *dir, char *file);
void				find_files(t_files **list, int *files_count, char *s, char *s2);


#endif