#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

typedef struct SUDO {
    char quest[100];
    char input[100];
    char answer[100];
    char error[100];
    char log_x[100];
    char log_y[100];
    char log_v[100];
} SUDO;

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

int sudoQuest(SUDO *sudo,int randval,int difficult)
{
    difficult=MAX(MIN(difficult,10),0);
    int num[100],log[1000];
    int x,y,i,j,flag=0,flag2=0,r;
    struct timeval time,time2;
    struct timezone tz;
    srand(randval);
    for(i=0;i<100;i++)num[i]=0;
    for(y=0;y<9;)
        for(x=0;x<9;)
        {   num[y*9+x]=rand()%(9-1+1)+1;
	    flag=0;
	    flag2=0;
	    log[(y*9+x)*10+num[y*9+x]]=1;
	    for(i=0;i<9;i++)
	    {	if((num[y*9+x]==num[y*9+i] && x!=i) || (num[y*9+x]==num[i*9+x] && y!=i) || 
		   (num[y*9+x]==num[((y/3)*3+i/3)*9+((x/3)*3+i%3)] && y*9+x!=((y/3)*3+i/3)*9+((x/3)*3+i%3)))
		    flag=1;
		if(log[(y*9+x)*10+i+1]==1)flag2=flag2+1;
	    }

	    if(flag==0){
		if(x==8 && y==8){x=9;y=9;break;}
		if(x==8){y++;x=0;}
		else x=x+1;
	    }
	    else if(flag2>8){
		num[y*9+x]=0;
		for(i=0;i<10;i++){
		    log[(y*9+x)*10+i]=0;
		}
		if(x>0) x=x-1;
		else if(y>0) {y--;x=8;}
	    }
	    
/*system("clear");
	printf("%d\n",flag2);
    for(i=0;i<9;i++)
    {   for(j=0;j<9;j++)
	{    printf("%d ",num[i*9+j]);
	     if((j+1)%3==0) printf(" ");
    	}
    if((i+1)%3==0)printf("\n");
    printf("\n");
    }*/

        }
    for(i=0;i<9;i++)
    {   for(j=0;j<9;j++)
	{    printf("%d ",num[i*9+j]);
	     if((j+1)%3==0) printf(" ");
    	}
    if((i+1)%3==0)printf("\n");
    printf("\n");
    }
    
    for(i=0;i<81;i++){
	r=(rand()%(100-1+1)+1);
	sudo->input[i]=0;
	if((20+3*difficult)>r)sudo->quest[i]=0;
	else sudo->quest[i]=num[i];
	//printf("%d ",sudo->quest[i]);
	//if((i+1)%9==0)printf("\n");
    }
}


int sudoLog(SUDO *sudo,int target_x,int target_y){
int j;
		for(j=98;j>=0;j--){
		    sudo->log_x[j+1]=sudo->log_x[j];
		    sudo->log_y[j+1]=sudo->log_y[j];
		    sudo->log_v[j+1]=sudo->log_v[j];
		    }
		sudo->log_x[0]=target_x;
		sudo->log_y[0]=target_y;
		sudo->log_v[0]=sudo->input[target_y*9+target_x];
}

int sudoCheck(SUDO *sudo){
    int x,y,flag,i,j=0;
    for(i=0;i<81;i++){
        if(sudo->quest[i]!=0)sudo->answer[i]=sudo->quest[i];
	else if(sudo->input[i]!=0)sudo->answer[i]=sudo->input[i];
	else sudo->answer[i]=0;
    }

    for(y=0;y<9;y++)
        for(x=0;x<9;x++)
        {   flag=0;

	    for(i=0;i<9;i++)
	    {	if((sudo->answer[y*9+x]==sudo->answer[y*9+i] && x!=i) || (sudo->answer[y*9+x]==sudo->answer[i*9+x] && y!=i) || 
		   (sudo->answer[y*9+x]==sudo->answer[((y/3)*3+i/3)*9+((x/3)*3+i%3)] && y*9+x!=((y/3)*3+i/3)*9+((x/3)*3+i%3)) ||
		    sudo->answer[y*9+x]==0)
		    flag=1;  
	    }
	    if(flag==1) sudo->error[y*9+x]=1;
	    else sudo->error[y*9+x]=0;
	}
}
/*
int main(){
SUDO s;
sudoQuest(&s,2);
}*/

