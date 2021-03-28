#include "minishell.h"
#include <termcap.h>
#include <termios.h>
#include <term.h>

#define DELETE 127
#define T_UP 65
#define T_DOWN 66

typedef	struct	s_hist
{
	struct s_hist	*next;
	struct s_hist	*prev;
	struct s_hist	*end;
	struct s_hist	*current;
	char			*cmd;
}				t_hist;


int		ft_isblank(int c)
{
	return (c == ' ' || c == '\t' ? 1 : 0);
}

char *
ft_strcat(char *s1, const char *s2)
{
	int i;
	int j;

	i = 0;
	while (s1[i] != '\0')
	{
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
	{
		s1[i] = s2[j];
		i++;
		j++;
	}
	s1[i] = '\0';
	return (s1);
}

int
set_state(void)
{
	struct termios		term;

	if (tgetent(0, get_var_value("TERM")) < 1)
		return (0);
	tcgetattr(0, &term);
	term.c_lflag &= ~(ICANON | ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &term);
	return (1);
}

void
restore_state(void)
{
	struct termios		restore;

	tcgetattr(0, &restore);
	restore.c_lflag |= (ICANON | ECHO);
	tcsetattr(0, TCSANOW, &restore);
}

int
putchr(int c)
{
	write(1, &c, 1);
	return (0);
}

void
clear_line()
{
	tputs(tgetstr("cr", NULL), 1, &putchr);
	tputs(tgetstr("cd", NULL), 1, &putchr);
	ft_fprintf(1, "%s%s%s%s", BOLD, PRINT_GR, PS, RESET);
}

void
delete(t_readline *data)
{
	int len;
	char *tmp;

	len = ft_strlen(data->line);
	if (len == 0)
		return ;
	clear_line();
	data->line[len - 1] = '\0';
	ft_putstr_fd(data->line, 1);
}

void
append(t_readline *data)
{
	char *tmp;
	int len;

	len = ft_strlen(data->line);
	tmp = data->line;
	data->line = (char *)malloc((len + 1) * sizeof(char));
	ft_bzero(data->line, len + 1);
	strncpy(data->line, tmp, len);
	data->line[len] = data->input[0];
	data->line[len + 1] = '\0';
	write(1, data->input, 1);
	// free(tmp);
}

void
history_add(t_hist *history, char *line)
{
	if (!history->cmd)
	{
		history->cmd = ft_strdup(line);
		return ;
	}
	else if (!history->next)
	{
		history->next = (t_hist*)malloc(sizeof(t_hist));
		history->next->next = NULL;
		history->next->prev = history;
		history->end = history->next;
		history->next->cmd = ft_strdup(line);
		return ;
	}
	history->end->next = (t_hist*)malloc(sizeof(t_hist));
	history->end->next->next = NULL;
	history->end->next->prev = history->end;
	history->end->next->cmd = ft_strdup(line);
	history->end = history->end->next;
}

void
print_hist(t_hist *head)
{
	t_hist *tmp;
	if (!head->cmd)
		return ;
	tmp = head;
	printf("\n\nHIST:\n\n");
	while (tmp)
	{
		printf("%s\n", tmp->cmd);
		tmp = tmp->next;
	}
	ft_putstr_fd("\n", 1);
}

void
history_init(t_hist *history)
{
	history->cmd = NULL;
	history->current = NULL;
	history->end = NULL;
	history->next = NULL;
	history->prev = NULL;
}

void
hist_down(t_readline *data, t_hist *history)
{
	clear_line();
	if (!history->current || !history->current->cmd)
		history->current = history;
	else if (history->current->cmd)
		history->current = history->current->next;
	if (history->current && history->current->cmd)
	{
		ft_bzero(data->line, ft_strlen(data->line));
		write(1, history->current->cmd, ft_strlen(history->current->cmd));
		ft_strcpy(data->line, history->current->cmd);
	}
	else
		ft_bzero(data->line, ft_strlen(data->line));
}

void
hist_up(t_readline *data, t_hist *history)
{
	clear_line();
	if (!history->current || !history->current->cmd)
		history->current = history->end;
	else if (history->current->cmd)
		history->current = history->current->prev;
	if (history->current && history->current->cmd)
	{
		write(1, history->current->cmd, ft_strlen(history->current->cmd));
		ft_bzero(data->line, ft_strlen(data->line));
		ft_strcpy(data->line, history->current->cmd);
	}
	else
		ft_bzero(data->line, ft_strlen(data->line));
}

char*
readline()
{
	t_hist			history;
	t_readline		data;

	if (!set_state())
		return (NULL);
	history_init(&history);
	ft_bzero(&data.input, 5);
	data.line = NULL;
	while (1)
	{
		while (read(0, &data.input, 4) && data.input[0] != '\n')
		{
			// printf("%d %d %d %d", data.input[0], data.input[1], data.input[2], data.input[3]);
			if (data.input[0] == DELETE)
				delete(&data);
			else if (data.input[2] == 65)
				hist_up(&data, &history);
			else if (data.input[2] == 66)
				hist_down(&data, &history);
			else if (ft_isprint(data.input[0]))
			{
				append(&data);
			}
			ft_bzero(data.input, 5);
		}
		ft_putchar_fd('\n', 1);
		if (strncmp(data.line, "exit", 4) == 0)
			exit(0);
		history_add(&history, data.line);
		print_hist(&history);
		ft_bzero(data.line, ft_strlen(data.line));
		ft_fprintf(1, "%s%s%s%s", BOLD, PRINT_GR, PS, RESET);
	}
	restore_state();
}
