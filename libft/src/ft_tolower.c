/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arraji <arraji@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 01:06:20 by arraji            #+#    #+#             */
/*   Updated: 2019/10/19 02:15:15 by arraji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_tolower(int car)
{
	if (car >= 65 && car <= 90)
		return (car + 32);
	else
		return (car);
}