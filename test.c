#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

void	test_case(char *filename, int buffer_size)
{
	int		fd;
	char	*line;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("Error opening file");
		return ;
	}
	printf("Reading from file: %s\n", filename);
	printf("Buffer size: %d\n", buffer_size);
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	printf("\n");
}

int	main(void)
{
	// Normal file
	test_case("testfile.txt", 42);
	test_case("testfile.txt", 1);
	test_case("testfile.txt", 9999);
	test_case("testfile.txt", 10000000);
	// Empty file
	test_case("emptyfile.txt", 42);
	// File with a single character
	test_case("single_char.txt", 42);
	// File with no newline character
	test_case("no_newline.txt", 42);
	// File with very long lines
	test_case("long_lines.txt", 42);
	return (0);
}
