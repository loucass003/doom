#include <SDL_ttf.h>
#include "gui.h"
#include "doom.h"

#define PAGE_SIZE (17)

void			update_selects(t_gui *self, t_doom *doom)
{
	int	i;
	t_select	*select;

	i = -1 + 3;
	while (++i < self->components->len)
	{
		if (i < (doom->res_manager.ressources->len - (doom->res_manager.page * PAGE_SIZE)) + 3)
		{
			self->components->values[PAGE_SIZE + 4 - i]->visible = TRUE;
			t_select *select = (t_select *)self->components->values[PAGE_SIZE + 4 - i];
			select->selected_item = doom->res_manager.ressources->values[doom->res_manager.page * PAGE_SIZE + (i - 3)]->type - 1;
		}
		else
			self->components->values[PAGE_SIZE + 4 - i]->visible = FALSE;
	}
}

void			apply_select_value(t_gui *self, t_doom *doom)
{
	int	i;
	t_select	*select;

	i = -1 + 3;
	while (++i < self->components->len)
	{
		if (i < (doom->res_manager.ressources->len - (doom->res_manager.page * PAGE_SIZE)) + 3)
		{
			t_select *select = (t_select *)self->components->values[PAGE_SIZE + 4 - i];
			doom->res_manager.ressources->values[doom->res_manager.page * PAGE_SIZE + (i - 3)]->type = select->items->values[select->selected_item].value;
		}
	}
}

static void		action_performed(t_component *cmp, t_doom *doom)
{
	if (cmp == doom->guis[doom->current_gui].components->values[1])
		doom->res_manager.page--;
	else if (cmp == doom->guis[doom->current_gui].components->values[2])
		doom->res_manager.page++;
	if (doom->res_manager.page < 0)
		doom->res_manager.page = get_pages_count(&doom->res_manager);
	if (doom->res_manager.page > get_pages_count(&doom->res_manager))
		doom->res_manager.page = 0;
	update_selects(&doom->guis[doom->current_gui], doom);
}

void	g_ressources_on_enter(t_gui *self, t_doom *doom)
{
	doom->screen.secure = TRUE;
	int i;

	append_components_array(&self->components, create_button((SDL_Rect){S_WIDTH - 210, S_HEIGHT - 50, 200, 40}, NULL, "CONTINUE"));
	append_components_array(&self->components, create_button((SDL_Rect){S_WIDTH_2 - 140, S_HEIGHT - 50, 40, 40}, NULL, "<"));
	append_components_array(&self->components, create_button((SDL_Rect){S_WIDTH_2 + 100, S_HEIGHT - 50, 40, 40}, NULL, ">"));
	append_components_array(&self->components, create_textfield((SDL_Rect){100, 50, 200, 40}, "TEXTFIELD"));
	
	i = -1;
	while (++i < 3)
		self->components->values[i]->perform_action = action_performed;


	// i = PAGE_SIZE;
	// while (--i > 0)
	// {
	// 	append_components_array(&self->components, create_select((SDL_Rect){S_WIDTH_2, 25 + i * 31, 200, 30}, "RESSOURCE TYPE"));
	// 	self->components->values[self->components->len - 1]->visible = FALSE;
	// 	((t_select *)self->components->values[self->components->len - 1])->items = doom->res_manager.ressources_types;
	// }
	
	// update_selects(self, doom);
}

void	g_ressources_on_leave(t_gui *self, t_doom *doom)
{
	doom->screen.secure = FALSE;
}

int		get_pages_count(t_ressource_manager *rm)
{
	return (ceilf(rm->ressources->len / PAGE_SIZE));
}

void	render_page_label(t_gui *self, t_doom *doom)
{
	const SDL_Color	color = (SDL_Color){255, 255, 255, 0};
	SDL_Surface *t;

	t = TTF_RenderText_Blended(doom->fonts.helvetica,
		ft_int_to_str(doom->res_manager.page).str, color);
	apply_surface_blended(&doom->screen, t, (SDL_Rect){0, 0, t->w, t->h},
		(SDL_Rect){S_WIDTH_2 - 60 - t->w, S_HEIGHT - 52 + t->h / 2, t->w, t->h});
	SDL_FreeSurface(t);
	t = TTF_RenderText_Blended(doom->fonts.helvetica, "/", color);
	apply_surface_blended(&doom->screen, t, (SDL_Rect){0, 0, t->w, t->h},
		(SDL_Rect){S_WIDTH_2 - t->w / 2, S_HEIGHT - 52 + t->h / 2, t->w, t->h});
	SDL_FreeSurface(t);
	t = TTF_RenderText_Blended(doom->fonts.helvetica, ft_int_to_str(get_pages_count(&doom->res_manager)).str, color);
	apply_surface_blended(&doom->screen, t, (SDL_Rect){0, 0, t->w, t->h},
		(SDL_Rect){S_WIDTH_2 + 40 - t->w, S_HEIGHT - 52 + t->h / 2, t->w, t->h});
	SDL_FreeSurface(t);
	t = TTF_RenderText_Blended(doom->fonts.helvetica, "RESSOURCES MANAGER",
		color);
	apply_surface_blended(&doom->screen, t, (SDL_Rect){0, 0, t->w, t->h},
		(SDL_Rect){S_WIDTH_2 - t->w / 2, 15, t->w, t->h});
	SDL_FreeSurface(t);
}

void	render_page(t_gui *self, t_doom *doom)
{
	int	i;
	t_ressource	*res;
	SDL_Surface *t;

	// i = -1 + 3;
	// while (++i < self->components->len)
	// {
	// 	if (i < (doom->res_manager.ressources->len - (doom->res_manager.page * PAGE_SIZE)) + 3)
	// 	{
	// 		res = doom->res_manager.ressources->values[doom->res_manager.page * PAGE_SIZE + (i - 3)];
	// 		t = TTF_RenderText_Blended(doom->fonts.helvetica,
	// 			res->display_name, (SDL_Color){255, 255, 255, 0});
	// 		apply_surface_blended(&doom->screen, t, (SDL_Rect){0, 0, t->w, t->h},
	// 			(SDL_Rect){150, 25 + (i - 2) * 31, t->w, t->h});
	// 		SDL_FreeSurface(t);
	// 	}
	// }
}

void	g_ressources_render(t_gui *self, t_doom *doom)
{
	fill_color(&doom->screen, 0xFF252525);
	render_page(self, doom);
	render_page_label(self, doom);
	//apply_select_value(self, doom);
	render_components(doom, self);
}
