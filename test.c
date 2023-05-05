#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

void	test_case(char *filename)
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
	printf("Buffer size: %d\n", BUFFER_SIZE);
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("LINE_RETURNED=%s", line);
		free(line);
	}
	close(fd);
}

int	main(void)
{
	// Normal file
	test_case("simplefile.txt");
	test_case("gnlTester/files/alternate_line_nl_with_nl");
	// // Empty file
	test_case("emptyfile.txt");
	// File with a single character
	test_case("single_char.txt");
	// File with no newline character
	test_case("no_newline.txt");
	// File with very long lines
	test_case("long_lines.txt");
	return (0);
}
