/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hturkatr <hturkatr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/04 18:05:04 by hturkatr          #+#    #+#             */
/*   Updated: 2019/10/21 16:30:57 by hturkatr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_FDF_H
# define FDF_FDF_H

# include <math.h>
# include "libft/libft.h"
# include "minilibx_macos/mlx.h"
# include <OpenGL/gl.h>
# include "gnl.h"
# include "fcntl.h"
# define MLX_K_A 0
# define MLX_K_S 1
# define MLX_K_D 2
# define MLX_K_X 7
# define MLX_K_C 8
# define MLX_K_Q 12
# define MLX_K_W 13
# define MLX_K_E 14
# define MLX_K_R 15
# define MLX_K_T 17
# define MLX_K_P 35
# define MLX_K_ESC 53
# define MLX_K_1 18
# define MLX_K_2 19
# define MLX_K_3 20
# define MLX_K_4 21
# define MLX_K_5 23
# define MLX_K_6 22
# define MLX_K_7 26
# define MLX_K_8 28
# define MLX_K_9 25
# define NWHILE(X, Y) X; while(Y) {
# define CLOSE_BRACKET }
# define END_NWHILE(X) X; }
# define INIT_COUNTER(X) int X = 0
# define INIT_STR(X) char *X = NULL
# define INIT_COORD(X) t_ivec2 X = (t_ivec2){0, 0};
# define INIT_MAP_POINT(X) t_map_point X = (t_map_point){(t_ivec2){0, 0}, 0.0, 0};

typedef struct s_data		t_data;
typedef struct s_map_point	t_map_point;
typedef struct s_vector		t_vector;
typedef struct s_main_loop	t_main_loop;
typedef struct s_renderer	t_renderer;
typedef struct s_image		t_image;
typedef struct s_landscape	t_landscape;
typedef struct s_mat4		t_mat4;
typedef struct s_vec4		t_vec4;
typedef struct s_vec3		t_vec3;
typedef struct s_vec2		t_vec2;
typedef struct s_ivec3		t_ivec3;
typedef struct s_ivec2		t_ivec2;

struct						s_ivec2
{
	int				x;
	int				y;
};

struct						s_ivec3
{
	int				x;
	int				y;
	int				z;
};

struct						s_vec2
{
	float			x;
	float			y;
};

struct						s_vec3
{
	float			x;
	float			y;
	float			z;
};

struct						s_vec4
{
	float			x;
	float			y;
	float			z;
	float			w;
};

struct						s_mat4
{
	float			a;
	float			b;
	float			c;
	float			d;
	float			e;
	float			f;
	float			g;
	float			h;
	float			i;
	float			j;
	float			k;
	float			l;
	float			m;
	float			n;
	float			o;
	float			p;
};

/*
** landscape stuff.
*/

struct						s_landscape
{
	float			*points;
	t_ivec2			dimensions;
	float			min;
	float			max;
	float			(*get)(struct s_landscape *this, t_ivec2 point);
	void			(*calculate_min_max)(struct s_landscape *this);
	void			(*normalize)(struct s_landscape *this);
	void			(*set)(struct s_landscape *this, t_ivec2 point, float num);
};

t_landscape					*make_t_landscape(t_ivec2 dimensions);
void						make_t_landscape2(t_landscape *object);
void						destroy_t_landscape(t_landscape *object);

/*
** image stuff.
*/

struct						s_image
{
	void			*image;
	char			*ptr;
	int				bpp;
	int				stride;
	int				endian;
	t_ivec2			geometry;
	void			(*set_pixel)(struct s_image *this, t_ivec2 pos, int color);
	int				(*get_pixel)(struct s_image *this, t_ivec2 pos);
	void			(*clear)(struct s_image *this);
};

t_image						*make_t_image(void *mlx, t_ivec2 geometry);
void						destroy_t_image(t_image *object, void *mlx);

/*
** renderer stuff.
*/

struct						s_renderer
{
	void			*mlx;
	void			*window;
	t_ivec2			viewport_geometry;
	t_image			*buffer;
	float			*zbuffer;
	void			(*set_pixel)(struct s_renderer *this,
								 t_ivec2 point, float depth, int color);
	void			(*flush)(struct s_renderer *this);
	void			(*clear)(struct s_renderer *this);
	void			(*line)(struct s_renderer *this,
							t_vec4 one, t_vec4 two, t_ivec2 color);
	void			(*line_ortho)(struct s_renderer *this,
								  t_vec4 one, t_vec4 two, t_ivec2 color);
	void			(*plot_line)(struct s_renderer *this,
								 t_vec3 one, t_vec3 two, int color);
	void			(*plot_line_gradient)(struct s_renderer *this,
										  t_vec3 one, t_vec3 two, t_ivec2 color);
};

t_renderer					*make_t_renderer(void *mlx,
											   void *window, t_ivec2 geometry);
void						destroy_t_renderer(t_renderer *object);

/*
** main_loop stuff.
*/
struct						s_main_loop
{
	void			*mlx;
	void			*window;
	t_vec3			target_position;
	t_vec3			current_position;
	t_vec3			rotation;
	int				left_mouse_down;
	t_vec2			mouse_delta;
	t_vec2			prev_mouse_coord;
	t_ivec2			win_geometry;
	t_ivec2			gradient;
	float			target_radius;
	float			current_radius;
	t_renderer		*renderer;
	t_landscape		*landscape;
	t_mat4			mvp;
	t_mat4			projection;
	int				is_perspective;
	int				is_sphere;
	int				is_draw_text;
	float			landscape_multiplier;
	int				(*loop)(struct s_main_loop *this);
	void			(*run)(struct s_main_loop *this);
	int				(*render)(struct s_main_loop *this);
	int				(*on_key)(int key, struct s_main_loop *this);
	int				(*on_mouse)(int x, int y, struct s_main_loop *this);
	int				(*on_mouse_down)(int button, int x,
										int y, struct s_main_loop *this);
	int				(*on_mouse_up)(int button, int x,
									  int y, struct s_main_loop *this);
	int				(*on_close)(struct s_main_loop *this);
	void			(*render_as_plane)(struct s_main_loop *this);
	void			(*render_as_sphere)(struct s_main_loop *this);
	void			(*draw_text)(struct s_main_loop *this);
};

t_main_loop					*make_t_main_loop(char *title,
												 t_ivec2 geometry, t_landscape *landscape);
void						destroy_t_main_loop(t_main_loop *object);

/*
** vector stuff.
*/

struct						s_vector
{
	size_t			capacity;
	size_t			last_item_index;
	size_t			data_sizeof;
	void			*data;
	void			(*push)(struct s_vector *this, void *data);
	void			(*pop)(struct s_vector *this);
	void			(*erase)(struct s_vector *this);
	void			*(*at)(struct s_vector *this, size_t index);
	size_t			(*size)(struct s_vector *this);
};

void						ft_make_t_vector2(t_vector *object);
t_vector					*ft_make_t_vector(size_t capacity,
											  size_t data_sizeof);
void						ft_destroy_t_vector(t_vector *object);

struct						s_map_point
{
	t_ivec2			xy;
	float			z;
	int				color;
};

struct						s_data
{
	t_vector		*map_points;
	int				x_size;
	int				y_size;
};

/*
** Linear algebra.
**/
t_vec4						mat4_vec4_mul(const t_mat4 *mat, const t_vec4 *vec);
t_mat4						mat4_mat4_mul(const t_mat4 *m1, const t_mat4 *m2);
t_mat4						trans_mat(const float x, const float y, const float z);
t_mat4						scale_mat(const float x, const float y, const float z);
float						vec3_length(const t_vec3 vec);
t_vec3						vec3_normalize(const t_vec3 vec);
t_vec3						vec3_cross(const t_vec3 x, const t_vec3 y);
float						vec3_dot(const t_vec3 x, const t_vec3 y);
t_mat4						look_at(const t_vec3 eye, const t_vec3 center, const t_vec3 up);
t_mat4						orthographic(const t_vec4 bounds, const float near, const float far);

float						lerpf(float source, float target, float t);
int							lerpcolor(int source, int target, float t);
t_vec3						lerpvec3(t_vec3 source, t_vec3 target, float t);
int							rgb_to_int(t_vec3 rgb);
t_vec3						int_to_rgb(int color);
t_vec3						int_to_hsv(int color);
int							hsv_to_int(t_vec3 color);
float						clampf(float min, float max, float num);
int							get_color(char *f_color);
int							read_map(t_data *data, int fd);
int							get_color(char *f_color);
t_landscape					*process_input(char *filename);
void						init_data(t_data *data);
void						free_str_arr(char **arr);
int							process_points(char *points, t_map_point map_point,
											  t_data *data, t_ivec2 *coord);
int							process_line(char **points,
											t_data *data, t_ivec2 *coord);
void						image_to_bmp(t_image *image, char *filename);
void						image_to_bmp2(int fd, t_image *image);
void						write_repeatedly(int fd, uint8_t byte, size_t count);

#endif
