//bahri abacý BMP kütüphanesi projesi
//BMP dosyalarýndan veri okumak yeni BMP dosyalarý yazmak için kullanýlýr
#ifndef cbmp_h
#define cbmp_h

#define pi 3.1415926535897932
#define rbyte(x) (x>255 ? 255 : (x>0 ? (byte)x : 0))

#define MAT_SAT(A)	(*(A-3))
#define	MAT_SUT(A)	(*(A-2))
#define	MAT_BOY(A)	(*(A-1))

typedef unsigned char  byte;//1 byte
typedef unsigned short word;//2 byte
typedef unsigned int   dword; //4 byte
typedef	double       **CMAT;//Double Matrix

typedef struct MAT{
        dword satir; // matriz row number
        dword sutun; // matrix coloumn number
        dword boyut; // matrix dimension
        double *mat; // 1-D matrix data  
        };

typedef struct BMPH {
        word  bftype;  //bitmap indicator must be 'BM'
        dword bfsize;  //bitmap file size
        word  bfres1;  //reseved bytes
        word  bfres2;  //reserved bytes
        dword bfoffset;//start byte of colors
        };
        
typedef struct BMIH {
        dword bisize; //bitmap info size
        dword width;  //x pixels of bmp
        dword height; //y pixels of bmp
        word  biplane;//number of planes set to 1
        word  bppixel;//bit per pixel
        dword comtype;//compression method
        dword rawsize;//total bytes of color and padding
        dword xpmeter;//x pixel per meter-horizontal resolution
        dword ypmeter;//y pixel per meter-vertical   resolution 
        dword ncolors;//number of colors in the palette
        dword icolors;//important colors      
        };

typedef struct RGBA {
        byte  blue;
        byte  green;
        byte  red;
        byte  alpha; 
        };

typedef struct HSV  {
        byte hue;
        byte saturation;
        byte value; };         

typedef struct  BMP {
        BMPH  header;
        BMIH  bminfo;
        RGBA **pixels; };
 
dword pow2(int t);
byte max(byte,byte,byte);
byte min(byte,byte,byte);   
void sirala(int dizi[]);
void sirala(double dizi[],int index[], int n);

long int kimebagli(long int* baglistesi,long int deger);

BMP   resim_oku(char* dosya_adi);
bool  resim_sil(BMP kaynak);
BMP   yenim_bmp(dword en,dword boy);//24bit bmp yarat
BMP   resim_boy(BMP kaynak,word yenien,word yeniboy);
BMP   histo_esi(BMP kaynak);
BMP   histo_ger(BMP kaynak,int alt,int ust);
BMP   resim_iki(BMP kaynak,byte esik);
bool  resim_yaz(BMP kaynak,char* dosya_adi);
byte  resim_max(BMP kaynak);
byte  resim_min(BMP kaynak);
byte  otsus_esi(BMP kaynak);
byte  gbyte_yap(double); 
BMP   resim_hsv(BMP kaynak);//rgb2hsv int tersi için
BMP   resim_rgb(BMP kaynak);
BMP   resim_kon(BMP kaynak,double *filt,int k);//3d matris konvolusyon al
BMP   resim_kes(BMP kaynak,int x1,int y1,int width,int height);
BMP   resim_ero(BMP kaynak,byte* strel,byte strellen);
BMP   resim_dil(BMP kaynak,byte* strel,byte strellen);
BMP   resim_bbe(BMP kaynak,int *labels);
BMP   resim_yiq(BMP kaynak);//rgb2yiq y deðeri gri tonlama deðeri
BMP   resim_ycb(BMP kaynak);
BMP   resim_tek(BMP kaynak, int ch);
BMP   resim_rnk(BMP kaynak, int size, double rnk);
BMP   resim_far(BMP k1,BMP k2,int absd);

double* resim_dou(BMP kaynak);
BMP   resim_bmp(double *, int,int,int);
BMP   resim_m2l(BMP,double*,int);

CMAT  resim_kum(BMP kaynak);
double*  gabor_fil(double lamda,double sigma,double gama,double fi,double theta,short int normalize,short int size);

BMP   resim_don(BMP* kaynak,double perspektif[3][3]);//3d matris perspektif ayarý

        
#include "cbmp.cpp"        
#endif
