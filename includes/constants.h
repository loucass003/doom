/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 21:05:41 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/12 12:00:47 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_H
# define CONSTANTS_H

# define W_HEIGHT 640.
# define W_WIDTH 1280.0
# define S_HEIGHT 640.
# define S_WIDTH 1280.0

# define S_HEIGHT_2 (S_HEIGHT / 2.)
# define S_WIDTH_2 (S_WIDTH / 2.)
# define S_SIZE (S_WIDTH * S_HEIGHT)
# define ASPECT_INV (1. / (S_WIDTH / S_HEIGHT))

# define FONT_VERAMONO "assets/fonts/VeraMono.ttf"

# define FOV (60. * (M_PI / 180.))
# define NEAR (0.5)
# define NEAR_CLIP (0.5)
# define FAR_CULL (500)
# define FAR (500)

# define GUI_COUNT (10)
# define GUI_INGAME (0)
# define GUI_MAIN_MENU (1)
# define GUI_EDITOR (2)
# define GUI_GAMEOVER (3)
# define GUI_RESSOURCES (4)
# define GUI_EDITOR_SETTINGS (5)
# define GUI_LEVEL (6)
# define GUI_ESC (7)
# define GUI_LEADERBOARD (8)
# define GUI_MESSAGE (9)

# define AMBIANT_LIGHT (50)

# define MAX_SOUNDS (20)
# define CHAR_SHOOTING (MAX_SOUNDS - 1)
# define ITEM_PICK (CHAR_SHOOTING - 1)
# define CHAR_FOOTSTEP (ITEM_PICK - 1)

# define MAX_OPEN_SOUNDS (CHAR_FOOTSTEP)

#endif
