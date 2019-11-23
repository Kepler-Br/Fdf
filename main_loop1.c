/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsena <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/21 18:49:57 by nsena             #+#    #+#             */
/*   Updated: 2019/10/21 18:50:00 by nsena            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int			t_main_loop_loop(t_main_loop *this)
{
	const t_mat4 translation_matrix = trans_mat(this->current_position.x, this->current_position.y, 0.0f);
	const t_mat4 scale_matrix = scale_mat(this->current_radius, this->current_radius, this->current_radius);
	const t_mat4 model_matrix = mat4_mat4_mul(&scale_matrix, &translation_matrix);
	const t_mat4 view_matrix = look_at((t_vec3){500.0f * cosf(this->rotation.x) * sinf(this->rotation.y),
												500.0f * sinf(this->rotation.x) * sinf(this->rotation.y),
												500.0f * cosf(this->rotation.y)},
									   (t_vec3){0.0f, 0.0f, 0.0f},
									   (t_vec3){0.0f, 0.0f, 1.0f});
	const t_mat4 PV = mat4_mat4_mul(&this->projection, &view_matrix);

	this->current_radius = lerpf(this->current_radius, this->target_radius, 0.1f);
	this->current_position = lerpvec3(this->current_position, this->target_position, 0.1f);
	this->mvp = mat4_mat4_mul(&PV, &model_matrix);
	this->render(this);
	return (0);
}

static void			t_main_loop_render_as_plane(t_main_loop *this)
{
	static t_vec4 point1;
	static t_vec4 point2;
	static t_ivec2 coord;

	NWHILE(coord.x = 0, coord.x < this->landscape->dimensions.x);
		NWHILE(coord.y = 1, coord.y < this->landscape->dimensions.y);
			point1 = (t_vec4){(float)coord.x, (float)coord.y-1, this->landscape->get(this->landscape, (t_ivec2){coord.x, coord.y-1}) * this->landscape_multiplier, 1.0f};
			point2 = (t_vec4){(float)coord.x, (float)coord.y, this->landscape->get(this->landscape, (t_ivec2){coord.x, coord.y})* this->landscape_multiplier, 1.0f};
			point1 = mat4_vec4_mul(&this->mvp, &point1);
			point2 = mat4_vec4_mul(&this->mvp, &point2);
			if (this->is_perspective)
				this->renderer->line(this->renderer, point1, point2,
									 (t_ivec2){lerpcolor(this->gradient.x, this->gradient.y, this->landscape->get(this->landscape, (t_ivec2){coord.x, coord.y-1})/(this->landscape->max)),
											   lerpcolor(this->gradient.x, this->gradient.y, this->landscape->get(this->landscape, (t_ivec2){coord.x, coord.y})/(this->landscape->max))});
			else
				this->renderer->line_ortho(this->renderer, point1, point2,
										   (t_ivec2){lerpcolor(this->gradient.x, this->gradient.y, this->landscape->get(this->landscape, (t_ivec2){coord.x, coord.y-1})/(this->landscape->max)),
													 lerpcolor(this->gradient.x, this->gradient.y, this->landscape->get(this->landscape, (t_ivec2){coord.x, coord.y})/(this->landscape->max))});
			END_NWHILE(coord.y++);
	END_NWHILE(coord.x++);
	NWHILE(coord.x = 1, coord.x < this->landscape->dimensions.x);
		NWHILE(coord.y = 0, coord.y < this->landscape->dimensions.y);
			point1 = (t_vec4){(float)coord.x-1, (float)coord.y, this->landscape->get(this->landscape, (t_ivec2){coord.x-1, coord.y})* this->landscape_multiplier, 1.0f};
			point2 = (t_vec4){(float)coord.x, (float)coord.y, this->landscape->get(this->landscape, (t_ivec2){coord.x, coord.y})* this->landscape_multiplier, 1.0f};
			point1 = mat4_vec4_mul(&this->mvp, &point1);
			point2 = mat4_vec4_mul(&this->mvp, &point2);
			if (this->is_perspective)
				this->renderer->line(this->renderer, point1, point2,
						(t_ivec2){lerpcolor(this->gradient.x, this->gradient.y, this->landscape->get(this->landscape, (t_ivec2){coord.x-1, coord.y})/(this->landscape->max)),
								lerpcolor(this->gradient.x, this->gradient.y, this->landscape->get(this->landscape, (t_ivec2){coord.x, coord.y})/(this->landscape->max))});
			else
				this->renderer->line_ortho(this->renderer, point1, point2,
						(t_ivec2){lerpcolor(this->gradient.x, this->gradient.y, this->landscape->get(this->landscape, (t_ivec2){coord.x-1, coord.y})/(this->landscape->max)),
								lerpcolor(this->gradient.x, this->gradient.y, this->landscape->get(this->landscape, (t_ivec2){coord.x, coord.y})/(this->landscape->max))});

		END_NWHILE(coord.y++);
	END_NWHILE(coord.x++);
}

static void			t_main_loop_render_as_sphere(t_main_loop *this)
{
	static t_vec4 point1;
	static t_vec4 point2;
	float theta;
	float alpha;
	static t_ivec2 landscape_steps;
	const float ystep = M_PI/this->landscape->dimensions.y;
	const float xstep = M_PI*2/this->landscape->dimensions.x;

	landscape_steps = (t_ivec2){0, 0};
	for (alpha = 0; alpha < M_PI*2; alpha += xstep)
	{
		for (theta = 0.0f; theta < M_PI; theta += ystep)
		{
			point1.x = (10.0f + this->landscape_multiplier * this->landscape->get(this->landscape, landscape_steps)) * cosf(alpha) * sinf(theta);
			point1.y = (10.0f + this->landscape_multiplier * this->landscape->get(this->landscape, landscape_steps)) * sinf(alpha) * sinf(theta);
			point1.z = (10.0f + this->landscape_multiplier * this->landscape->get(this->landscape, landscape_steps)) * cosf(theta);
			point1.w = 1.0f;

			point2.x = (10.0f + this->landscape_multiplier * this->landscape->get(this->landscape, (t_ivec2){landscape_steps.x, landscape_steps.y-1})) * cosf(alpha) * sinf(theta - ystep);
			point2.y = (10.0f + this->landscape_multiplier * this->landscape->get(this->landscape, (t_ivec2){landscape_steps.x, landscape_steps.y-1})) * sinf(alpha) * sinf(theta - ystep);
			point2.z = (10.0f + this->landscape_multiplier * this->landscape->get(this->landscape, (t_ivec2){landscape_steps.x, landscape_steps.y-1})) * cosf(theta - ystep);
			point2.w = 1.0f;

			point1 = mat4_vec4_mul(&this->mvp, &point1);
			point2 = mat4_vec4_mul(&this->mvp, &point2);
			if (this->is_perspective)
				this->renderer->line(this->renderer, point1, point2,
									 (t_ivec2){lerpcolor(this->gradient.x, this->gradient.y,this->landscape->get(this->landscape, landscape_steps)/(this->landscape->max)),
											   lerpcolor(this->gradient.x, this->gradient.y, this->landscape->get(this->landscape, (t_ivec2){landscape_steps.x, landscape_steps.y-1})/(this->landscape->max))});
			else
				this->renderer->line_ortho(this->renderer, point1, point2,
									 (t_ivec2){lerpcolor(this->gradient.x, this->gradient.y,this->landscape->get(this->landscape, landscape_steps)/(this->landscape->max)),
											   lerpcolor(this->gradient.x, this->gradient.y, this->landscape->get(this->landscape, (t_ivec2){landscape_steps.x, landscape_steps.y-1})/(this->landscape->max))});

			landscape_steps.y += 1;
		}
		landscape_steps.x += 1;
		landscape_steps.y = 0;
	}

	landscape_steps = (t_ivec2){0, 0};
	for (alpha = 0; alpha < M_PI*2; alpha+= xstep)
	{
		for (theta = 0.0f; theta < M_PI; theta += ystep)
		{
			point1.x = (10.0f + this->landscape_multiplier * this->landscape->get(this->landscape, landscape_steps)) * cosf(alpha) * sinf(theta);
			point1.y = (10.0f + this->landscape_multiplier * this->landscape->get(this->landscape, landscape_steps)) * sinf(alpha) * sinf(theta);
			point1.z = (10.0f + this->landscape_multiplier * this->landscape->get(this->landscape, landscape_steps)) * cosf(theta);
			point1.w = 1.0f;

			point2.x = (10.0f + this->landscape_multiplier * this->landscape->get(this->landscape, (t_ivec2){landscape_steps.x-1, landscape_steps.y})) * cosf(alpha - xstep) * sinf(theta);
			point2.y = (10.0f + this->landscape_multiplier * this->landscape->get(this->landscape, (t_ivec2){landscape_steps.x-1, landscape_steps.y})) * sinf(alpha - xstep) * sinf(theta);
			point2.z = (10.0f + this->landscape_multiplier * this->landscape->get(this->landscape, (t_ivec2){landscape_steps.x-1, landscape_steps.y})) * cosf(theta);
			point2.w = 1.0f;

			point1 = mat4_vec4_mul(&this->mvp, &point1);
			point2 = mat4_vec4_mul(&this->mvp, &point2);
			if (this->is_perspective)
				this->renderer->line(this->renderer, point1, point2,
									 (t_ivec2){lerpcolor(this->gradient.x, this->gradient.y,this->landscape->get(this->landscape, landscape_steps)/(this->landscape->max)),
											   lerpcolor(this->gradient.x, this->gradient.y, this->landscape->get(this->landscape, (t_ivec2){landscape_steps.x-1, landscape_steps.y})/(this->landscape->max))});
			else
				this->renderer->line_ortho(this->renderer, point1, point2,
									 (t_ivec2){lerpcolor(this->gradient.x, this->gradient.y,this->landscape->get(this->landscape, landscape_steps)/(this->landscape->max)),
											   lerpcolor(this->gradient.x, this->gradient.y, this->landscape->get(this->landscape, (t_ivec2){landscape_steps.x-1, landscape_steps.y})/(this->landscape->max))});

			landscape_steps.y += 1;
		}

		landscape_steps.x += 1;
		landscape_steps.y = 0;
	}
}

static int			t_main_loop_render(t_main_loop *this)
{
	this->renderer->clear(this->renderer);
	if (this->is_sphere)
		this->render_as_sphere(this);
	else
		this->render_as_plane(this);
	this->renderer->flush(this->renderer);
	if (this->is_draw_text)
		this->draw_text(this);
	return (0);
}

static int			t_main_loop_on_key(int key, t_main_loop *this)
{
	printf("%d!\n", key);
	if (key == MLX_K_P)
		image_to_bmp(this->renderer->buffer, "screenshot.bmp");
	if (key == MLX_K_ESC)
		this->on_close(this);
	if (key == MLX_K_D)
		this->target_position.x += 5.0f;
	if (key == MLX_K_R)
	{
		if (this->is_sphere)
			this->target_position = (t_vec3) {0.0f, 0.0f, 0.0f};
		else
			this->target_position = (t_vec3) {-(float) this->landscape->dimensions.x / 2.0f,
											-(float) this->landscape->dimensions.y / 2.0f, 0.0f};
		this->target_radius = 10.5f;
	}
	if (key == MLX_K_X)
	{
		this->is_sphere = !this->is_sphere;
		if (this->is_sphere)
			this->target_position = (t_vec3) {0.0f, 0.0f, 0.0f};
		else
			this->target_position = (t_vec3) {-(float) this->landscape->dimensions.x / 2.0f,
											  -(float) this->landscape->dimensions.y / 2.0f, 0.0f};
	}
	if (key == MLX_K_C)
		this->is_perspective = !this->is_perspective;
	if (key == MLX_K_A)
		this->target_position.x -= 5.0f;
	if (key == MLX_K_S)
		this->target_position.y += 5.0f;
	if (key == MLX_K_W)
		this->target_position.y -= 5.0f;
	if (key == MLX_K_Q)
		this->landscape_multiplier -= this->landscape_multiplier < 0.01f ? 0.0f : 0.01f;
	if (key == MLX_K_E)
		this->landscape_multiplier += 0.01f;
	if (key == MLX_K_T)
		this->is_draw_text = !this->is_draw_text;
	if (key == MLX_K_1)
		this->gradient = (t_ivec2){0x0000FF, 0xFF0000};
	if (key == MLX_K_2)
		this->gradient = (t_ivec2){0x00FF00, 0xFF00FF};
	if (key == MLX_K_3)
		this->gradient = (t_ivec2){0x111111, 0xFFFFFF};
	if (key == MLX_K_4)
		this->gradient = (t_ivec2){0xDF825F, 0x706695};
	if (key == MLX_K_5)
		this->gradient = (t_ivec2){0x86E3CE, 0xFA897B};
	if (key == MLX_K_6)
		this->gradient = (t_ivec2){0x04AA04, 0xD2691E};
	if (key == MLX_K_7)
		this->gradient = (t_ivec2){0x155177, 0x04AA04};
	if (key == MLX_K_8)
		this->gradient = (t_ivec2){0xffedbc, 0xed4264};
	if (key == MLX_K_9)
		this->gradient = (t_ivec2){0xFE7A15, 0x35BBCA};
	return (0);
}

static int			t_main_loop_on_mouse(int x, int y, t_main_loop *this)
{
	this->mouse_delta = (t_vec2){(float)x - this->prev_mouse_coord.x,
								(float)y - this->prev_mouse_coord.y};
	this->prev_mouse_coord = (t_vec2){(float)x, (float)y};
	if (this->left_mouse_down)
	{
		this->rotation.x += this->mouse_delta.x / 500.0f;
		this->rotation.y += this->mouse_delta.y / 500.0f;
		if (this->rotation.x > M_PI*2)
			this->rotation.x -= (float)M_PI*2;
		if (this->rotation.y > M_PI/1.5)
			this->rotation.y = M_PI/1.5;
		if (this->rotation.x < 0)
			this->rotation.x += (float)M_PI*2;
		if (this->rotation.y < M_PI/8)
			this->rotation.y = M_PI/8;
	}
	return (0);
}

static int			t_main_loop_on_mouse_up(int button, int x, int y, t_main_loop *this)
{
	(void)x;
	(void)y;
	if (button == 1)
		this->left_mouse_down = 0;
	return (0);
}

static int			t_main_loop_on_mouse_down(int button, int x, int y, t_main_loop *this)
{
	(void)x;
	(void)y;
	if (button == 4)
		this->target_radius += 0.5f;
	if (button == 5)
		this->target_radius -= 0.5f;
	if (this->target_radius < 0.0f)
		this->target_radius = 0.0f;
	if (button == 1)
		this->left_mouse_down = 1;
	return (0);
}

static int					t_main_loop_on_close(t_main_loop *this)
{
	destroy_t_main_loop(this);
	exit(0);
}

static void					t_main_loop_draw_text(t_main_loop *this)
{
	const static int text_color = 0xFFFFFF;
	const static int x = 65;
	int y;

	y = 0;
	mlx_string_put(this->mlx, this->window, x, y += 20, text_color, "How to Use:");
	mlx_string_put(this->mlx, this->window, x - 50, y += 30, text_color, "WASD - move camera.");
	mlx_string_put(this->mlx, this->window, x - 50, y += 30, text_color, "Mouse wheel - zoom.");
	mlx_string_put(this->mlx, this->window, x - 50, y += 30, text_color, "ESC - exit.");
	mlx_string_put(this->mlx, this->window, x - 50, y += 30, text_color, "LMB - camera rotation.");
	mlx_string_put(this->mlx, this->window, x - 50, y += 30, text_color, "Q E - terrain height.");
	mlx_string_put(this->mlx, this->window, x - 50, y += 30, text_color, "R - reset camera.");
	mlx_string_put(this->mlx, this->window, x - 50, y += 30, text_color, "C - change projection.");
	mlx_string_put(this->mlx, this->window, x - 50, y += 30, text_color, "X - change mapping.");
	mlx_string_put(this->mlx, this->window, x - 50, y += 30, text_color, "T - show help.");
	mlx_string_put(this->mlx, this->window, x - 50, y += 30, text_color, "P - screenshot.");
	mlx_string_put(this->mlx, this->window, x - 50, y += 30, text_color, "1-9 - change gradient.");
}

static void                t_main_loop_run(t_main_loop *this)
{
	mlx_loop(this->mlx);
}

void				destroy_t_main_loop(t_main_loop *object)
{
	destroy_t_landscape(object->landscape);
	destroy_t_renderer(object->renderer);
	mlx_destroy_window(object->mlx, object->window);
	free(object);
}

t_main_loop			*make_t_main_loop(char *title, t_ivec2 geometry, t_landscape *landscape)
{
	t_main_loop *object;

	object = (t_main_loop *)malloc(sizeof(t_main_loop));

	object->mlx = mlx_init();
	object->window = mlx_new_window(object->mlx, (int)geometry.x, (int)geometry.y, title);
	if (!object->window)
	{
		free(object);
		return (NULL);
	}
	object->win_geometry = geometry;
	object->renderer = make_t_renderer(object->mlx, object->window, object->win_geometry);
	if (!object->renderer)
	{
		mlx_destroy_window(object->mlx, object->window);
		free(object);
		return (NULL);
	}
	object->landscape = landscape;

	object->mouse_delta = (t_vec2){0.0f, 0.0f};
	object->prev_mouse_coord = (t_vec2){0.0f, 0.0f};
	object->target_position = (t_vec3){-(float)object->landscape->dimensions.x/2.0f,
										-(float)object->landscape->dimensions.y/2.0f, 0.0f};
	object->current_position = (t_vec3){0.0f, 0.0f, 0.0f};
	object->rotation = (t_vec3){3.8f, 1.0f, 0.0f};
	object->projection = orthographic((t_vec4){-1.0f, 1.0f, -1.0f, 1.0f},
								0.01f, 500.0f);

	object->target_radius = 10.5f;
	object->current_radius = 1.0f;
	object->is_perspective = 1;
	object->is_draw_text = 1;
	object->is_sphere = 0;
	object->landscape_multiplier = 1.0f;
	object->gradient = (t_ivec2){0x0000FF, 0xFF0000};

	object->loop = &t_main_loop_loop;
	object->run = &t_main_loop_run;
	object->on_key = &t_main_loop_on_key;
	object->on_mouse = &t_main_loop_on_mouse;
	object->render = &t_main_loop_render;
	object->on_mouse_down = &t_main_loop_on_mouse_down;
	object->on_mouse_up = &t_main_loop_on_mouse_up;
	object->on_close = &t_main_loop_on_close;
	object->render_as_plane = &t_main_loop_render_as_plane;
	object->render_as_sphere = &t_main_loop_render_as_sphere;
	object->draw_text = &t_main_loop_draw_text;

	mlx_hook(object->window, 2, 0, object->on_key, (void *)object);
	mlx_hook(object->window, 6, 0, object->on_mouse, (void *)object);
	mlx_hook(object->window, 4, 0, object->on_mouse_down, (void *)object);
	mlx_hook(object->window, 5, 0, object->on_mouse_up, (void *)object);
	mlx_hook(object->window, 17, 0, object->on_close, (void *)object);
	mlx_loop_hook(object->mlx, object->loop, (void *)object);
	return (object);
}
