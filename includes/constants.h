/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 21:05:41 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/06 09:01:13 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_H
# define CONSTANTS_H

# define W_HEIGHT 600.0
# define W_WIDTH 1000.0
# define S_HEIGHT (600.0)
# define S_WIDTH (1000.0)
# define S_HEIGHT_2 (S_HEIGHT / 2.)
# define S_WIDTH_2 (S_WIDTH / 2.)
# define S_SIZE (S_WIDTH * S_HEIGHT)
# define ASPECT_INV (1. / (S_HEIGHT / S_WIDTH))

# define FONT_HELVETICA "assets/fonts/helvetica.ttf"

# define FOV (60 * (M_PI / 180))
# define NEAR (1)
# define NEAR_CLIP (0.1)
# define FAR (100)

# define GUI_COUNT (3)
# define GUI_INGAME (0)
# define GUI_MAIN_MENU (1)
# define GUI_EDITOR (2)

#endif
