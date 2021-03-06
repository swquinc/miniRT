/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: swquinc <swquinc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/06 22:31:10 by swquinc           #+#    #+#             */
/*   Updated: 2020/11/01 01:12:52 by swquinc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	create_camera(t_scene *scene)
{
	t_camera	*c;
	t_bilist	*tmp;

	tmp = scene->cam_list;
	while (tmp)
	{
		c = tmp->content;
		if (!(c->image.img =
		mlx_new_image(scene->mlx, scene->res.x, scene->res.y)))
			errors_handler(MLX_NEW_IMAGE_FAIL, scene);
		c->image.addr = mlx_get_data_addr(c->image.img,
		&c->image.bits_per_pixel, &c->image.line_lenght, &c->image.endian);
		raytrace(c, scene);
		if (!scene->camera.image.img)
			scene->camera = *c;
		if (scene->save)
			return ;
		tmp = tmp->next;
	}
}

void	next_camera(t_scene *scene)
{
	t_camera	*cam;

	if (!scene->cam_list->next)
		return ;
	cam = scene->cam_list->next->content;
	scene->camera = *cam;
	scene->cam_list = scene->cam_list->next;
}

void	prev_camera(t_scene *scene)
{
	t_camera	*cam;

	if (!scene->cam_list->prev)
		return ;
	cam = scene->cam_list->prev->content;
	scene->camera = *cam;
	scene->cam_list = scene->cam_list->prev;
}
