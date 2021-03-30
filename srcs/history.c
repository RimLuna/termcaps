#include "minishell.h"
#define vibe write(1, "v", 1);
void	append(char input, t_hlist *hist)
{
	char	*tmp;
	int		len;

	len = ft_strlen(hist->updated);
	tmp = hist->updated;
	hist->updated = (char *)malloc((len + 1) * sizeof(char));
	ft_bzero(hist->updated, len + 1);
	strncpy(hist->updated, tmp, len);
	hist->updated[len] = input;
	hist->updated[len + 1] = '\0';
	write(1, &input, 1);
	hist->llen++;
	free(tmp);
}

void	handle_input(t_hist *hist, char input[5])
{
	if (input[0] == 127 && hist->list->llen > 0
	&& write(1, "\b \b", 3))
		hist->list->updated[--hist->list->llen] = '\0';
	else if (input[2] == 65 && hist->list->prev)
	{
		del(hist->list->llen);
		hist->list = hist->list->prev;
		write(1, hist->list->updated, hist->list->llen);
	}
	else if (input[2] == 66 && hist->list->next)
	{
		del(hist->list->llen);
		hist->list = hist->list->next;
		write(1, hist->list->updated, hist->list->llen);
	}
	else if (ft_isprint(input[0]))
		append(input[0], hist->list);
}

void	hist_init(t_hlist *head, t_hlist **end)
{
	*end =
	dlist_push_back(&g_all->hist.list, (t_hlist *)malloc(sizeof(t_hlist)));
	g_all->hist.list = *end;
	(*end)->llen = 0;
	(*end)->cmd = NULL;
	(*end)->updated = NULL;
	if (g_all->hist.head == NULL)
		g_all->hist.head = g_all->hist.end;
	while (head)
	{
		head->updated = ft_strdup(head->cmd);
		head = head->next;
	}
}

void	hist_reset(t_hlist *head, int all)
{
	struct termios	restore;

	while (head)
	{
		free(head->updated);
		head->updated = NULL;
		head->llen = ft_strlen(head->cmd);
		head = head->next;
	}
	if (all)
	{
		write(1, "\n", 1);
		tcgetattr(0, &restore);
		restore.c_lflag |= (ICANON | ECHO);
		tcsetattr(0, TCSANOW, &restore);
	}
}

int		fail(t_hist *hist, int all)
{
	hist->list = hist->end->prev;
	if (hist->list)
		hist->list->next = NULL;
	ft_end((void **)&(hist->end->cmd), (void **)&(hist->end), 0);
	hist->end = hist->list;
	hist_reset(hist->head, 0);
	if (!hist->list)
		hist->head = NULL;
	if (all)
		hist_init(hist->head, &(hist->end));
	else
		write(1, "\n", 1);
	return (1);
}

int		readline(char **line, t_hist *hist)
{
	char			input[5];

	ft_bzero(input, 5);
	if (line == NULL)
		return (fail(hist, 1));
	hist_init(hist->head, &(hist->end));
	while ((g_all->parser.rt = read(0, &input, 4)) >= 0 && input[0] != '\n')
	{
		if (input[0] == 4 && hist->list->llen == 0)
			return (0);
		else
			handle_input(&g_all->hist, input);
		ft_bzero(input, 5);
	}
	*line = hist->list->updated ? ft_strdup(hist->list->updated) : ft_strdup("");
	if (ft_strlen(hist->list->updated) == 0)
		return (fail(hist, 0));
	hist->end->cmd = ft_strdup(hist->list->updated);
	hist_reset(hist->head, 1);
	return (1);
}
