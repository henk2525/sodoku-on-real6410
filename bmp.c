// http://ccckmit.wikidot.com/cp:bmp
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef long UINT32;
typedef unsigned short int UINT16;
typedef unsigned char BYTE;

#define HEADER_SIZE 14
#define INFO_SIZE 40
#define DataSize(bmp) (bmp->width*bmp->height*3)

typedef struct BMP {
    BYTE header[HEADER_SIZE];    
    BYTE info[INFO_SIZE];
    char *filename;
    // Header
    UINT16 signature; // Magic Number = "BM" = 0x4D42
    UINT32 fileSize; // File size in bytes
    UINT32 hreserved; // unused (=0)
    UINT32 dataOffset; // File offset to Raster Data
    // InfoHeader
    UINT32 size; // Size of InfoHeader =40 
    UINT32 width; // Bitmap Width
    UINT16 height; // Bitmap Height
    UINT16 planes; // Number of Planes (=1)
    UINT16 bitsPerPixel; // Bits per Pixel, 1, 4, 8, 16, 24
    UINT32 compression; // Type of Compression, 0 = BI_RGB no compression, 1 = BI_RLE8 8bit RLE encoding, 2 = BI_RLE4 4bit RLE encoding
    UINT32 imageSize; // (compressed) Size of Image, It is valid to set this =0 if Compression = 0
    UINT32 xPixelsPerM; // horizontal resolution: Pixels/meter
    UINT32 yPixelsPerM; // vertical resolution: Pixels/meter
    UINT32 colorsUsed; // Number of actually used colors
    UINT32 colorsImportant; // Number of important colors , 0 = all
    // ColorTable : present only if Info.BitsPerPixel <= 8 colors should be ordered by importance
    BYTE blue; // Blue intensity
    BYTE green; // Green intensity
    BYTE red; // Red intensity
    BYTE creserved; // unused (=0)
    // Raster Data
    BYTE *data;
    BYTE *dataYUV;
} BMP;

typedef struct Pixel {
    BYTE R;
    BYTE G;
    BYTE B;
} Pixel;

#define U16(x)    ((unsigned short) (x))
#define U32(x)    ((int) (x))
#define B2U16(bytes,offset)  (U16(bytes[offset]) | U16(bytes[offset+1]) << 8)
#define B2U32(bytes,offset)  (U32(bytes[offset]) | U32(bytes[offset+1]) << 8 | \
                   U32(bytes[offset+2]) << 16 | U32(bytes[offset+3]) << 24)

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

int bmpLoad(BMP *bmp, char *fileName) {
   FILE *file;
   BYTE header[14];
   BYTE info[40];
   bmp->filename=fileName;
   if ((file = fopen(fileName, "rb") ) == NULL ) {
      fprintf(stderr, "Error: bmpLoad(), File open fail!\n");
      exit(0);
   }

   fread(header, 1, HEADER_SIZE, file);
   fread(info, 1, INFO_SIZE, file);
   memcpy(bmp->header, header, HEADER_SIZE);
   memcpy(bmp->info, info, INFO_SIZE);
   // Header
   bmp->signature = B2U16(header,0); assert(bmp->signature == 0x4D42);
   bmp->fileSize = B2U32(header,2);
   bmp->dataOffset = B2U32(header,10);
   // InfoHeader
   bmp->size = B2U32(info,0); assert(bmp->size==40);
   bmp->width = B2U32(info,4);
   bmp->height = B2U32(info,8);
   bmp->planes = B2U16(info,12); assert(bmp->planes==1);
   bmp->bitsPerPixel = B2U16(info,14); assert(bmp->bitsPerPixel==24);
   bmp->compression = B2U32(info,16);
   bmp->imageSize = B2U32(info,20);
   bmp->xPixelsPerM = B2U32(info,24);
   bmp->yPixelsPerM = B2U32(info,28);
   bmp->colorsUsed = B2U32(info,32);
   bmp->colorsImportant = B2U32(info,36);

   bmp->data = malloc(DataSize(bmp));
   fseek(file, bmp->dataOffset, SEEK_SET);
   fread(bmp->data, 1, DataSize(bmp), file);
//RGB to YUV
    int i=0,PR,PG,PB,T2,T1;
    bmp->dataYUV=malloc(DataSize(bmp)*2/3);
    for(i=0;i<DataSize(bmp);i=i+3){
	PR=bmp->data[i+2]/8;	PR=MAX(MIN(PR,31),0);
	PG=bmp->data[i+1]/4;	PG=MAX(MIN(PG,63),0);
	PB=bmp->data[i+0]/8;	PB=MAX(MIN(PB,31),0);
	T2=PR*8+PG/8;		T2=MAX(MIN(T2,255),0);
	T1=(PG%8)*32+PB;	T1=MAX(MIN(T1,255),0);
        bmp->dataYUV[i*2/3+0]=T1;
        bmp->dataYUV[i*2/3+1]=T2;
    }
//
   fclose (file);
}

int bmpPrint(BMP *bmp) {
   printf("%s\n",bmp->filename);
   printf("==== Header ====\n");
   printf("Signature = %04X\n", bmp->signature); // 0x4d42 = BM
   printf("FileSize = %ld \n", bmp->fileSize);
   printf("DataOffset = %ld \n", bmp->dataOffset);
   printf("==== Info ======\n");
   printf("size = %ld \n", bmp->size);
   printf("Width = %ld \n", bmp->width);
   printf("Height = %ld \n", bmp->height);
   printf("Planes = %d \n", bmp->planes);
   printf("BitsPerPixel = %d \n", bmp->bitsPerPixel);
   printf("Compression = %ld \n", bmp->compression);
   printf("ImageSize = %ld \n", bmp->imageSize);
   printf("XpixelsPerM = %ld \n", bmp->xPixelsPerM);
   printf("YpixelsPerM = %ld \n", bmp->yPixelsPerM);
   printf("ColorsUsed = %ld \n", bmp->colorsUsed);
   printf("ColorsImportant = %ld \n", bmp->colorsImportant);
}

/*
int main(int argc, char *argv[]) {
    BMP bmp;
    bmpLoad(&bmp, argv[1]);
    bmpPrint(&bmp);
    bmpSetBox(&bmp, 0, 0, 20, 20, 0x66, 0x33, 0x99);
    bmpSave(&bmp, argv[2]);
    return 0;
}*/

int bmpDisplay(BMP *bmp, int x0, int y0, unsigned char *vf, int w, int h){
    int x,y,i=0;

    for(y=y0+bmp->height-1;y>y0;y--){
        if(y<0 || y>=h);
        else
        for(x=x0;x<x0+bmp->width;x++){
            if(x<0 || x>=w);
            else{
            vf[(w*y+x)*2+0]=bmp->dataYUV[i+0];
            vf[(w*y+x)*2+1]=bmp->dataYUV[i+1];}
            i=i+2;
        }
    }
}
#include <math.h>
int bmpDisplayAdv(BMP *bmp, int x0, int y0, unsigned char *vf, int w, int h, BMP *mask, float alpha){
    int x,y,i=0,i2=0,ind,PR,PG,PB,T1,T2;
    int xt,yt;
    float maskAlpha=1.0,totalAlpha=1.0;
    for(y=bmp->height-1;y>=0;y--){
        for(x=0;x<bmp->width;x++){
            i2=(bmp->width*y+x)*3;
            xt=x+x0;
            yt=-y+y0+bmp->height;
            ind=((w*(yt)+(xt))*2);
            if(0<=yt && yt<h && 0<=xt && xt<w && 0<=i2 && i2<=DataSize(bmp)){
                    maskAlpha=(255.0-(float)((mask->data[i2+0]+mask->data[i2+1]+mask->data[i2+2])/3))/255;
//printf("%f %f ",maskAlpha,alpha);
                    totalAlpha=MAX(MIN(sqrt(maskAlpha*alpha),1),0);
//printf("%f\n",totalAlpha);
                        PR=(vf[ind+1]/8)*			(1-totalAlpha)+	(bmp->data[i2+2]/8)*totalAlpha;			
                        PG=((vf[ind+1]%8)*8+vf[ind+0]/32)*	(1-totalAlpha)+	(bmp->data[i2+1]/4)*totalAlpha;	
                        PB=(vf[ind+0]%32)*			(1-totalAlpha)+	(bmp->data[i2+0]/8)*totalAlpha;			
                        //PR=MAX(MIN(PR,31),0);
                        //PG=MAX(MIN(PG,63),0);
                        //PB=MAX(MIN(PB,31),0);
                        T2=PR*8+PG/8;		//T2=MAX(MIN(T2,255),0);
                        T1=(PG%8)*32+PB;	//T1=MAX(MIN(T1,255),0);
                        vf[ind+0]=T1;
                        vf[ind+1]=T2;
            }
        }
    }
}

int bmpBG(unsigned char *vf, int w, int h,BMP *bmp){
    unsigned int x,y,ind,i,PR,PG,PB;
    bmp->width=w;
    bmp->height=h;
    bmp->dataYUV = malloc(w*h*2);
    for(y=0;y<h;y++){
        for(x=0;x<w;x++){
	    ind=(y*w+x)*2;
	    i=((h-y-1)*w+x)*2;
	    bmp->dataYUV[i+0]=vf[ind+0];
	    bmp->dataYUV[i+1]=vf[ind+1];
	}
    }
}

