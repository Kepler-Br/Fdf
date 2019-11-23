/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsena <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/21 18:49:47 by nsena             #+#    #+#             */
/*   Updated: 2019/10/21 18:49:50 by nsena            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int main(int argc, char **argv)
{
	t_landscape *land;
	t_main_loop *main_loop;

	if (argc == 1)
	{
		ft_putendl("Usage: ./fdf [filename]");
		return (0);
	}
	if (argc > 2)
	{
		ft_putendl("Too many arguments.");
		return (0);
	}
	land = process_input(argv[1]);
	main_loop = make_t_main_loop("SNAFU team FDF", (t_ivec2){1000, 1000}, land);
	if (!main_loop)
		return (0);
	main_loop->run(main_loop);
//	void			*mlx = mlx_init();
//	t_image *img = make_t_image(mlx, (t_ivec2){1000, 1000});
//	img->clear(img);
//	image_to_bmp(img, "screenshot.bmp");
	return 0;
}
