#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <asm/types.h> 
#include <linux/videodev2.h>
#include <linux/fb.h>

#include "bmp.c"
#include "sudo.c"
#include <pthread.h>
#include <sys/stat.h>
#include <linux/input.h>
#define NOKEY 0

static unsigned char *vf_buff;
static unsigned char *vf_buff1;
static int fb_bpp;
static int cam_fp = -1;
static int fb_fp = -1;
static char *fb_addr = NULL;
static char cam_name[50];
int width=0;
int height=0;

int mouse_X;
int mouse_Y;
int mouse_state;

void *mouse(void *arg);

static inline int fb_init(void)
{
	int dev_fp = -1;
	int fb_size;
	struct fb_var_screeninfo vinfo;

	dev_fp = open("/dev/fb0", O_RDWR);
	if (dev_fp < 0) {
		perror("/dev/fb0");
		return -1;
	}
	if (ioctl(dev_fp, FBIOGET_VSCREENINFO, &vinfo)) {
		printf("Error reading variable information.\n");
		exit(1);
	}
	width=vinfo.xres;
	height=vinfo.yres;
	fb_bpp=vinfo.bits_per_pixel;
	if(fb_bpp==24) fb_bpp=32;
	fb_size=width*height*fb_bpp/8;
	if ((fb_addr = (char*)mmap(0, fb_size, 
			PROT_READ | PROT_WRITE, MAP_SHARED, dev_fp, 0)) < 0) {
		perror("mmap()");
		return -1;
	}
	printf("%dx%d bpp:%d mmaped 0x%08x\n",width,height,fb_bpp,fb_addr);

	return dev_fp;
}
/*
static inline void print_fps(struct timeval *s, struct timeval *e)
{
	unsigned long time;
	unsigned long sec;
	unsigned long usec;
	int fps = 0;
	sec = e->tv_sec - s->tv_sec;
	if (e->tv_usec > s->tv_usec)
		usec = e->tv_usec - s->tv_usec;
	else {
		usec = e->tv_usec + 1000000 - s->tv_usec;	
		sec--;
	}
	time = sec * 1000 + (usec+1) / 1000;
	if(time==0)
	    return;
	fps = 30000 / time;
	printf("%d fps\n", fps);
}*/
int main(int argc, char *argv[])
{
	unsigned int frames = 0;
	struct timeval tv1;
	struct timeval start_tv, end_tv;
	struct timezone tz;
	int src_img;
	unsigned long size;
	int index=0;
	struct v4l2_capability cap;
	struct v4l2_input i;
	struct v4l2_framebuffer fb;
	int on=1;
	int tmp,m,n;
	fd_set fds1;
	int fd;
	char cmd[256];
	struct fb_var_screeninfo vinfo;
	
	fb_fp = fb_init();
	
	size=width*height*fb_bpp/8;
	
	memset(&i, 0, sizeof(i));
	i.index=index;
	
	index=0;

	printf("g_fbuf:capabilities=0x%x,flags=0x%x,width=%d,height=%d\npixelformat=0x%x,bytesperline=%d,colorspace=%d,base=0x%x\n",
		fb.capability,fb.flags,fb.fmt.width,fb.fmt.height,fb.fmt.pixelformat,
		fb.fmt.bytesperline,fb.fmt.colorspace,fb.base);
	fb.capability = cap.capabilities;
	fb.fmt.width =width;
	fb.fmt.height = height;
	fb.fmt.pixelformat = (fb_bpp==32)?V4L2_PIX_FMT_BGR32:V4L2_PIX_FMT_RGB565;
	
	vf_buff = (char*)malloc(size);
	if(vf_buff==NULL)
		goto err;


	gettimeofday(&start_tv,&tz);
//==================================================================================================<<<<<<<<<<<<<<<<<<<<<<<<<<<
//dim & load bmp
    mouse_X=0;
    mouse_Y=0;
    mouse_state=0;
    pthread_t ntid;
    int gameThread = pthread_create (&ntid,NULL,mouse,NULL);
    int state=0,error_val=0;
    int num_x=0,num_y=0,target_x=0,target_y=0,complete=0;
    BMP title,title_c,background,mouse,purple,red,blue,target,block,layout,layout_m,btn_alpha;
    BMP num[10],dot,BG[10],btn[10];
    BMP btn_l,btn_r,btn_c,btn_Y,btn_Y_a,back,back_a,back_Y,back_Y_a;
    BMP succese,fail,succese_g,fail_r,text1,text2;
    BMP white,white_a;
    bmpLoad(&title,"./picture/title.bmp");
    bmpLoad(&title_c,"./picture/title_c.bmp");
    bmpLoad(&background,"./picture/background.bmp");
    bmpLoad(&mouse,"./picture/wait_rm.bmp");
    bmpLoad(&purple,"./picture/purple.bmp");
    bmpLoad(&red,"./picture/red.bmp");
    bmpLoad(&blue,"./picture/blue.bmp");
    bmpLoad(&block,"./picture/block.bmp");

    bmpLoad(&layout,"./picture/layout.bmp");
    bmpLoad(&layout_m,"./picture/layout_m.bmp");
    bmpLoad(&target,"./picture/target.bmp");

    bmpLoad(&num[1],"./picture/num1.bmp");
    bmpLoad(&num[2],"./picture/num2.bmp");
    bmpLoad(&num[3],"./picture/num3.bmp");
    bmpLoad(&num[4],"./picture/num4.bmp");
    bmpLoad(&num[5],"./picture/num5.bmp");
    bmpLoad(&num[6],"./picture/num6.bmp");
    bmpLoad(&num[7],"./picture/num7.bmp");
    bmpLoad(&num[8],"./picture/num8.bmp");
    bmpLoad(&num[9],"./picture/num9.bmp");
    bmpLoad(&num[0],"./picture/num0.bmp");
    bmpLoad(&dot,"./picture/dot.bmp");

    bmpLoad(&btn[0],"./picture/btn_0.bmp");
    bmpLoad(&btn[1],"./picture/btn_1.bmp");
    bmpLoad(&btn[2],"./picture/btn_2.bmp");
    bmpLoad(&btn[3],"./picture/btn_3.bmp");
    bmpLoad(&btn[4],"./picture/btn_4.bmp");
    bmpLoad(&btn[5],"./picture/btn_5.bmp");
    bmpLoad(&btn[6],"./picture/btn_6.bmp");
    bmpLoad(&btn[7],"./picture/btn_7.bmp");
    bmpLoad(&btn[8],"./picture/btn_8.bmp");
    bmpLoad(&btn[9],"./picture/btn_9.bmp");
    bmpLoad(&btn_l,"./picture/btn_l.bmp");
    bmpLoad(&btn_c,"./picture/btn_clear.bmp");
    bmpLoad(&btn_r,"./picture/btn_r.bmp");
    bmpLoad(&btn_Y,"./picture/btn_Y.bmp");
    bmpLoad(&btn_Y_a,"./picture/btn_Y_alpha.bmp");
    bmpLoad(&btn_alpha,"./picture/btn_alpha.bmp");
    bmpLoad(&back,"./picture/back.bmp");
    bmpLoad(&back_a,"./picture/back_a.bmp");
    bmpLoad(&back_Y,"./picture/back_Y.bmp");
    bmpLoad(&back_Y_a,"./picture/back_Y_a.bmp");

    bmpLoad(&succese,"./picture/succese_a.bmp");
    bmpLoad(&succese_g,"./picture/succese_g.bmp");
    bmpLoad(&fail,"./picture/fail_a.bmp");
    bmpLoad(&fail_r,"./picture/fail_r.bmp");
    bmpLoad(&text1,"./picture/text1.bmp");
    bmpLoad(&text2,"./picture/text2.bmp");
    bmpLoad(&white,"./picture/white.bmp");
    bmpLoad(&white_a,"./picture/white_a.bmp");

    SUDO sudo;
    struct timeval time_a,time_b;
    struct timezone timezone;
    unsigned long time=0,time_min=0,time_sec=0;
    int randval[15],difficult[5];
//==========================================================================<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//display & contral
while(1){
int i=0,j=0;

    if(state==0){									//title
	bmpDisplay(&background,0,0,vf_buff,width,height);
	bmpDisplayAdv(&title_c,120,40,vf_buff,width,height,&title,1.0);
	bmpBG(vf_buff,width,height,&BG[1]);
	printf("Load Done!\n");
	state=1;}
    if(state==1){									//title
	bmpDisplay(&BG[1],0,0,vf_buff,width,height);
	bmpDisplayAdv(&mouse,mouse_X,mouse_Y,vf_buff,width,height,&mouse,1.0);
	    if(mouse_state==1){
		printf("Setting Game...\n");
		state=2;
		mouse_state=0;}
	    }
    if(state==2){									//display text & option value & button
	bmpDisplay(&background,0,0,vf_buff,width,height);
	bmpDisplayAdv(&text1,40,40,vf_buff,width,height,&text1,1.0);
	bmpDisplayAdv(&white,40,75,vf_buff,width,height,&white_a,0.4);
	bmpDisplayAdv(&text2,40,120,vf_buff,width,height,&text2,1.0);
	bmpDisplayAdv(&white,40,155,vf_buff,width,height,&white_a,0.4);
	bmpDisplayAdv(&btn[1],300,180,vf_buff,width,height,&btn_alpha,1.0);
	bmpDisplayAdv(&btn[2],350,180,vf_buff,width,height,&btn_alpha,1.0);
	bmpDisplayAdv(&btn[3],400,180,vf_buff,width,height,&btn_alpha,1.0);
	bmpDisplayAdv(&btn[4],300,150,vf_buff,width,height,&btn_alpha,1.0);
	bmpDisplayAdv(&btn[5],350,150,vf_buff,width,height,&btn_alpha,1.0);
	bmpDisplayAdv(&btn[6],400,150,vf_buff,width,height,&btn_alpha,1.0);
	bmpDisplayAdv(&btn[7],300,120,vf_buff,width,height,&btn_alpha,1.0);
	bmpDisplayAdv(&btn[8],350,120,vf_buff,width,height,&btn_alpha,1.0);
	bmpDisplayAdv(&btn[9],400,120,vf_buff,width,height,&btn_alpha,1.0);
	bmpDisplayAdv(&btn_l,300,210,vf_buff,width,height,&btn_alpha,1.0);
	bmpDisplayAdv(&btn[0],350,210,vf_buff,width,height,&btn_alpha,1.0);
	bmpDisplayAdv(&btn_r,400,210,vf_buff,width,height,&btn_alpha,1.0);
	gettimeofday(&time_a,&tz);
	srand(time_a.tv_sec);
	randval[0]=rand()%(100000000-1+1)+1;
	printf("Random Seed:%d\n",randval[0]);
	difficult[0]=1;
	for(i=1;i<=8;i++)
	    randval[i]=randval[0]%(int)pow(10,i)/pow(10,i-1);
	for(i=1;i<2;i++)
	    difficult[i]=difficult[0]%(int)pow(10,i)/pow(10,i-1);
	bmpBG(vf_buff,width,height,&BG[2]);
	bmpBG(vf_buff,width,height,&BG[3]);
	state=3;
    }
    if(state==3){
	if(mouse_X>=300 && mouse_X<440 && mouse_Y>120 && mouse_Y<235 && ((mouse_X-300)%50)<40 && (((240-mouse_Y)%30)<30 && ((240-mouse_Y)%30)>5)){										//display yellow frame
	    bmpDisplay(&BG[3],0,0,vf_buff,width,height);
	    bmpDisplayAdv(&btn_Y,300+((mouse_X-300)/50)*50,210-((240-mouse_Y)/30)*30,vf_buff,width,height,&btn_Y_a,1.0);
	}
	else bmpDisplay(&BG[3],0,0,vf_buff,width,height);

	if(mouse_state==1){
	    bmpDisplay(&BG[2],0,0,vf_buff,width,height);
		if(mouse_X>=300 && mouse_Y>120 && mouse_X<440 && mouse_Y<235 && ((mouse_X-300)%50)<40 && (((240-mouse_Y)%30)<30 && ((240-mouse_Y)%30)>5)){
		    printf("Click Button Code:%d\t",((mouse_X-300)/50+((240-mouse_Y)/30)*3));
		    if(((mouse_X-300)/50+((240-mouse_Y)/30)*3)>=3){			//input number 1~9
			printf("Function:NUM_%d\n",((mouse_X-300)/50+((240-mouse_Y)/30)*3)-2);
			if(target_y==1){
			    randval[9-target_x]=((mouse_X-300)/50+((240-mouse_Y)/30)*3)-2;
			    if(target_x<8)target_x++;
			}
			else if(target_y==2){
			    difficult[3-target_x]=((mouse_X-300)/50+((240-mouse_Y)/30)*3)-2;
			    if(target_x<2)target_x++;
			}
		    }
		    else if(((mouse_X-300)/50+((240-mouse_Y)/30)*3)==1){		//input number 0
			printf("Function:NUM_0\n");
			if(target_y==1){
			    randval[9-target_x]=0;
			    if(target_x<8)target_x++;
			}
			else if(target_y==2){
			    difficult[3-target_x]=0;
			    if(target_x<2)target_x++;
			}
		    }
		    else if(((mouse_X-300)/50+((240-mouse_Y)/30)*3)==2){		//start game
			printf("Function:Start Game\n");
			randval[0]=0;
			difficult[0]=0;
			for(i=1;i<=8;i++)
			    randval[0]=randval[0]+randval[i]*pow(10,i-1);
			for(i=1;i<=2;i++)
			    difficult[0]=difficult[0]+difficult[i]*pow(10,i-1);
			sudoQuest(&sudo,randval[0],difficult[0]);
			state=10;
		    }
		    else if(((mouse_X-300)/50+((240-mouse_Y)/30)*3)==0){		//backspace
			printf("Function:Backspace\n");
			if(target_x>1)target_x--;
		    }
		}
	    else if(mouse_X>=40 && mouse_X<280 && mouse_Y>=40 && mouse_Y<100){		//select randval
		printf("Function:Select Text1\n");
		target_x=(mouse_X-40)/24+1;
		target_y=1;
	    }
	    else if(mouse_X>=40 && mouse_X<88 && mouse_Y>=120 && mouse_Y<180){		//select difficult
		printf("Function:Select Text2\n");
		target_x=(mouse_X-40)/24+1;
		target_y=2;
	    }
	bmpBG(vf_buff,width,height,&BG[3]);
	mouse_state=0;
	}
	for(i=1;i<=8;i++){								//display randval value
	    if(target_x==(9-i) && target_y==1)
		bmpDisplayAdv(&red,20+(9-i)*24,78,vf_buff,width,height,&num[randval[i]],1.0);
	    else
		bmpDisplayAdv(&blue,20+(9-i)*24,78,vf_buff,width,height,&num[randval[i]],1.0);
	}
	for(i=1;i<=2;i++){								//display dufficult value
	    if(target_x==(3-i) && target_y==2)
		bmpDisplayAdv(&red,20+(3-i)*24,158,vf_buff,width,height,&num[difficult[i]],1.0);
	    else
		bmpDisplayAdv(&blue,20+(3-i)*24,158,vf_buff,width,height,&num[difficult[i]],1.0);
	}
	bmpDisplayAdv(&mouse,mouse_X,mouse_Y,vf_buff,width,height,&mouse,1.0);
    }
    if(state==10){									//display layout & number & button
	bmpDisplay(&background,0,0,vf_buff,width,height);
	bmpDisplayAdv(&layout,0,0,vf_buff,width,height,&layout_m,1.0);
	for(num_x=0;num_x<9;num_x++)
	    for(num_y=0;num_y<9;num_y++){
		if(sudo.quest[num_y*9+num_x]!=0)
		    bmpDisplayAdv(&block,19+num_x*26,19+num_y*26,vf_buff,width,height,&num[sudo.quest[num_y*9+num_x]],1.0);
	    }
	bmpDisplayAdv(&btn[1],300,180,vf_buff,width,height,&btn_alpha,1.0);
	bmpDisplayAdv(&btn[2],350,180,vf_buff,width,height,&btn_alpha,1.0);
	bmpDisplayAdv(&btn[3],400,180,vf_buff,width,height,&btn_alpha,1.0);
	bmpDisplayAdv(&btn[4],300,150,vf_buff,width,height,&btn_alpha,1.0);
	bmpDisplayAdv(&btn[5],350,150,vf_buff,width,height,&btn_alpha,1.0);
	bmpDisplayAdv(&btn[6],400,150,vf_buff,width,height,&btn_alpha,1.0);
	bmpDisplayAdv(&btn[7],300,120,vf_buff,width,height,&btn_alpha,1.0);
	bmpDisplayAdv(&btn[8],350,120,vf_buff,width,height,&btn_alpha,1.0);
	bmpDisplayAdv(&btn[9],400,120,vf_buff,width,height,&btn_alpha,1.0);
	bmpDisplayAdv(&btn_l,300,210,vf_buff,width,height,&btn_alpha,1.0);
	bmpDisplayAdv(&btn_c,350,210,vf_buff,width,height,&btn_alpha,1.0);
	bmpDisplayAdv(&btn_r,400,210,vf_buff,width,height,&btn_alpha,1.0);

	bmpBG(vf_buff,width,height,&BG[2]);
	bmpBG(vf_buff,width,height,&BG[3]);
	printf("Layout OK!\nTimer Start!\n");
	gettimeofday(&time_a,&timezone);
	state=11;}
    if(state==11){
	if(mouse_X>=300 && mouse_X<440 && mouse_Y>120 && mouse_Y<235 && ((mouse_X-300)%50)<40 && (((240-mouse_Y)%30)<30 && ((240-mouse_Y)%30)>5)){										//display yellow frame
	    bmpDisplay(&BG[3],0,0,vf_buff,width,height);
	    bmpDisplayAdv(&btn_Y,300+((mouse_X-300)/50)*50,210-((240-mouse_Y)/30)*30,vf_buff,width,height,&btn_Y_a,1.0);
	}
	else bmpDisplay(&BG[3],0,0,vf_buff,width,height);

	if(mouse_state==1){
	    bmpDisplay(&BG[2],0,0,vf_buff,width,height);
		if(mouse_X>=300 && mouse_Y>=120 && mouse_X<440 && mouse_Y<235 && ((mouse_X-300)%50)<40 && (((240-mouse_Y)%30)<30 && ((240-mouse_Y)%30)>5)){
		    printf("Click Button Code:%d\t",((mouse_X-300)/50+((240-mouse_Y)/30)*3));
		    if(((mouse_X-300)/50+((240-mouse_Y)/30)*3)>=3){			//input number
			printf("Function:(%d,%d)NUM_%d\n",target_x,target_y,((mouse_X-300)/50+((240-mouse_Y)/30)*3)-2);
			sudoLog(&sudo,target_x,target_y);
			sudo.input[(target_y)*9+(target_x)]=(mouse_X-300)/50+((240-mouse_Y)/30)*3-2;
		    }
		    else if(((mouse_X-300)/50+((240-mouse_Y)/30)*3)==1){		//clear input
			printf("Function:(%d,%d)Clear\n",target_x,target_y);
			sudoLog(&sudo,target_x,target_y);
			sudo.input[(target_y)*9+(target_x)]=0;
		    }
		    else if(((mouse_X-300)/50+((240-mouse_Y)/30)*3)==0){		//restore last step
			printf("Function:Backspace\n");
			sudo.input[sudo.log_y[0]*9+sudo.log_x[0]]=sudo.log_v[0];
			for(j=0;j<=98;j++){
			    sudo.log_x[j]=sudo.log_x[j+1];
			    sudo.log_y[j]=sudo.log_y[j+1];
			    sudo.log_v[j]=sudo.log_v[j+1];
			}
		    }
		    else if(((mouse_X-300)/50+((240-mouse_Y)/30)*3)==2){		//check answer
			printf("Function:Check Answer\t");
			sudoCheck(&sudo);
			error_val=0;
			for(num_y=0;num_y<9;num_y++)					//display wrong number
			    for(num_x=0;num_x<9;num_x++){
				if(sudo.error[num_y*9+num_x]==1 && sudo.quest[num_y*9+num_x]==0){
				    bmpDisplayAdv(&red,19+num_x*26,19+num_y*26,vf_buff,width,height,&num[sudo.input[num_y*9+num_x]],1.0);
				    error_val=error_val+1;
				    }
				else if(sudo.error[num_y*9+num_x]==0 && sudo.quest[num_y*9+num_x]==0){
				    bmpDisplayAdv(&blue,19+num_x*26,19+num_y*26,vf_buff,width,height,&num[sudo.input[num_y*9+num_x]],1.0);
				    }
			    }
			if(error_val==0){
			    printf("Game Clear!\n");
			    bmpDisplayAdv(&succese_g,300,20,vf_buff,width,height,&succese,1.0);
			}
			else{
			    printf("Game Fail!\n");
			    bmpDisplayAdv(&fail_r,330,20,vf_buff,width,height,&fail,1.0);
			}
			bmpBG(vf_buff,width,height,&BG[4]);
			gettimeofday(&time_b,&timezone);				//count use time
			if (time_b.tv_usec > time_a.tv_usec)
			    time = (time_b.tv_sec - time_a.tv_sec)*1000+(time_b.tv_usec - time_a.tv_usec + 1)/1000;
			else
			    time = (time_b.tv_sec - time_a.tv_sec - 1)*1000+(time_b.tv_usec + 1000001 - time_a.tv_usec)/1000;	
			time_sec=time/1000-(time/1000/60)*60;
			time_min=time/1000/60;
			printf("Use Time %02d:%02d\n",time_min,time_sec);
			state=12;
		    }
		}
		else if(mouse_X>=19 && mouse_X<253 && mouse_Y>=19 && mouse_Y<253){	//select target
		    target_x=(mouse_X-19)/26;
		    target_y=(mouse_Y-19)/26;
		    printf("Select :X=%d\tY=%d\n",target_x,target_y);
		}
		for(num_y=0;num_y<9;num_y++)						//refresh number
		    for(num_x=0;num_x<9;num_x++){
			if(sudo.quest[num_y*9+num_x]==0 && sudo.input[num_y*9+num_x]!=0)
			    bmpDisplayAdv(&blue,19+num_x*26,19+num_y*26,vf_buff,width,height,&num[sudo.input[num_y*9+num_x]],1.0);}
		bmpBG(vf_buff,width,height,&BG[3]);
		mouse_state=0;
	    }
	    else if(mouse_state==2){
		if(target_x<8)target_x++;
		else if(target_x==8 && target_y<8){target_x=0;target_y++;}
		else {target_x=0;target_y=0;}
		printf("Select :X=%d\tY=%d\n",target_x,target_y);
		mouse_state=0;
	    }
	    else if(mouse_X>=19 && mouse_X<253 && mouse_Y>=19 && mouse_Y<253){		//display point
		if(sudo.quest[((mouse_Y-19)/26)*9+(mouse_X-19)/26]==0)
		    bmpDisplayAdv(&target,19+((mouse_X-19)/26)*26,19+((mouse_Y-19)/26)*26,vf_buff,width,height,&target,1.0);
		if(sudo.quest[((mouse_Y-19)/26)*9+(mouse_X-19)/26]!=0)
		    bmpDisplayAdv(&red,19+((mouse_X-19)/26)*26,19+((mouse_Y-19)/26)*26,vf_buff,width,height,&target,1.0);
	    }
	bmpDisplayAdv(&blue,19+target_x*26,19+target_y*26,vf_buff,width,height,&target,1.0);
	bmpDisplayAdv(&mouse,mouse_X,mouse_Y,vf_buff,width,height,&mouse,1.0);
    }
    if(state==12){
	bmpDisplay(&BG[4],0,0,vf_buff,width,height);
											//display use time
	bmpDisplayAdv(&num[time_min/10],300,60,vf_buff,width,height,&num[time_min/10],1.0);
	bmpDisplayAdv(&num[time_min%10],330,60,vf_buff,width,height,&num[time_min%10],1.0);
	bmpDisplayAdv(&dot,350,60,vf_buff,width,height,&dot,1.0);
	bmpDisplayAdv(&num[time_sec/10],370,60,vf_buff,width,height,&num[time_sec/10],1.0);
	bmpDisplayAdv(&num[time_sec%10],400,60,vf_buff,width,height,&num[time_sec%10],1.0);
	bmpDisplayAdv(&back,300,90,vf_buff,width,height,&back_a,1.0);
	if(mouse_X>=300 && mouse_X<=340 && mouse_Y>=210 && mouse_Y<=235){		//click button try again
	    bmpDisplayAdv(&btn_Y,300,210,vf_buff,width,height,&btn_Y_a,1.0);
	    if(mouse_state==1 && ((mouse_X-300)/50+((240-mouse_Y)/30)*3)==0){
		printf("Click Button Code:0\tFunction:Backspace\n");
		bmpDisplay(&BG[2],0,0,vf_buff,width,height);
		sudo.input[sudo.log_y[0]*9+sudo.log_x[0]]=sudo.log_v[0];
		for(j=0;j<=98;j++){
		    sudo.log_x[j]=sudo.log_x[j+1];
		    sudo.log_y[j]=sudo.log_y[j+1];
		    sudo.log_v[j]=sudo.log_v[j+1];
		}
		for(num_y=0;num_y<9;num_y++)
		    for(num_x=0;num_x<9;num_x++){
			if(sudo.quest[num_y*9+num_x]==0 && sudo.input[num_y*9+num_x]!=0)
			    bmpDisplayAdv(&blue,19+num_x*26,19+num_y*26,vf_buff,width,height,&num[sudo.input[num_y*9+num_x]],1.0);}
		bmpBG(vf_buff,width,height,&BG[3]);
		state=11;
		mouse_state=0;
	    }
	}
	else if(mouse_X>=300 && mouse_X<=400 && mouse_Y>=90 && mouse_Y<=115){		//click button to title
	    bmpDisplayAdv(&back_Y,300,90,vf_buff,width,height,&back_Y_a,1.0);
	    if(mouse_state==1){
		printf("Back Title\n");
		state=0;
		mouse_state=0;
	    }
	}
	else mouse_state=0;
	bmpDisplayAdv(&mouse,mouse_X,mouse_Y,vf_buff,width,height,&mouse,1.0);
    }
//================================================================================================<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    memcpy(&fb_addr[0],vf_buff,width*height*fb_bpp/8);
    frames++;

    if((frames%30)==0){
	gettimeofday(&end_tv, &tz);
	//print_fps(&start_tv, &end_tv);
	gettimeofday(&start_tv, &tz);
    }
    fd=0;
    tv1.tv_sec=0;
    tv1.tv_usec=0;
    FD_ZERO(&fds1);
    FD_SET(fd,&fds1);
    select(fd+1,&fds1,NULL,NULL,&tv1);
}
err:
	return 1;
end:
	return 0;
}

void *mouse(void *arg)
{
	int keys_fd;	
	char ret[2]; 
	struct input_event t;
	while(1){
	keys_fd = open("/dev/input/event1", O_RDONLY);
	if(keys_fd<=0)
	{
		printf("open /dev/input/event1 device error!\n");
		system("sleep 5"); 
	}
	else break;
	}
	while(1)
	{	
		if(read(keys_fd,&t,sizeof(t))==sizeof(t)) {
			//printf("key %d %d\n",t.code,t.value);
			if(t.code==0 && t.value!=0)
				if(mouse_X>=0 && mouse_X<=480){
					mouse_X=mouse_X+t.value;
					//printf("X=%d Y=%d\n",mouse_X,mouse_Y);
				}
			if(t.code==1 && t.value!=0)
				if(mouse_Y>=0 && mouse_Y<=272){
					mouse_Y=mouse_Y+t.value;
					//printf("X=%d Y=%d\n",mouse_X,mouse_Y);
				}
			if(t.code==272 && t.value==1){
				mouse_state=1;
				//printf("Left Click\n");
			}
			if(t.code==273 && t.value==1){
				mouse_state=2;
				//printf("Right Click\n");
			}
			if(mouse_X<0)mouse_X=0;
			if(mouse_X>480)mouse_X=479;
			if(mouse_Y<0)mouse_Y=0;
			if(mouse_Y>272)mouse_Y=271;
			
		}
	}	
	close(keys_fd);
	return 0;
}




