#include <bits/stdc++.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>

const int MAXBUF = 1024;

using namespace std;

void *RECV(void *ptr)
{
    char buffer[MAXBUF+1];
    int len;

    int *sockClient = (int *)ptr;    
    
    while(1)
    {   
        len = recv(*sockClient,buffer,MAXBUF,0);

        if(len == -1)
        {
            perror("receive");
            exit(EXIT_FAILURE);
        } 
        
        if(len > 0)
        {
            buffer[len] = '\0';
            printf("####### : %s\n",buffer);
        }
    }
}

void *SEND(void *ptr)
{
    int *sockClient = (int *)ptr;
    string str;
    int len;
    while(1)
    {
        getline(cin,str);
            
        if(str.length() > 0 && str.length() <= 1024)
        {
            len = send(*sockClient,str.c_str(),str.length(),0);
                
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

int main(int argc,char **argv)
{

    if(argc != 3)
    {
        puts("Worng argument!");
        exit(EXIT_FAILURE);
    }
    
    int sockServer,sockClient;
    unsigned int myport,lisnum;
    sockaddr_in my_addr,their_addr;
    socklen_t len;    
    myport = atoi(argv[2]);
    lisnum = 5;
    
    if( (sockServer = socket(AF_INET,SOCK_STREAM,0) ) == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    bzero(&my_addr,sizeof(my_addr));
    
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(myport);
    my_addr.sin_addr.s_addr = inet_addr(argv[1]);
    
    if(bind(sockServer,(sockaddr *)&my_addr,sizeof(my_addr)) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    
    if(listen(sockServer,lisnum) == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    puts("connect me,please!");
    
    len = sizeof(sockaddr);
        
    if( (sockClient = accept(sockServer,(sockaddr *)&their_addr,&len) ) == -1)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Server got connection form IP : %s port : %d,socket %d\n",inet_ntoa(their_addr.sin_addr),ntohs(their_addr.sin_port),sockClient);
    }
    
    pthread_t thR,thS;
    
    pthread_create(&thR,NULL,RECV,(void *)&sockClient);
    pthread_create(&thS,NULL,SEND,(void *)&sockClient);
    
    pthread_join(thR,NULL);
    pthread_join(thS,NULL);
    
    close(sockServer);
    close(sockClient);
    
    return 0;
}
