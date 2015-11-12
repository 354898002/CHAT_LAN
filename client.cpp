#include <sys/stat.h>
#include <bits/stdc++.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

const int MAXBUF = 1024;

using namespace std;

void *RECV(void *ptr)
{
    int *sockfd = (int *)ptr;
    
    char buffer[MAXBUF+1];
    int len;
        
    while(1)
    {
        len = recv(*sockfd,buffer,MAXBUF,0);
            
        if(len == -1)
        {
            perror("receive");
            exit(EXIT_FAILURE);
        }
            
        if(len > 0)
        {
            buffer[len] =  '\0';
            printf("#### : %s\n",buffer);
        }
    }
}

void *SEND(void *ptr)
{    
    int *sockfd = (int *)ptr;
    
    string str;
        
    int len;
        
    while(1)
    {
        getline(cin,str);
            
        if(str.length() > 0 && str.length() <= 1024)
        {
            len = send(*sockfd,str.c_str(),str.length(),0);
                
            if(len < 0)
            {
                perror("send");
                exit(EXIT_FAILURE);
            }
            else
            {
                cout<<"------- "<<str<<endl;    
            }
        }
        else
        {
            cout<<"------- the length must be between 1 and 1024 "<<endl; 
        }
    }
}
    

int main(int argc,char *argv[])
{
    if(argc != 3)
    {
        puts("Wrong argument !");
        exit(EXIT_FAILURE);
    }
    
    int sockfd,len;
    
    sockaddr_in dest;
    
    if( (sockfd = socket(AF_INET,SOCK_STREAM,0) ) < 0 )
    {
        perror("Socket");
        exit(errno);    
    }
    
    printf("scoket created ! \n");
    bzero(&dest,sizeof(dest));
    
    
    dest.sin_family = AF_INET;
    dest.sin_port = htons(atoi(argv[2]));
    
    if(inet_aton(argv[1],(in_addr*)&dest.sin_addr.s_addr) == 0)
    {
        perror("wrong address : argv[1]");
        exit(errno);
    }
    
    if(connect(sockfd,(sockaddr *)&dest,sizeof(dest) ) == -1)
    {
        perror("Connect ");
        exit(errno);
    }
    
   
    puts("Let's chat ！"); 
    
    pthread_t thR,thS;
    
    pthread_create(&thR,NULL,RECV,(void *)&sockfd);
    pthread_create(&thS,NULL,SEND,(void *)&sockfd);
    
    pthread_join(thR,NULL);
    pthread_join(thS,NULL);
    
    close(sockfd);
    
    return 0;
}
