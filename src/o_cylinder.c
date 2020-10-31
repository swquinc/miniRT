/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_cylinder.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: swquinc <swquinc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 01:03:48 by swquinc           #+#    #+#             */
/*   Updated: 2020/10/31 05:46:09 by swquinc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	cap(t_plane plane, t_xyz origin, t_xyz ray)
{
	double	a;
	double	b;
	double	t;

	a = vec_dot(vec_sub(origin, plane.coord), plane.orient);
	b = vec_dot(ray, plane.orient);
	if (b == 0 || (a < 0 && b < 0) || (a > 0 && b > 0))
	 	return (__INT_MAX__);
	t = -a / b;
	if (t < 0)
		return (__INT_MAX__);
	return (t);
}

static int	cylinder1(t_xyz ray, double t1, double t2, t_cylinder cy, t_camera *cam)
{
	double	t;
	double	check;
	double	check2;
	t_xyz	origin;
	t_plane	cy_plane;

	cy_plane.orient = cy.orient;
	cy_plane.coord = cy.coord;
	t = -1;
	if (t1 < 0 && t2 >= 0)
		t = t2;
	else if (t2 < 0 && t1 >= 0)
		t = t1;
	else if (t1 >= 0 && t2 >= 0)
		t = fmin(t1, t2);
	else if (t < 0)
		return (-1);
	origin = vec_sum(cam->coord, vec_mpl(ray, t));
	check = cap(cy_plane, origin, cy.orient);
	check2 = cap(cy_plane, origin, vec_mpl(cy.orient, -1));
	if (check > cy.height / 2 && check2 > cy.height / 2)
		return (-1);
	return (t);
}
int		cylinder(t_scene *scene, t_camera *cam, t_xyz ray, t_cylinder cy)
{
	double	t;
	double	discr;
	t_xyz	vec;
	t_xyz	vec2;
	t_xyz	eq;

	// vec = vec_sub(ray, vec_mpl(cy.orient, vec_dot(ray, cy.orient)));
	// vec2 = vec_sub(vec_sub(cam->coord, cy.coord), vec_mpl(cy.orient, vec_dot(vec_sub(cam->coord, cy.coord), cy.orient)));
	vec = vec_cross(ray, cy.orient);
	vec2 = vec_sub(cam->coord, cy.coord);
	vec2 = vec_cross(vec2, cy.orient);
	eq.x = vec_dot(vec, vec);
	eq.y = 2 * vec_dot(vec, vec2);
	eq.z = vec_dot(vec2, vec2) - (pow(cy.diam / 2, 2) * vec_dot(cy.orient, cy.orient));
	discr = pow(eq.y, 2) - (4 * eq.x * eq.z);
	if (discr < 0)
		return (0);
	discr = sqrt(discr);
	t = (-eq.y + discr) / (eq.x * 2);
	eq.z = (-eq.y - discr) / (eq.x * 2);
	if ((t = cylinder1(ray, t, eq.z, cy, cam)) < 0)
		return (0);
	scene->mhave.dist2 = t;
	scene->mhave.cur = cy.rgb;
	return (1);
}
