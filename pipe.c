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
	/*����һ���ܵ�*/
	if(pipe(pipe_fd)<0)
	{
		printf("pipe create error\n");
		return -1;
	}
	/*�����ӽ���*/
	if((pid1=fork( ))==0)	// �ӽ���1ִ������
	  {
		close(pipe_fd[0]);	// �ӽ����ȹرչܵ��Ķ���
		sprintf(w_buffer,"child 1 process is sending message!"); 
		len1 = strlen(w_buffer);
		write(pipe_fd[1],w_buffer,50);     /*��ܵ�д��Ϊlen�ֽڵĴ�*/
		exit(0);
	   }
	 else if(pid1>0)	// ������ִ������
	  {
		waitpid(pid1,NULL,0);	// ͬ��1--���ӽ���1��д��		
		if((pid2=fork( ))==0)	// �ӽ���2ִ������
		{ 
			close(pipe_fd[0]);	// �ӽ����ȹرչܵ��Ķ���
			sprintf(w_buffer,"child 2 process is sending message!");
			len2 = strlen(w_buffer);
			write(pipe_fd[1],w_buffer,50);
			exit(0);
		 }
		 else if(pid2>0)	// ������ִ������
		 {  			
			waitpid(pid2,NULL,0);	// ͬ��2--�ٵ��ӽ���2д��
			close(pipe_fd[1]);	// �������ȹرչܵ���д��
			/*�ӹܵ��ж��̶������ֽڵĴ�*/
			if(read(pipe_fd[0],r_buffer,50)>0)   
				printf("%s\n",r_buffer);
			if(read(pipe_fd[0],r_buffer,50)>0)
				printf("%s\n",r_buffer);
			exit(0);
		}
	  }
	  return 0;
}
// waitpid �������ȴ��ӽ���������ϣ������̲ſ������С�
// ����fork�������ӽ��������ؿ����˸����̵�������ַ�ռ䣬�����ӽ��̶��йܵ��Ķ��˺�д�ˡ�
// ��������ؽ�������ùص����õ���һ�ˡ�
// ����Ҫ�󣬡��������Ƚ����ӽ���P1��������Ϣ��Ȼ���ٽ����ӽ���P2��������Ϣ����
// ��������ͬ�����⣬�����ӽ��̺͸�����֮�䣨����д�󸸶������ӽ���1���ӽ���2֮�䣨��1д����2д��
