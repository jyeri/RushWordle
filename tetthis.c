#include "header.h"

//ToDo
//Fiksaa filtteri vielä niin, että kaksi samaa kirjainta toimii.


void error(void)
{
	printf("!!!!!!!!!!ERROR!!!!!!!!!!!\n");
	printf("!!!Pilluminati cuntroll!!!\n");
	printf("!!!!!!!!!!!11!!!!!!!!!!!!!\n");
	exit(0);
}

void getinput(char *buffer)
{
	char inputbuf[6];
	int		scanner;

	//This checks if the input is correct
	printf("Input the characters here:");
	scanner = scanf("%5s", inputbuf);
	if (scanner != 1)
		error();
	scanner = strlen(inputbuf);
	if (scanner != 5)
	{
		if (inputbuf[0] == 'Q' && inputbuf[1] == '\0')
		{
			printf("Thanks for playing!\n");
			exit(0);
		}
		error();
	}
	printf("Your input was: %s\n", inputbuf);
	int i = 0;
	while (i < 6)
	{
		buffer[i] = inputbuf[i];
		i++;
	}
}

t_node *create_node(t_node **head, char *thisone)
{
	t_node *tmp = malloc(sizeof(t_node));
	t_node *last = *head;

	tmp->word = strdup(thisone);
	tmp->next = NULL;
	if(*head == NULL)
	{
		*head = tmp;
		return (*head);
	}
	while (last->next != NULL)
		last = last->next;
	last->next = tmp;
	return (*head);
}

void print_list(t_node *node)
{
	int i = 0;
	printf("\nPossible options for the word:\n");
	if(node->next != NULL)
	{
		while (node != NULL)
		{
			printf("%s", node->word);
			i++;
			if (i == 6)
			{
				printf("\n");
				i = 0;
			}
			else
				printf("\t");
			node = node->next;
		}
	}
	else
		printf("%s\n", node->word);
	printf("\n");
}

void delete_node(t_node **head, char *sana)
{
	t_node *tmp = NULL;
	t_node *current;

	current = *head;
	if (current->word == sana)
	{
		tmp = current;
		current = current->next;
		(*head) = current;
		free(tmp);

	}
	else
	{
		while (current->next != NULL)
		{
			if (current->next->word == sana)
			{
				if (current->next)
					tmp = current->next;
				if (current->next->next)
					current->next = current->next->next;
				else
					current->next = NULL;
				free(tmp);
				break;
			}
			else
				current = current->next;
		}
	}
}

int comparenode(char *data, char *restrictions)
{
	int j = 0;
	while (restrictions[j] != '\0')
	{
		if (restrictions[j] == '.')
			j++;
		else if (restrictions[j] >= 'A' && restrictions[j] <= 'Z')
		{
			if (data[j] != tolower(restrictions[j]))
				return 0;
			j++;
		}
		else if (restrictions[j] >= 'a' && restrictions[j] <= 'z')
		{
			if (!(strchr(data, restrictions[j])))
				return 0;
			if (data[j] == restrictions[j])
				return 0;
			j++;
		}
		else
			error();
	}
	return 1;
}

void	check_list(t_node *head, char *input)
{
	t_node *last = head;
	t_node *tmp;

	tmp = malloc(sizeof(t_node));
	while (last->next != NULL)
	{

		if (comparenode(last->word, input) != 1)
		{
			tmp = last->next;
			delete_node(&head, last->word);
			last = tmp;
		}
		if (last->next)
			last = last->next;
		else
			break;
	}
	if (comparenode(last->word, input) != 1)
	{
		tmp = last->next;
		delete_node(&head, last->word);
		last->next = tmp;
	}
}

t_node	*listcreator(int fd, char *word)
{
	char	readbuf[6];
	int	reader = 0;
	t_node	*head = NULL;

	reader = read(fd, readbuf, 6);
	if (reader < 0)
		error();
	while (reader != 0)
	{
		readbuf[5] = '\0';
		if (comparenode(readbuf, word))
		{
			create_node(&head, readbuf);
		}
		reader = read(fd, readbuf, 6);
	}
	return(head);
}

int	main(int argc, char **argv)
{
	int		fd;
	t_node *list;
//	t_node *current = NULL;
	char	input[6];

	if (argc != 2)
	{
		printf("usage: ./helper words.txt\n");
		return (1);
	}
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		error();


	printf("Wordle helper instructions:\n");
	printf("Give wordle your first guess.\n");
	printf("Use the following commands to input the result:\n");
	printf("Big letter marks a correct spot.\n");
	printf("Small letter is a correct letter in an incorrect spot.\n");
	printf("Use . (period) to mark a wrong letter.\n\n");
	printf("Example:\n");
	printf("Correct word: CANDY\n");
	printf("Word you guessed: CRANE\n");
	printf("Ansver to input: C.an.\n");
	printf("\n\n\n\n\n");


	getinput(input);
	list = listcreator(fd, input);
	print_list(list);


	getinput(input);
	check_list(list, input);
	print_list(list);

	getinput(input);
	check_list(list, input);
	print_list(list);

	getinput(input);
	check_list(list, input);
	print_list(list);

	getinput(input);
	check_list(list, input);
	print_list(list);

	/*int rounds = 1;
	while (rounds <= 6)
	{

	}*/
	close(fd);
	return (0);
}
