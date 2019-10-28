/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rle-ru <rle-ru@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 21:05:41 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/28 17:30:56 by rle-ru           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_H
# define CONSTANTS_H

// # define W_HEIGHT 900.0
// # define W_WIDTH 1600.0
// # define S_HEIGHT (619.0)
// # define S_WIDTH (1100.0)
# define W_HEIGHT 640.
# define W_WIDTH 1280.0
# define S_HEIGHT 640.
# define S_WIDTH 1280.0

// MENU TEXTURES
# define MT_Y 150
# define MT_X (S_WIDTH - 170)
# define MT_W 40
# define MT_H 40

// ICONE TEXTURES

// # define IT_Y 30
// # define IT_X (W_WIDTH - 60)
// # define IT_W 50
// # define IT_H 50

# define S_HEIGHT_2 (S_HEIGHT / 2.)
# define S_WIDTH_2 (S_WIDTH / 2.)
# define S_SIZE (S_WIDTH * S_HEIGHT)
# define ASPECT_INV (1. / (S_HEIGHT / S_WIDTH))

# define FONT_VERAMONO "assets/fonts/VeraMono.ttf"

# define FOV (80. * (M_PI / 180.))
# define NEAR (0.5)
# define NEAR_CLIP (0.5)
# define FAR_CULL (500)
# define FAR (500)

# define GUI_COUNT (5)
# define GUI_INGAME (0)
# define GUI_MAIN_MENU (1)
# define GUI_EDITOR (2)
# define GUI_GAMEOVER (3)
# define GUI_RESSOURCES (4)

# define AMBIANT_LIGHT (100)

# define MAX_SOUNDS (20)

#endif
