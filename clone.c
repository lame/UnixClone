/*	
Ryan Kuhl
COP4610
Assignment 4
4/20/2014
Clone Utility 
*/


// (1) If the source directory doesn't exist, print an error.

// (2) If the destination directory doesn't exist, create it.

// (3) Copy all files / directories in the source directory 
// to the destination directory (recursively). Print a message for each file / directory copied.

// (4) Set permissions, owner, and group for each destination file / directory to that of the 
// source file / directory. Print a message for each file / directory attribute changed.

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define _GNU_SOURCE
#define MAX_NUM 99999
#define COPYMORE 0644


int Dir(char *sourcepath, char* destpath)
{
	struct stat s;
	struct dirent *d;
	DIR *ptr=NULL;

	char tempDestination[PATH_MAX+1];
	char tempSource[PATH_MAX+1];

	strcpy(tempDestination, destpath);
	strcpy(tempSource, sourcepath);

	if((ptr = opendir(sourcepath)) == NULL)
	{
		fprintf(stderr, "Error: Unable to open file %s\n", sourcepath);
	}
	else
	{
		while((d = readdir(ptr)))
		{
			// printf("d DNAME is %s\n", d->d_name);
			// file = p->d_name;
			// printf("File name before Reg Check: %s\n", file);
			if(Stat(d->d_name))
			{
				strcat(tempDestination, d->d_name);
				strcat(tempSource, d->d_name);
				File(tempSource, tempDestination);
				strcpy(tempDestination, destpath);
				strcpy(tempSource, sourcepath);
			}
		}
		closedir(ptr);
		return (1);
	}
}

int File(char* sourcepath, char* destpath)
{
	int in, out, chars;
	char buffer[1025];

	if((in=open(sourcepath, O_RDONLY)) == -1)
	{
		fprintf(stderr, "Error: Cannot open %s\n", sourcepath);
	}
	if((out=creat(destpath, COPYMORE)) == -1)
	{
		fprintf(stderr, "Error: Cannot Create %s\n", destpath);
	}
	while((chars = read(in, buffer, 1025)) >0)
	{
		if(write(out, buffer, chars) != chars)
		{
			fprintf(stderr, "Error: Write error from %s\n", sourcepath);
		}
		if(chars == -1)
		{
			fprintf(stderr, "Error: Read error from %s\n", destpath);
		}
	}

	if(close(in) == -1 || close(out) == -1)
	{
		fprintf(stderr, "Error: Unable to close files.\n");
	}
}

int Stat(char* file)
{
	struct stat s;
	if(stat(file, &s) >= 0)
	{
		if(S_ISREG(s.st_mode))
		{
			return(1);
		}
		else
		{
			return 0;
		}
	}
	return;
}

int main(int argc, char *argv[])
{

	char *source;
	char *destination;
	char *sourceptr;
	char *destptr;
	char *staticptr;
	
	char sourcepat[PATH_MAX+1];
	char destpath[PATH_MAX+1];
	char staticDest[PATH_MAX+1];

	struct stat s;

	if(argc != 3)
	{
		fprintf(stderr, "Error: Usage is <clone.c> <source> <destination>\n");
		exit(1);
	}
	
	/* ABSOLUTE PATH FOR ALL INPUT */
	/*===============================*/
	source = argv[1];
	destination = argv[2];

	sourceptr = realpath(source, sourcepath);
	destptr = realpath(destination, destpath);
	staticptr = realpath(destination, destpath);

	printf("SourcePtr: %s\n", sourceptr);
	printf("DourcePtr: %s\n", destptr);
	printf("StaticPtr: %s\n", staticptr);

	/*===============================*/

	// Check is available

	if (chdir(source))
	{
		fprintf(stderr, "Error: %s: No such file or directory.\n", source);
		exit(1);
	}

	if( stat(source,&s) == 0 )
	{
		if( s.st_mode & S_IFDIR )
		{
			printf("%s is a directory", source);
		}
		else if( s.st_mode & S_IFREG )
		{
			printf("%s is a file", source);
		}
	}
	else
	{
		fprintf(stderr, "Error: Stat did not recognize input type\n");
		exit(1);
	}


	sourcepath = realpath(source, NULL);


	return 0;
}