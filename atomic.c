#include <stdio.h>		// for printf and fflush
#include <errno.h>		// for errno
#include <stdlib.h>		// for exit
#include <string.h>		// for strcmp
#include <sys/types.h>	// for open and lseek
#include <sys/stat.h>	// for open
#include <fcntl.h>		// for open
#include <unistd.h>		// for close, write and lseek
#include <stdlib.h>		// for strtol

// program to investigate the importance of the atomicity of system calls: specifically a write to a file opened with the O_APPEND flag.
// atomic_append filename num-bytes [x]
// appends num-bytes to filename (creating it if nessecary). If x is specified then O_APPEND should be replaced with a call to lseek before every write.

typedef enum {false, true} bool;

void errExit(char* errMsg)
{
	int errorno = errno;

	printf("Exiting on error:\t%s\n", errMsg);
	printf("\n\nThe errno was %i\n", errorno);
	
	fflush(stdout);

	exit(EXIT_FAILURE);
}

long int getLong(char string[]) { // gets a long from a string
	char *firstCharNotNumber;	// will be set by strtol to the first charachter in the input which is not a number
	long int result;	// stores the result of the conversion

	errno = 0;	// so that we can check if it was changed by strtol

	result = strtol(string, &firstCharNotNumber, 10); 	// assume that input will be in base 10

	if (errno != 0)			// check to see if strtol experienced an error
		errExit("strtol");

	if (*firstCharNotNumber != '\0')	// if the string contained invalid charichters
		errExit("strtol: Expected base 10 number, got something else");

	return result;
}

void writeAppend(char filename[], long int numBytes, bool atomic) {	// this function does all of the work
	int openFlags = O_CREAT | O_WRONLY;

	if (atomic == true)
		openFlags |= O_APPEND;

	int fd = open(filename, openFlags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH); // rw-rw-r--
	if (fd == -1)
		errExit("open");

	// write numBytes bytes to the file
	char dummy = 'a';	// dummy data to be written to the file

	for (int i = 0; i < numBytes; i++) {
		// if we are allowing the race condition then we lseek before we write
		if (atomic == false) {
			if (lseek(fd, 0, SEEK_END) == -1)
				errExit("lseek");
		}

		// write one byte of dummy data to the file
		if (write(fd, &dummy, 1) == -1)
			errExit("write");
	}

	if (close(fd) == -1)
		errExit("close");

}

int main(int argc, char* argv[])
{
	// deal with command line arguements
	if (argc == 4 && strcmp(argv[3], "x") == 0) {
		writeAppend(argv[1], getLong(argv[2]), false);
	}
	else if (argc == 3) {
		writeAppend(argv[1], getLong(argv[2]), false);
	}
	else {
		printf("USAGE: %s filename num-bytes [x]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
