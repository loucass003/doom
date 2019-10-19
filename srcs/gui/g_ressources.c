#include "gui.h"
#include "doom.h"

void	g_ressources_on_enter(t_gui *self, t_doom *doom)
{
	append_components_array(&self->components, create_select((SDL_Rect){ (S_WIDTH - 250) / 2, (S_HEIGHT - 150) / 2, 250, 50 }, "Ressource type"));
	t_select	*select = (t_select *)self->components->values[0];
	append_select_items_array(&select->items, (t_select_item){ .name = "TEST", .value = 0 });
	append_select_items_array(&select->items, (t_select_item){ .name = "TESTLOL", .value = 0 });
	append_select_items_array(&select->items, (t_select_item){ .name = "COUCOU", .value = 0 });
}

void	g_ressources_on_leave(t_gui *self, t_doom *doom)
{

}

void	g_ressources_render(t_gui *self, t_doom *doom)
{
	fill_color(&doom->screen, 0xFF252525);
	render_components(doom, self);
}
