/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sazelda <sazelda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 11:50:57 by sazelda           #+#    #+#             */
/*   Updated: 2022/01/21 11:50:58 by sazelda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static void	go_to(const char *str, int *i, int *znak)
{
	while (str[*i] == ' ' || str[*i] == '\t' || str[*i] == '\n' \
			|| str[*i] == '\v' || str[*i] == '\f' || str[*i] == '\r')
		*i = *i + 1;
	if (str[*i] == '-')
	{
		*znak = -1;
		*i = *i + 1;
	}
	else if (str[*i] == '+')
		*i = *i + 1;
}

int	ft_atoi(const char *str)
{
	long int	res;
	int			i;
	int			znak;

	res = 0;
	i = 0;
	znak = 1;
	go_to(str, &i, &znak);
	while (str[i] >= '0' && str[i] <= '9')
	{	
		if (znak * res < -2147483648)
			return (0);
		else if (res > 2147483647)
			return (-1);
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return ((int)res * znak);
}
