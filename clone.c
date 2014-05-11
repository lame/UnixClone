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

int IsFile(char* file)
{
  struct stat info;
  return (!stat(file, &info) && S_ISREG(info.st_mode));
}

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
		printf("Creating directory %s\n", destpath);
		while((d = readdir(ptr)))
		{
			if(IsFile(d->d_name))
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

int File(char* sourceptr, char* destptr)
{
	int in, out, chars;
	char buffer[1025];
	struct stat fst;

	stat(sourceptr,&fst);

	if((in=open(sourceptr, O_RDONLY)) == -1)
	{
		fprintf(stderr, "Error: Cannot open %s\n", sourceptr);
	}
	if((out=creat(destptr, COPYMORE)) == -1)
	{
		fprintf(stderr, "Error: Cannot Create %s\n", destptr);
	}

	printf("Copying %s to %s\n", sourceptr, destptr);
	
	chmod(destptr,fst.st_mode);
	printf("Setting permissions for %s: %d\n", destptr, (unsigned int)fst.st_mode);
	chown(destptr,fst.st_uid,fst.st_gid);
	printf("Setting user and group for %s: %d, %d\n", destptr, (unsigned int)fst.st_uid,(unsigned int)fst.st_gid);
	
	while((chars = read(in, buffer, 1025)) >0)
	{
		printf("\nIn While loop\n");
		if(write(out, buffer, chars) != chars)
		{
			fprintf(stderr, "Error: Write error from %s\n", sourceptr);
		}
		if(chars == -1)
		{
			fprintf(stderr, "Error: Read error from %s\n", destptr);
		}
	}

	if(close(in) == -1 || close(out) == -1)
	{
		fprintf(stderr, "Error: Unable to close files.\n");
	}
}

int main(int argc, char *argv[])
{

	char *source;
	char *destination;
	char *sourceptr;
	char *destptr;
	char *staticptr;
	
	char sourcepath[PATH_MAX+1];
	char destpath[PATH_MAX+1];
	char staticDest[PATH_MAX+1];
	char cwd[PATH_MAX+1];

	struct stat s;

	if(argc != 3)
	{
		fprintf(stderr, "Error: Usage is <clone.c> <source> <destination>\n");
		exit(1);
	}
	/*===============================*/
	/* ABSOLUTE PATH FOR ALL INPUT */
	/*===============================*/
	source = argv[1];
	destination = argv[2];

	sourceptr = realpath(source, sourcepath);
	destptr = realpath(destination, destpath);
	staticptr = realpath(destination, destpath);

	getcwd(cwd, sizeof(cwd));

	strcat(cwd, "/");
	strcat(cwd, source);

	printf("SourcePtr: %s\n", sourceptr);
	printf("DourcePtr: %s\n", destptr);
	printf("StaticPtr: %s\n", staticptr);

	/*===============================*/
	if (chdir(sourceptr))
	{
		fprintf(stderr, "Error: %s: No such file or directory.\n", source);
		exit(1);
	}
	if( IsFile(sourceptr) && IsFile(destptr))	/*File && File*/
	{
		printf("If conditional 1\n");
		printf("Sending\n\t%s\n\t%s\n\n", sourceptr, destptr);
		File(sourceptr, destptr);
	}
	else if(IsFile(sourceptr) && !IsFile(destptr))	/*File && Directory*/
	{
		printf("If conditional 2\n");
		printf("Sending\n\t%s\n\t%s\n\n", sourceptr, destptr);
		strcat(destptr, "/");
		File(sourceptr, destptr);
	}
	else if(!IsFile(sourceptr) && !IsFile(destptr))	/*Directory && Directory*/
	{
		printf("If conditional 3\n");
		printf("Sending\n\t%s\n\t%s\n\n", sourceptr, destptr);
		strcat(destptr, "/");
		strcat(sourceptr, "/");
		Dir(sourceptr, destptr);

	}
	else	
	{
		fprintf(stderr, "Error: Stat did not recognize input type\n");
		exit(1);
	}

	return 0;
}

// int Stat(char* file)
// {
// 	struct stat s;
// 	if(stat(file, &s) >= 0)
// 	{
// 		if(S_ISREG(s.st_mode))
// 		{
// 			return(1);
// 		}
// 		else
// 		{
// 			return (0);
// 		}
// 	}
// }
