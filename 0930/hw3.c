#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

ino_t get_inode(char *);
void printpathto(ino_t);
void inum_to_name(ino_t, char *, int);
int n=0,recurCnt=0;
char path[300];   

int main()
{
	printpathto(get_inode("."));
	putchar('\n');
	printf("%s\n",path);
    return 0;
}
ino_t get_inode(char *fname)
{
	struct stat info;

	if(stat(fname, &info) ==1)
	{
		fprintf(stderr, "Cannot stat");
		perror(fname);
		exit(1);
	}
	return info.st_ino;
}
int compare(const void *A, const void  *B)
{
    struct dirent** ptrA = (struct dirent**)A;
    struct dirent** ptrB = (struct dirent**)B;

    return (*ptrA)->d_ino - (*ptrB)->d_ino;
}
void printDirent(struct dirent *ptr[], int count)
{
    int idx=0;
    while(idx < count)
        printf("%ld\t%s\n",ptr[idx]->d_ino, ptr[idx++]->d_name);       
    printf("--------------------------\n");
}
void printpathto(ino_t this_inode)
{
	ino_t my_inode;
	char its_name[BUFSIZ];
    char tempstr[BUFSIZ];
    struct dirent *direntArr[200]={NULL,};
    DIR *dir_ptr;
	int cnt = 0;
    int idx =0;
    char space[200];
    recurCnt++;
    n=recurCnt;

	if(get_inode("..") != this_inode)
	{
        chdir("..");
        
        dir_ptr = opendir(".");
        while((direntArr[cnt] = readdir(dir_ptr)) != NULL){ cnt++; }
        qsort(direntArr, cnt, sizeof(struct dirent*), compare);
    
        inum_to_name(this_inode, its_name, BUFSIZ);     //inode number -> name을 its_name에 저장
		my_inode = get_inode(".");                      //현재 directory의 inode number를 얻어옴
        printpathto(my_inode);                          //recursive

        /*공백*/
        for(int k=n; k<recurCnt; k++)
            strcat(space,"          ");
        n -= 1;

        /*print*/
        while(idx < cnt)
            printf("%ld\t%s%s\n", direntArr[idx++]->d_ino,space, direntArr[idx]->d_name);    //idx++을 뒤에 뒀을 때 error 발생하는 이유
        
        sprintf(tempstr, "/%s",its_name);
        strcat(path, tempstr);
        closedir(dir_ptr);
	}
}
void inum_to_name(ino_t inode_to_find, char *namebuf, int buflen)
{
	DIR *dir_ptr;
	struct dirent *direntp;
	dir_ptr = opendir(".");
	if(dir_ptr == NULL)
	{
		perror(".");
		exit(1);
	}

	while ((direntp = readdir(dir_ptr)) != NULL)
	    if(direntp->d_ino == inode_to_find)
	    {
		    strncpy(namebuf, direntp->d_name, buflen);
		    namebuf[buflen-1] = '\0';
		    closedir(dir_ptr);
		    return;
    	}
	fprintf(stderr, "error looking for inum %ld\n", inode_to_find);
	exit(1);
}

