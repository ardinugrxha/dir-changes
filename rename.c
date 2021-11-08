#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

void print_dir(char *dir, int depth, int searchSubDir) 
{
    char newName[512];
    int ret = 0;
	DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(dir)) == NULL) {
        fprintf(stderr,"cannot open directory: %s\n", dir);
        return;
    }
    chdir(dir);
    while((entry = readdir(dp)) != NULL) {
        memset(newName, 0, sizeof(newName));
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode)) {
            /*ignore . and .. dir */
            if(strcmp(".",entry->d_name) == 0 ||
                strcmp("..",entry->d_name) == 0)
                continue;
            
            printf("Rename from '%s' to :",entry->d_name);
            scanf("%s",newName);
            
            if(strcmp("N", newName) == 0)
            {
                printf("\n Folder skipped \n \n");
                continue;
            } 
            else 
            {
                if ((ret = rename(entry->d_name,newName)) != 0)
                    fprintf(stderr, "Error in renaming, return code: %d", ret);
            }

            if(searchSubDir == 1)
                print_dir(entry->d_name,depth+4, 1);
        }
        else printf("%*s%s\n",depth,"",entry->d_name);
    }
    chdir("..");
    closedir(dp);
}
int main()
{
    char dir[500];
    int subDir;
	printf("Enter Directory that contain folders: (type 'this' for get current dir) \n");
    scanf("%s", dir);

    printf("Enable scan for sub directory ? (1 for yes 0 for no) \n");
    scanf("%d", &subDir);

    if(strcmp("this", dir) == 0 ||strcmp("This", dir) == 0) 
    {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            print_dir(cwd,1,subDir);
        } else {
            perror("getcwd() error");
            return 1;
        }
    } 
    else 
    {
        print_dir(dir,1,subDir);
    }
    printf("done.\n");
	return 0;
}

