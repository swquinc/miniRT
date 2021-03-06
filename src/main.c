/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: swquinc <swquinc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 16:15:19 by hovalygta         #+#    #+#             */
/*   Updated: 2020/11/01 00:38:21 by swquinc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int		kill(t_scene *scene)
{
	mlx_destroy_window(scene->mlx, scene->win_ptr);
	free_all(scene);
	exit(EXIT_SUCCESS);
}

static int		cam_switch(int key, t_scene *scene)
{
	if (key == 53)
		kill(scene);
	else if (key == 124)
		next_camera(scene);
	else if (key == 123)
		prev_camera(scene);
	mlx_put_image_to_window(scene->mlx, scene->win_ptr,
	scene->camera.image.img, 0, 0);
	return (0);
}

void			my_mlx_pixel_put(t_image *data, int x, int y, int color)
{
	char		*dst;

	dst = data->addr + (y * data->line_lenght + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void			start_rt(t_scene *s)
{
	if (!(s->mlx = mlx_init()))
		errors_handler(MINILIBX_FAIL, s);
	s->win_ptr = mlx_new_window(s->mlx, s->res.x, s->res.y, "miniRT");
	create_camera(s);
	if (s->save)
	{
		create_bmp(s);
		free_all(s);
		exit(EXIT_SUCCESS);
	}
	mlx_hook(s->win_ptr, 17, 0, kill, s);
	mlx_key_hook(s->win_ptr, cam_switch, s);
	mlx_put_image_to_window(s->mlx, s->win_ptr, s->camera.image.img, 0, 0);
	mlx_loop(s->mlx);
}

int				main(int argc, char **argv)
{
	t_scene		scene;
	char		*rt;

	ft_bzero(&scene, sizeof(t_scene));
	if ((argc != 2 && argc != 3) ||
	(argc == 3 && (ft_strncmp(argv[2], "--save", 6) != 0)))
		errors_handler(WRONG_ARGS, &scene);
	rt = argv[1] + ft_strlen(argv[1]) - 3;
	if (ft_strncmp(rt, ".rt", 3) != 0)
		errors_handler(WRONG_EXTENSION, &scene);
	if (argc == 3)
		scene.save = 1;
	parsing(argv[1], &scene);
	start_rt(&scene);
	free_all(&scene);
	return (0);
}
