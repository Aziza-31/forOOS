#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFF_SIZE 0xffff
int main(int argc, char *argv[]) {
	char *ptr;
	int fd[2];
	char buff[BUFF_SIZE];
	key_t key = ftok("/bin/cat", 0);
	int desc = shmget(key, BUFF_SIZE, 0666 | IPC_CREAT | IPC_EXCL);
	if(desc == -1){
		if(errno == EEXIST){
			printf("Memory was existed\n");
			exit(-1);
		}
	}
	close(0);
	close(1);
	pipe(fd);
	int a = fork();
	if (a == 0){
	    execl("/bin/ps","ps", "-1", NULL);
	}
	else{
	if((ptr = (char *)shmat(desc, NULL, 0)) == (char *)(-1)){
	printf("Error attach shared memory\n");
	exit(-1);
    }
    close(1);
    read(0, buff, BUFF_SIZE);
    memcpy(ptr, buff	, BUFF_SIZE );
    if (shmdt(ptr) < 0){
	printf("Can't detach shared memory\n");
	exit(-1);
    }
	}
}
