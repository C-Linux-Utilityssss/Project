//현재 디렉터리와 파일들의 권한, 파일사이즈, 내용을 출력

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<dirent.h>
#include<sys/stat.h>
#include<unistd.h>

void ls_Inode(struct stat buf){
    printf("%d", (unsigned int)buf.st_ino);
}

void ls_Mode(struct stat buf){
    printf("%o", (unsigned long)buf.st_mode);
}

void ls_FSize(struct stat buf){
    printf("%d", buf.st_size);
}

void ls_option(struct stat buf, char * option){

    if(strcmp(option, "-l") == 0){
	    ls_Mode(buf);
	    ls_Inode(buf);
	    ls_FSize(buf);
    }

    else if(strcmp(option, "-i") == 0){
        ls_Inode(buf);
    }
}

int main(int argc, char *argv[]){
	char * cwd = (char *)malloc(sizeof(char) * 1024);
	
	
    memset(cwd, 0, 1024);
	DIR * dir = NULL;
    struct dirent * entry;
    struct stat buf;

    getcwd(cwd, 1024);

    if( (dir = opendir(cwd)) == NULL){
        printf("opendir() error\n");
        exit(1);
    }

    while( (entry = readdir(dir)) != NULL){
        lstat(entry->d_name, &buf);
        
        if(S_ISREG(buf.st_mode))
            printf("FILE");
        else if(S_ISDIR(buf.st_mode))
            printf("DIR");
        else
            printf("???");
        if(argc > 1)
            ls_option(buf, argv[1]);
        printf("%s\n", entry->d_name);
	}

    free(cwd);
    closedir(dir);

    return 0;

}