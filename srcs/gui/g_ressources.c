#include <SDL_ttf.h>
#include "gui.h"
#include "doom.h"

#define PAGE_SIZE (17)

void			update_selects(t_gui *self, t_ressource_manager *rm)
{
	int	i;
	int	res_index;
	t_select	*s;
	t_textfield *text;

	i = -1;
	while (++i < PAGE_SIZE)
	{
		if (i < (rm->ressources->len - (rm->page * PAGE_SIZE)))
		{
			res_index = rm->page * PAGE_SIZE + i;
			self->components->values[i * 3]->visible = TRUE;
			s = (t_select *)self->components->values[i * 3];
			s->selected_item = rm->ressources->values[res_index]->type - 1;
			self->components->values[i * 3 + 1]->visible = TRUE;
			text = (t_textfield *)self->components->values[i * 3 + 1];
			ft_strcpy(text->text, 
				rm->ressources->values[res_index]->display_name);
			text->text_len = ft_strlen(rm->ressources->values[res_index]->display_name);
			text->super.enabled = !rm->ressources->values[res_index]->fixed;
			s->super.enabled = !rm->ressources->values[res_index]->fixed && !rm->ressources->values[res_index]->loaded; 
			s->fg_color = !s->super.enabled ? 0xFFAAAAAA : 0xFFFFFFFF;
			text->fg_color = !text->super.enabled ? 0xFFAAAAAA : 0xFFFFFFFF;
			self->components->values[i * 3 + 2]->visible = !rm->ressources->values[res_index]->fixed;
			self->components->values[i * 3 + 2]->enabled = !rm->ressources->values[res_index]->fixed;
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
	int		i;

	i = -1;
	while (++i < PAGE_SIZE)
	{
		if (i < (doom->res_manager.ressources->len - (doom->res_manager.page * PAGE_SIZE)))
		{
			t_select *select = (t_select *)self->components->values[i * 3];
			if (select->selected_item >= 0)
				doom->res_manager.ressources->values[doom->res_manager.page * PAGE_SIZE + i]->type = select->items->values[select->selected_item].value;
			t_textfield *tf = (t_textfield *)self->components->values[i * 3 + 1];
			ft_strcpy(doom->res_manager.ressources->values[doom->res_manager.page * PAGE_SIZE + i]->display_name, tf->text);
		}
	}
}

static t_bool		delete_ressource_performed(t_component *cmp, t_doom *doom)
{
	int	i;

	i = -1;
	while (++i < PAGE_SIZE)
	{
		if (cmp == doom->guis[doom->current_gui].components->values[i * 3 + 2])
		{
			int index = i + (doom->res_manager.page * PAGE_SIZE);
			if (index >= 0 && index < doom->res_manager.ressources->len
				&& doom->res_manager.ressources->values[index]->used == 0)
			{
				splice_ressources_array(doom->res_manager.ressources, index, 1);
				if (doom->res_manager.page > get_pages_count(&doom->res_manager))
					doom->res_manager.page = get_pages_count(&doom->res_manager);
				update_selects(&doom->guis[doom->current_gui], &doom->res_manager);
			}
			return (TRUE);
		}
	}
	return (TRUE);
}

t_vec2	get_mouse_pos(t_doom *doom)
{
	int	x;
	int	y;
	int	x_w;
	int	y_w;

	SDL_GetWindowPosition(doom->win, &x_w, &y_w);
	SDL_GetGlobalMouseState(&x, &y);
	return (t_vec2){ x - x_w, y - y_w };
}

void	g_ressources_on_event(t_gui *self, SDL_Event *event, t_doom *doom)
{	
	t_ressource		*r;

	(void)self;
	if (event->type == SDL_DROPFILE)
	{
		t_vec2	pos = get_mouse_pos(doom);
		int index = ((int)(pos.y - 53) / 30) + (doom->res_manager.page * PAGE_SIZE);
		if (index >= 0 && index < doom->res_manager.ressources->len)
		{
			r = doom->res_manager.ressources->values[index];
			if (r->type == RESSOURCE_UNSET)
				return ;
			load_ressource(doom, r, event->drop.file);
			SDL_free(event->drop.file);
			update_selects(&doom->guis[doom->current_gui], &doom->res_manager);
		}
	}
}

static t_bool		action_performed(t_component *cmp, t_doom *doom)
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
		a(doom, "", RESSOURCE_UNSET, FALSE);
	update_selects(&doom->guis[doom->current_gui], &doom->res_manager);
	if (cmp == doom->guis[doom->current_gui].components->values[PAGE_SIZE * 3] && !check_ressources_errors(doom))
	{
		set_gui(doom, GUI_MAIN_MENU);
		return (FALSE);
	}
	return (TRUE);
}

void	g_ressources_on_enter(t_gui *self, t_doom *doom)
{
	int i;

	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
	i = -1;
	while (++i < PAGE_SIZE)
	{
		append_components_array(&self->components, create_select((SDL_Rect){S_WIDTH_2, 55 + i * 30, 200, 25}, "RESSOURCE TYPE"));
		self->components->values[i * 3]->visible = FALSE;
		((t_select *)self->components->values[i * 3])->items = doom->res_manager.ressources_types;
		append_components_array(&self->components, create_textfield((SDL_Rect){S_WIDTH_2 - 353, 55 + i * 30, 350, 25}, "RESSOURCE NAME", FALSE));
		self->components->values[i * 3 + 1]->visible = FALSE;
		append_components_array(&self->components, create_button((SDL_Rect){S_WIDTH_2 + 202, 55 + i * 30, 25, 25}, NULL, "X"));
		self->components->values[i * 3 + 2]->visible = FALSE;
		self->components->values[i * 3 + 2]->perform_action = delete_ressource_performed;
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
	(void)self;
	SDL_EventState(SDL_DROPFILE, SDL_DISABLE);
	doom->screen.secure = FALSE;
}

int		get_pages_count(t_ressource_manager *rm)
{
	return (ceil((rm->ressources->len - 1) / PAGE_SIZE));
}

void	render_page_label(t_gui *self, t_doom *doom)
{
	const SDL_Color	color = (SDL_Color){255, 255, 255, 0};
	SDL_Surface *t;

(void)self;
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
	const t_vec2	pos = get_mouse_pos(doom);
	int	i;
	t_ressource		*res;

	//printf("%d %d\n", x - x_w, y - y_w);
	(void)self;
	i = -1;
	while (++i < PAGE_SIZE)
	{
		if (i < (doom->res_manager.ressources->len - (doom->res_manager.page * PAGE_SIZE)))
		{
			res = doom->res_manager.ressources->values[doom->res_manager.page * PAGE_SIZE + i];
			int color = res->loaded ? 0xFF00FF00 : 0xFFFF0000;
			if (in_bounds((SDL_Rect){ S_WIDTH_2 - 356, 53 + i * 30, 584, 28 }, pos))
				color = 0xFFFFFFFF;
			draw_rect(&doom->screen, (SDL_Rect){ S_WIDTH_2 - 356, 53 + i * 30, 584, 28 }, color);
		}
	}
}

void	g_ressources_render(t_gui *self, t_doom *doom)
{
	fill_color(&doom->screen, 0xFF252525);
	render_page(self, doom);
	render_components(doom, self);
	render_page_label(self, doom);
	apply_select_value(self, doom);
}
