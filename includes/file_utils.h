/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 02:48:07 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/16 14:41:32 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_UTILS_H
# define FILE_UTILS_H
# include "editor.h"

char				*path_join(char *dir, char *file);
ssize_t				get_file_size(int fd);
char				*get_file_content(char *file, int *content_len);

#endif
