/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   24_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonmar <antonmar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 12:16:16 by albzamor          #+#    #+#             */
/*   Updated: 2022/09/10 11:25:47 by antonmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sigint_handler(int sig)
{
	if ((sig == SIGINT || sig == SIGQUIT) && g_interactive != 2)
	{
		//printf("entra\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		printf("\n");
		rl_redisplay();
		errno = 1;
		return ;
	}
	else
	{
		printf("\33[2K\r");
		exit(1);
	}
}

void	sigquit_handler(int sig)
{
	char	*nbr;

	if (g_interactive == 0)
	{
		nbr = ft_itoa(sig);
		ft_putendl_fd(ft_strjoin("Quit: ", nbr), 2);
		g_interactive = 1;
		free(nbr);
		return ;
	}
	signal(SIGQUIT, SIG_IGN);
}

void	siguser_handler(int sig)
{
	(void)sig;
	printf("LA IGNORA\n");
	signal(SIGQUIT, SIG_IGN);
}

void	signal_handler(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	signal(SIGUSR1, siguser_handler);
}
