/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:47:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/13 00:39:18 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

/*t_line	lines[] = {
	{ .a = { 0, -25 },  .b = { 250, 25 } },
	{ .a = { 250, 25 }, .b = { 500, 75 } },
	{ .a = { 500, 75 }, .b = { 500, 300 }},
	{ .a = {500, 300},  .b = { 800, 300 }},
	{ .a = {800, 300},  .b = { 800, 350 }},
	{ .a = {800, 350},  .b = { 800, 550 }},
	{ .a = {800, 550},  .b = {0, 550 }   },
	{ .a = {0, 550},    .b = {0, -25}    }
};*/

t_vec3 list[] = {
	{574, 0, 74}, {562, 0, 254}, {751, 0, -185}, {642, 0, 186}, {662, 0, 223}, {732, 0, 140}, {749, 0, 144}, {696, 0, 235}, {923, 0, -32}, {901, 0, 21}, {743, 0, 237}, {852, 0, 169}, {920, 0, 175}, {961, 0, 200}, {913, 0, 278}, {988, 0, 259}, {937, 0, 335}, {956, 0, 355}, {1195, 0, 319}, {866, 0, 443}, {867, 0, 477}, {1062, 0, 497}, {805, 0, 504}, {1110, 0, 550}, {866, 0, 540}, {1477, 0, 712}, {640, 0, 538}, {842, 0, 600}, {1095, 0, 737}, {1283, 0, 853}, {1053, 0, 792}, {1107, 0, 884}, {857, 0, 750}, {753, 0, 707}, {878, 0, 819}, {880, 0, 882}, {805, 0, 856}, {949, 0, 1123}, {907, 0, 1099}, {727, 0, 912}, {729, 0, 929}, {748, 0, 1040}, {691, 0, 1011}, {617, 0, 893}, {630, 0, 1159}, {539, 0, 908}, {518, 0, 815}, {508, 0, 1241}, {472, 0, 816}, {422, 0, 1028}, {388, 0, 1045}, {333, 0, 1088}, {371, 0, 922}, {357, 0, 867}, {183, 0, 1127}, {145, 0, 1163}, {163, 0, 1021}, {132, 0, 1009}, {226, 0, 810}, {62, 0, 944}, {175, 0, 797}, {203, 0, 745}, {58, 0, 793}, {-9, 0, 786}, {82, 0, 726}, {121, 0, 665}, {-172, 0, 729}, {-80, 0, 662}, {175, 0, 569}, {202, 0, 544}, {-63, 0, 569}, {46, 0, 512}, {-170, 0, 496}, {65, 0, 474}, {-277, 0, 379}, {-484, 0, 325}, {-107, 0, 367}, {-23, 0, 344}, {-168, 0, 238}, {-76, 0, 212}, {-196, 0, 57}, {-47, 0, 69}, {26, 0, 81}, {250, 0, 232}, {290, 0, 262}, {182, 0, 126}, {188, 0, 65}, {129, 0, -45}, {253, 0, 94}, {56, 0, -269}, {285, 0, 66}, {358, 0, 172}, {358, 0, 81}, {318, 0, -78}, {424, 0, 144}, {476, 0, 322}, {465, 0, 129}, {500, 0, 104}, {524, 0, -39}, {545, 0, 127}
};

void	init_bsp(t_doom *doom)
{
	/*int count = sizeof(lines) / sizeof(t_line);
	t_polygons *polygons = create_polygons_array(count + 2);
	
	t_3dvertices *vertices = create_3dvertices_array(count);
	for (int i = 0; i < count; i++)
	{
		append_3dvertices_array(&vertices, (t_vec3){
			lines[i].a.x / 100., 0, lines[i].a.y / 100.});
	}
	append_polygons_array(&polygons, create_polygon(vertices, P_FLOOR));
	
	t_3dvertices *vertices_ceil = create_3dvertices_array(count);
	for (int i = 0; i < count; i++)
	{
		append_3dvertices_array(&vertices_ceil, (t_vec3){
			lines[i].a.x / 100., 1, lines[i].a.y / 100.});
	}
	append_polygons_array(&polygons, create_polygon(vertices_ceil, P_FLOOR));
	
	for (int i = 0; i < count; i++)
	{
		append_polygons_array(&polygons, create_wall_polygon((t_line){ (t_vec2){lines[i].a.x / 100., (lines[i].a.y / 100.)}, (t_vec2){lines[i].b.x / 100., (lines[i].b.y / 100.)}}, 0, 1));
	}
*/
	int count = sizeof(list) / sizeof(t_vec3);
	t_polygons *polygons = create_polygons_array(count);
	for (int i = 0; i < count; i++)
	{
		int next = (i + 1) % count;
		append_polygons_array(&polygons, create_wall_polygon((t_line){ {list[i].x / 100, list[i].z / 100}, {list[next].x / 100, list[next].z / 100}, 0}, 0, 1));
	}
	t_node *n = create_node(polygons);
	build_node(n);
	triangulate_bsp(n);
	printf("graph TD\n");
	print_node(n);
	doom->bsp = n;
}

int		main(void)
{
	t_doom doom = (t_doom) {
		.running = TRUE
	};
	init_bsp(&doom);
	init_sdl(&doom);
	if (!(doom.rendered_area = (uint8_t *)malloc(doom.screen.width)))
		return (-1);
	game_loop(&doom);
	sdl_quit(&doom);
	free_node(doom.bsp);
	return (0);
}
