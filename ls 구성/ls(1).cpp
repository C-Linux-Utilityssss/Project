//현재 디렉터리와 파일 출력

#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<unistd.h>

int main(void){
    char * cwd = (char *)malloc(sizeof(char) * 1024);

    DIR * dir = NULL;
    struct dirent * entry = NULL;

    getcwd(cwd, 1024);

    if( (dir = opendir(cwd)) == NULL){
        printf("current directory error\n");
        exit(1);
    }
    while( (entry = readdir(dir)) != NULL){
        printf("%s\n", entry->d_name);
    }
    
    free(cwd);
    closedir(dir);

    return 0;
}

