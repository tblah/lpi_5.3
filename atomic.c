#include <stdio.h>		// for printf and fflush
#include <errno.h>		// for errno
#include <stdlib.h>		// for exit

// program to investigate the importance of the atomicity of system calls: specifically a write to a file opened with the O_APPEND flag.
// atomic_append filename num-bytes [x]
// appends num-bytes to filename (creating it if nessecary). If x is specified then O_APPEND should be replaced with a call to lseek before every write.

void errExit(char* errMsg)
{
	int errorno = errno;

	printf("Exiting on error:\t%s\n", errMsg);
	printf("\n\nThe errno was %i\n", errorno);
	
	fflush(stdout);

	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
	

	exit(EXIT_SUCCESS);
}
