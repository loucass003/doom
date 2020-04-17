/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 21:05:41 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/16 21:06:17 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_H
# define CONSTANTS_H

# define W_HEIGHT 640.
# define W_WIDTH 1280.0
# define S_HEIGHT 640.
# define S_WIDTH 1280.0

# define S_HEIGHT_2 320.
# define S_WIDTH_2 640.
# define S_SIZE 819200
# define ASPECT_INV 0.5

# define FONT_VERAMONO "assets/fonts/VeraMono.ttf"

# define FOV 1.047197551
# define NEAR 0.5
# define NEAR_CLIP 0.5
# define FAR_CULL 500
# define FAR 500

# define GUI_COUNT 10
# define GUI_INGAME 0
# define GUI_MAIN_MENU 1
# define GUI_EDITOR 2
# define GUI_GAMEOVER 3
# define GUI_RESSOURCES 4
# define GUI_EDITOR_SETTINGS 5
# define GUI_LEVEL 6
# define GUI_ESC 7
# define GUI_LEADERBOARD 8
# define GUI_MESSAGE 9

# define AMBIANT_LIGHT 50

# define MAX_SOUNDS 20
# define CHAR_SHOOTING 19
# define ITEM_PICK 18
# define CHAR_FOOTSTEP 17
# define CHAR_TELEPORT 16

# define MAX_OPEN_SOUNDS CHAR_TELEPORT

#endif
