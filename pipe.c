#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
int main( )
{ 
	pid_t pid1,pid2;
	int pipe_fd[2];
	char w_buffer[100],r_buffer[100];	 
	int	len1,len2;
	/*创建一个管道*/
	if(pipe(pipe_fd)<0)
	{
		printf("pipe create error\n");
		return -1;
	}
	/*创建子进程*/
	if((pid1=fork( ))==0)	// 子进程1执行序列
	  {
		close(pipe_fd[0]);	// 子进程先关闭管道的读段
		sprintf(w_buffer,"child 1 process is sending message!"); 
		len1 = strlen(w_buffer);
		write(pipe_fd[1],w_buffer,50);     /*向管道写长为len字节的串*/
		exit(0);
	   }
	 else if(pid1>0)	// 父进程执行序列
	  {
		waitpid(pid1,NULL,0);	// 同步1--等子进程1先写完		
		if((pid2=fork( ))==0)	// 子进程2执行序列
		{ 
			close(pipe_fd[0]);	// 子进程先关闭管道的读段
			sprintf(w_buffer,"child 2 process is sending message!");
			len2 = strlen(w_buffer);
			write(pipe_fd[1],w_buffer,50);
			exit(0);
		 }
		 else if(pid2>0)	// 父进程执行序列
		 {  			
			waitpid(pid2,NULL,0);	// 同步2--再等子进程2写完
			close(pipe_fd[1]);	// 父进程先关闭管道的写段
			/*从管道中读固定长度字节的串*/
			if(read(pipe_fd[0],r_buffer,50)>0)   
				printf("%s\n",r_buffer);
			if(read(pipe_fd[0],r_buffer,50)>0)
				printf("%s\n",r_buffer);
			exit(0);
		}
	  }
	  return 0;
}
// waitpid 函数，等待子进程运行完毕，父进程才可以运行。
// 由于fork函数让子进程完整地拷贝了父进程的整个地址空间，所以子进程都有管道的读端和写端。
// 所以在相关进程中最好关掉不用的那一端。
// 根据要求，“父进程先接收子进程P1发来的消息，然后再接收子进程P2发来的消息。”
// 存在两个同步问题，两个子进程和父进程之间（先子写后父读）、子进程1和子进程2之间（先1写，再2写）
