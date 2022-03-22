/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1b_search_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonmar <antonmar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 19:13:39 by antonmar          #+#    #+#             */
/*   Updated: 2022/03/22 19:34:33 by antonmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_quotes(char *line_walker, char quotes)
{
	char	*quotes_finder;

	quotes_finder = line_walker;
	if (*quotes_finder == quotes)
	{
		quotes_finder++;
		while (*quotes_finder)
		{
			if (*quotes_finder == quotes)
				return (1);
			quotes_finder++;
		}
	}
	return (0);
}

int	size_quotes_arg(char *line_walker, char quotes)
{
	int		i;
	char	*quotes_finder;

	i = 0;
	quotes_finder = line_walker;
	if (check_quotes(line_walker, quotes))
	{
		quotes_finder++;
		while (*quotes_finder)
		{
			if (*quotes_finder == quotes)
				return (i);
			i++;
			quotes_finder++;
		}
	}
	return (0);
}

char	check_allquotes(char *line_walker)
{
	char	*aux;

	aux = line_walker;
	if (check_quotes(aux, '\''))
		return ('\'');
	if (check_quotes(aux, '\"'))
		return ('\"');
	return (0);
}

char	jump_quotes(t_shell *shell)
{
	int		i;
	char	quotes;

	i = 0;
	quotes = check_allquotes(shell->line_walker);
	while (*shell->line_walker && quotes)
	{
		i = size_quotes_arg(shell->line_walker, quotes);
		if (*shell->line_walker && !i)
			shell->line_walker += 2;
		else if (i)
		{
			shell->line_walker++;
			return (quotes);
		}
		quotes = check_allquotes(shell->line_walker);
	}
	return (0);
}

char	jump_flag_quotes(char *flag_line)
{
	int		i;
	char	quotes;

	i = 0;
	quotes = check_allquotes(flag_line);
	while (*flag_line && quotes)
	{
		i = size_quotes_arg(flag_line, quotes);
		if (*flag_line && !i)
			flag_line += 2;
		else if (i)
		{
			flag_line++;
			return (quotes);
		}
		quotes = check_allquotes(flag_line);
	}
	return (0);
}

int	get_size_splitted_part(t_shell *shell, char quotes)
{
	int	size_command;

	size_command = 0;
	if (quotes)
	{
		while (*shell->line_walker && *shell->line_walker != quotes)
		{
			shell->line_walker++;
			size_command++;
		}
		shell->line_walker++;
	}
	else
	{
		while (*shell->line_walker && *shell->line_walker != ' '
			&& !check_allquotes(shell->line_walker))
		{
			shell->line_walker++;
			size_command++;
		}
	}
	return (size_command);
}

char	*get_command_part(t_shell *shell)
{
	int		size_command;
	char	*start_command;
	char	*command;
	char	quotes;
	char	*free_command;

	quotes = jump_quotes(shell);
	start_command = shell->line_walker;
	size_command = get_size_splitted_part(shell, quotes);
	command = ft_substr(start_command, 0, size_command);
	while (*shell->line_walker && *shell->line_walker != ' ')
	{
		size_command = 0;
		start_command = shell->line_walker;
		quotes = jump_quotes(shell);
		size_command = get_size_splitted_part(shell, quotes);
		start_command = ft_substr(start_command, 0, size_command);
		free_command = command;
		command = ft_strjoin(command, start_command);
		free(free_command);
		free(start_command);
	}
	while (*shell->line_walker && *shell->line_walker != ' ')
		shell->line_walker++;
	return (command);
}

int	check_flag(t_shell *shell)
{
	char	*flag;
	char	*no_flag;

	no_flag = shell->line_walker;
	while (*shell->line_walker && *shell->line_walker == ' ')
		shell->line_walker++;
	flag = get_command_part(shell);
	if (!ft_strncmp(flag, "-n", 2))
	{
		flag += 1;
		while (*flag && *flag == 'n')
			flag++;
		if (!*flag)
		{
			flag = "-n";
			shell->command_flag = flag;
			return (1);
		}
	}
	shell->command_flag = NULL;
	shell->line_walker = no_flag;
	return (0);
}
/* FUNCION DE COMPROBACIÓN DE COMILLAS SIN CERRAR
int check_correct_quotes(char *line)
{
    char    *line_aux;
    char    *line_back;
    char    quotes;
    int     num_slash;

    line_aux = line;
    while (*line_aux)
    {
        num_slash = 0;
        quotes = jump_flag_quotes(line_aux);
        if (check_allquotes(line_aux) && size_quotes_arg(line_aux, *line_aux) == 0)
            line_aux++;
        else if (quotes)
        {
            line_back = line_aux;
            line_back--;
            while (*line_back && *line_back == '\\')
            {
                line_back--;
                num_slash++;
            }
            if (num_slash % 2 == 1)
                return (0);
            line_aux++;
            while (*line_aux != quotes)
                line_aux++;
        }
        else if (!quotes && (*line_aux == '\"' || *line_aux == '\''))
        {
            line_back = line_aux;
            line_back--;
            while (*line_back && *line_back == '\\')
            {
                line_back--;
                num_slash++;
            }
            if (num_slash % 2 == 0)
                return (0);
        }
        line_aux++;
    }
    return (1);
}
*/
void	start_command(t_shell *shell)
{
	shell->line = readline(BLUE"AlicornioPrompt$ "RESET);
	if (shell->line && *shell->line)
		add_history(shell->line);
	shell->line_walker = shell->line;
	while (*shell->line_walker && *shell->line_walker == ' ')
		shell->line_walker++;
}

int	add_command(t_shell *shell)
{
	char		*aux;
	int			i;

	i = 0;
	start_command(shell);
	aux = shell->line_walker;
	if (!jump_quotes(shell)
		&& (!*shell->line_walker || *shell->line_walker == ' '))
		return (-1);
	shell->line_walker = aux;
	shell->command = get_command_part(shell);
	while (i < shell->size_c
		&& ft_strcmp(shell->command, shell->list_commands[i]))
		i++;
	if (i >= shell->size_c)
		return (-1);
	if (!ft_strcmp(shell->list_commands[i], "echo"))
		check_flag(shell);
	while (*shell->line_walker && *shell->line_walker == ' ')
		shell->line_walker++;
	shell->line_args = shell->line_walker;
	return (0);
}

void	add_arg(t_shell *shell, char *start_arg, int size_prev)
{
	t_arglist	*this_arg;

	if (size_prev > 0 && *start_arg)
	{
		start_arg = ft_substr(start_arg, 0, size_prev);
		this_arg = arg_node_new(start_arg);
		arglstadd_back(&shell->arg_list, this_arg);
	}
}

int	get_size_splitted_argpart(t_shell *shell, char quotes)
{
	int	size_command;

	size_command = 0;
	if (quotes)
	{
		shell->line_walker++;
		size_command++;
		while (*shell->line_walker && *shell->line_walker != quotes)
		{
			shell->line_walker++;
			size_command++;
		}
		shell->line_walker++;
		size_command++;
	}
	else
	{
		while (*shell->line_walker && *shell->line_walker != ' '
			&& !check_allquotes(shell->line_walker))
		{
			shell->line_walker++;
			size_command++;
		}
	}
	return (size_command);
}

char	jump_arg_quotes(t_shell *shell)
{
	int		i;
	char	quotes;

	i = 0;
	quotes = check_allquotes(shell->line_walker);
	while (*shell->line_walker && quotes)
	{
		i = size_quotes_arg(shell->line_walker, quotes);
		if (*shell->line_walker && !i)
			shell->line_walker += 2;
		else if (i)
			return (quotes);
		quotes = check_allquotes(shell->line_walker);
	}
	return (0);
}

char	*get_arg_parts(t_shell *shell, char *argument)
{
	int			size_arg;
	char		*arg_start;
	char		quotes;
	char		*free_argument;

	while (*shell->line_walker && *shell->line_walker != ' ')
	{
		size_arg = 0;
		quotes = jump_arg_quotes(shell);
		arg_start = shell->line_walker;
		size_arg = get_size_splitted_argpart(shell, quotes);
		arg_start = ft_substr(arg_start, 0, size_arg);
		if (check_allquotes(arg_start) != '\'')
			arg_start = change_dollars(shell, arg_start);
		if (check_allquotes(arg_start))
			arg_start = del_quotes(arg_start);
		quotes = jump_flag_quotes(arg_start);
		free_argument = argument;
		argument = ft_strjoin(argument, arg_start);
		free(arg_start);
	}
	return (argument);
}

int	argument_list_creator(t_shell *shell)
{
	char		*arg_start;
	char		*argument;
	char		quotes;

	quotes = jump_arg_quotes(shell);
	arg_start = shell->line_walker;
	argument = ft_substr(arg_start, 0,
			get_size_splitted_argpart(shell, quotes));
	if (check_allquotes(argument) != '\'')
		argument = change_dollars(shell, argument);
	if (check_allquotes(argument))
		argument = del_quotes(argument);
	arglstadd_back(&shell->arg_list,
		arg_node_new(get_arg_parts(shell, argument)));
	if (!(*shell->line_walker))
		return (0);
	else
	{
		shell->line_walker++;
		return (1);
	}
	shell->line_walker++;
	return (1);
}

int	split_arguments(t_shell *shell)
{
	t_arglist	*printer;
	int			i;

	i = 1;
	printer = NULL;
	shell->size_args = 0;
	if (*shell->line_walker)
		shell->size_args = 1;
	while (argument_list_creator(shell))
		shell->size_args++;
	shell->command_args = malloc(sizeof(char *) * shell->size_args);
	printer = shell->arg_list;
	shell->line_walker = shell->line_args;
	while (shell->arg_list)
	{
		shell->command_args[i] = shell->arg_list->content;
		shell->arg_list = shell->arg_list->next;
		i++;
	}
	shell->arg_list = printer;
	return (0);
}

char	*del_quotes(char *str_to_del_quotes)
{
	char	*no_quotes;

	if (!str_to_del_quotes)
		return (NULL);
	no_quotes = ft_substr(str_to_del_quotes, 1,
			ft_strlen(str_to_del_quotes) - 2);
	free (str_to_del_quotes);
	return (no_quotes);
}
