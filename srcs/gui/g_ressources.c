#include <SDL_ttf.h>
#include "gui.h"
#include "doom.h"

#define PAGE_SIZE (17)

void			update_selects(t_gui *self, t_ressource_manager *rm)
{
	int	i;
	t_select	*s;
	t_textfield *text;

	i = -1;
	while (++i < PAGE_SIZE)
	{
		if (i < (rm->ressources->len - (rm->page * PAGE_SIZE)))
		{
			self->components->values[i * 3]->visible = TRUE;
			s = (t_select *)self->components->values[i * 3];
			s->selected_item = rm->ressources->values[i]->type - 1;
			self->components->values[i * 3 + 1]->visible = TRUE;
			text = (t_textfield *)self->components->values[i * 3 + 1];
			ft_strcpy(text->text, 
				rm->ressources->values[i]->display_name);
			text->text_len = ft_strlen(rm->ressources->values[i]->display_name);
			text->super.enabled = !rm->ressources->values[i]->fixed;
			s->super.enabled = text->super.enabled;
			s->fg_color = !s->super.enabled ? 0xFFAAAAAA : 0xFFFFFFFF;
			text->fg_color = !text->super.enabled ? 0xFFAAAAAA : 0xFFFFFFFF;
			self->components->values[i * 3 + 2]->visible = TRUE;
		}
		else
		{
			self->components->values[i * 3]->visible = FALSE;
			self->components->values[i * 3 + 1]->visible = FALSE;
			self->components->values[i * 3 + 2]->visible = FALSE;
		}
	}
}

void			apply_select_value(t_gui *self, t_doom *doom)
{
	int	i;
	t_select	*select;

	i = -1;
	while (++i < self->components->len)
	{
		if (i < (doom->res_manager.ressources->len - (doom->res_manager.page * PAGE_SIZE)))
		{
			t_select *select = (t_select *)self->components->values[PAGE_SIZE - 1 - i];
			doom->res_manager.ressources->values[doom->res_manager.page * PAGE_SIZE + i]->type = select->items->values[select->selected_item].value;
		}
	}
}

static void		action_performed(t_component *cmp, t_doom *doom)
{
	if (cmp == doom->guis[doom->current_gui].components->values[PAGE_SIZE * 3 + 1])
		doom->res_manager.page--;
	else if (cmp == doom->guis[doom->current_gui].components->values[PAGE_SIZE * 3 + 2])
		doom->res_manager.page++;
	if (doom->res_manager.page < 0)
		doom->res_manager.page = get_pages_count(&doom->res_manager);
	if (doom->res_manager.page > get_pages_count(&doom->res_manager))
		doom->res_manager.page = 0;
	if (cmp == doom->guis[doom->current_gui].components->values[PAGE_SIZE * 3 + 3])
		a(doom, "EMPTY", RESSOURCE_UNSET, FALSE);
	update_selects(&doom->guis[doom->current_gui], &doom->res_manager);
}

void	g_ressources_on_enter(t_gui *self, t_doom *doom)
{
	doom->screen.secure = TRUE;
	int i;
	i = -1;
	while (++i < PAGE_SIZE)
	{
		append_components_array(&self->components, create_select((SDL_Rect){S_WIDTH_2, 55 + i * 30, 200, 25}, "RESSOURCE TYPE"));
		self->components->values[i * 3]->visible = FALSE;
		((t_select *)self->components->values[i * 3])->items = doom->res_manager.ressources_types;
		append_components_array(&self->components, create_textfield((SDL_Rect){S_WIDTH_2 - 353, 55 + i * 30, 350, 25}, "RESSOURCE NAME"));
		self->components->values[i * 3 + 1]->visible = FALSE;
		append_components_array(&self->components, create_button((SDL_Rect){S_WIDTH_2 + 202, 55 + i * 30, 25, 25}, NULL, "X"));
		self->components->values[i * 3 + 2]->visible = FALSE;
	}

	append_components_array(&self->components, create_button((SDL_Rect){S_WIDTH - 210, S_HEIGHT - 50, 200, 40}, NULL, "CONTINUE"));
	append_components_array(&self->components, create_button((SDL_Rect){S_WIDTH_2 - 140, S_HEIGHT - 50, 40, 40}, NULL, "<"));
	append_components_array(&self->components, create_button((SDL_Rect){S_WIDTH_2 + 100, S_HEIGHT - 50, 40, 40}, NULL, ">"));
	append_components_array(&self->components, create_button((SDL_Rect){S_WIDTH - 215, 15, 200, 40}, NULL, "ADD RESSOURCE"));
	i = -1;
	while (++i < 4)
		self->components->values[(PAGE_SIZE * 3) + i]->perform_action = action_performed;
	update_selects(self, &doom->res_manager);
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
		(SDL_Rect){S_WIDTH_2 - 60 - t->w, S_HEIGHT - 60 + t->h / 2, t->w, t->h});
	SDL_FreeSurface(t);
	t = TTF_RenderText_Blended(doom->fonts.helvetica, "/", color);
	apply_surface_blended(&doom->screen, t, (SDL_Rect){0, 0, t->w, t->h},
		(SDL_Rect){S_WIDTH_2 - t->w / 2, S_HEIGHT - 60 + t->h / 2, t->w, t->h});
	SDL_FreeSurface(t);
	t = TTF_RenderText_Blended(doom->fonts.helvetica, ft_int_to_str(get_pages_count(&doom->res_manager)).str, color);
	apply_surface_blended(&doom->screen, t, (SDL_Rect){0, 0, t->w, t->h},
		(SDL_Rect){S_WIDTH_2 + 40 - t->w, S_HEIGHT - 60 + t->h / 2, t->w, t->h});
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
	t_ressource		*res;

	i = -1;
	while (++i < PAGE_SIZE)
	{
		if (i < (doom->res_manager.ressources->len - (doom->res_manager.page * PAGE_SIZE)))
		{
			res = doom->res_manager.ressources->values[i];
			draw_rect(&doom->screen, (SDL_Rect){ S_WIDTH_2 - 356, 53 + i * 30, 584, 28 }, res->loaded ? 0xFF00FF00 : 0xFFFF0000);
		}
	}
}

void	g_ressources_render(t_gui *self, t_doom *doom)
{
	fill_color(&doom->screen, 0xFF252525);
	render_page(self, doom);
	render_components(doom, self);
	render_page_label(self, doom);
}
