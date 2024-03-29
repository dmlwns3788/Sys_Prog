#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

void do_ls(char []);

main(int ac, char *av[])
{
    if (ac==1) //입력받은게 없다면
        do_ls("."); //.은 자기자신을 의미
    else
        while(--ac){
            printf("%s: \n", *++av);
        }
}

void do_ls(char dirname[])
{
    DIR *dir_ptr;
    struct dirent *direntp;

    if((dir_ptr = opendir(dirname))==NULL)
        fprintf(stderr, "ls1: cannot open %s\n", dirname);
    else
    {
        while((direntp=readdir(dir_ptr))!=NULL)
            printf("%s\n", direntp->d_name);
        closedir(dir_ptr);
    }
}
