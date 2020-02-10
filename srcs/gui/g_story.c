/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_story.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 16:08:54 by lloncham          #+#    #+#             */
/*   Updated: 2020/02/10 01:04:21 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "editor.h"
#include "gui.h"

void			g_story_on_enter(t_gui *self, t_doom *doom)
{

}

void			g_story_on_leave(t_gui *self, t_doom *doom)
{
}

void			g_story_on_event(t_gui *self, SDL_Event *event, t_doom *doom)
{
	const SDL_Scancode	key = event->key.keysym.scancode;

    if (event->type == SDL_KEYDOWN && key == SDL_SCANCODE_RETURN)
    {
        doom->story.begin_story++;
        if (doom->story.first_item == TRUE)
          doom->story.item_story++;  
        if (doom->story.first_shoot == TRUE)
          doom->story.shoot_story++;
        if (doom->story.first_enemy == TRUE)
          doom->story.enemy_story++;  
    }
}

void			g_story_render(t_gui *self, t_doom *doom)
{
    const char      str[5][100] = {"BONJOUR SOLDAT\0", "AUJOURD'HUI NOUS AVONS UNE MISSION POUR TOI...\0", "TUER HITLER\0", "TU ES LE SEUL A POUVOIR LE FAIRE\0", "D'ABORD TROUVE DES ITEMS\0"};
    const char      str1[4][100] = {"SUPER! TON PREMIER ITEM\0", "TROUVE UNE ARME QUI TE PLAIT ET POURSUIS TA MISSION\0"};
    const char      str2[4][100] = {"OUF...TU L'AS ECHAPPE BELLE\0"};
    const char      str3[4][100] = {"ATTENTION!!!\0", "CE SOLDAT N'EST PAS AVEC NOUS...TIRE LUI DESSUS OU FUIS!!\0"};
    SDL_Surface		*text;
    
    if (doom->story.begin_story < 5
        || (doom->story.first_item == TRUE && doom->story.item_story < 2)
        || (doom->story.first_shoot == TRUE && doom->story.shoot_story < 1)
        || (doom->story.first_enemy == TRUE && doom->story.enemy_story < 2))
    {
        fill_rect(&doom->screen, (SDL_Rect)
            { 0, S_HEIGHT - 150, S_WIDTH, 150 }, 0xFF303030);
        draw_rect(&doom->screen, (SDL_Rect)
            { 0, S_HEIGHT - 150, S_WIDTH, 250 }, 0xFFFFFFFF);
        if (doom->story.begin_story < 5)
        {
            text = TTF_RenderText_Blended(doom->fonts.helvetica, str[doom->story.begin_story], (SDL_Color){255, 255, 255, 0});
            apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH/2 - text->w/2, S_HEIGHT - 100, text->w, text->h});
            SDL_FreeSurface(text);
        }
        else if (doom->story.first_item == TRUE && doom->story.item_story < 2)
        {
            text = TTF_RenderText_Blended(doom->fonts.helvetica, str1[doom->story.item_story], (SDL_Color){255, 255, 255, 0});
            apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH/2 - text->w/2, S_HEIGHT - 100, text->w, text->h});
            SDL_FreeSurface(text);
            if (doom->story.item_story == 2)
                doom->story.first_item = FALSE;
        }  
        else if (doom->story.first_shoot == TRUE && doom->story.shoot_story < 1)
        {
            text = TTF_RenderText_Blended(doom->fonts.helvetica, str2[doom->story.shoot_story], (SDL_Color){255, 255, 255, 0});
            apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH/2 - text->w/2, S_HEIGHT - 100, text->w, text->h});
            SDL_FreeSurface(text);
            if (doom->story.shoot_story == 1)
                doom->story.first_shoot = FALSE;
        }
        else if (doom->story.first_enemy == TRUE && doom->story.enemy_story < 2)
        {
            text = TTF_RenderText_Blended(doom->fonts.helvetica, str3[doom->story.enemy_story], (SDL_Color){255, 255, 255, 0});
            apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH/2 - text->w/2, S_HEIGHT - 100, text->w, text->h});
            SDL_FreeSurface(text);
            if (doom->story.enemy_story == 2)
                doom->story.first_enemy = FALSE;
        }
        text = TTF_RenderText_Blended(doom->fonts.helvetica, "PRESS ENTER", (SDL_Color){255, 255, 255, 0});
        apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH - 200, S_HEIGHT - 50, text->w, text->h});
        SDL_FreeSurface(text);
    }
}


