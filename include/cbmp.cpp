#include <iostream>
#include <fstream>
#include <cstring>
//#include <windows.h>
#include <math.h>
#include "cbmp.h"
using namespace std;
dword pow2(int t) {
      byte i;
      dword result=1;
      for(i=0;i<t;i++) {result*=2;}
      return result; }
 
BMP resim_oku(char *dosya_adi) {
     BMP im;
     ifstream bmp(dosya_adi,ios::binary);
     bmp.read((char*)&im.header.bftype,2);
     bmp.read((char*)&im.header.bfsize,4);
     bmp.read((char*)&im.header.bfres1,2);
     bmp.read((char*)&im.header.bfres2,2);
     bmp.read((char*)&im.header.bfoffset,4);
     
     bmp.read((char*)&im.bminfo.bisize,4 );
     bmp.read((char*)&im.bminfo.width,4  );
     bmp.read((char*)&im.bminfo.height,4 );
     bmp.read((char*)&im.bminfo.biplane,2);
     bmp.read((char*)&im.bminfo.bppixel,2);
     bmp.read((char*)&im.bminfo.comtype,4);
     bmp.read((char*)&im.bminfo.rawsize,4);
     bmp.read((char*)&im.bminfo.xpmeter,4);
     bmp.read((char*)&im.bminfo.ypmeter,4);
     bmp.read((char*)&im.bminfo.ncolors,4);
     bmp.read((char*)&im.bminfo.icolors,4);
     
     int i=0,j=0;
     byte padding=4-(3*im.bminfo.width)%4;
     if(padding==4) { padding=0; }
     bmp.seekg((int)im.header.bfoffset,ios::beg); //cursor sets the start of colors
     
     im.pixels=new RGBA* [im.bminfo.width];
     for(i=0;i<im.bminfo.width;i++) { im.pixels[i]=new RGBA [im.bminfo.height]; }
          
     word buffersize=im.bminfo.width*3+padding;
     byte *buffer;
     buffer=new byte [buffersize];
     j=im.bminfo.height-1;
     while(j>-1)     {
     bmp.read((char*)buffer,buffersize);
     for( i=0 ; i < im.bminfo.width ; i++ ) { memcpy( (char*) &(im.pixels[i][j]), buffer+3*i, 3 ); }
     j--;            }
     delete [] buffer;
     bmp.close();
     return im;                    } 
   
BMP  resim_boy(BMP kaynak,word yenien,word yeniboy) {
     if(yenien<1 || yeniboy<1) {
     cout <<"Yukseklik ve Genislik Pozitif Olmali!"<<endl;
     return kaynak;    }
     BMP im=yenim_bmp(yenien,yeniboy);
     int i,j;
     
     double a11=(double)kaynak.bminfo.width/(double)yenien;
     double a22=(double)kaynak.bminfo.height/(double)yeniboy;
     //rescaling pixels

     for(j=0;j<yeniboy;j++) {
                       for(i=0;i<yenien;i++) {
    
     im.pixels[i][j]=kaynak.pixels[(dword)(a11*i)][(dword)(a22*j)];
      }}
     //new width and height setting
     return im; }     
    
bool resim_yaz(BMP kaynak,char *dosya_adi) { 
     ofstream bmp(dosya_adi,ios::binary);
     
     int i,j;
     byte padding=4-(3*kaynak.bminfo.width)%4;
     if(padding==4) { padding=0; }
     dword row=padding+(kaynak.bminfo.bppixel*kaynak.bminfo.width)/8;
     
     bmp.write((char*)&kaynak.header.bftype,2);
     bmp.write((char*)&kaynak.header.bfsize,4);
     bmp.write((char*)&kaynak.header.bfres1,2);
     bmp.write((char*)&kaynak.header.bfres2,2);
     bmp.write((char*)&kaynak.header.bfoffset,4);
     bmp.write((char*)&kaynak.bminfo.bisize,4);
     bmp.write((char*)&kaynak.bminfo.width,4);
     bmp.write((char*)&kaynak.bminfo.height,4);
     bmp.write((char*)&kaynak.bminfo.biplane,2);
     bmp.write((char*)&kaynak.bminfo.bppixel,2);
     bmp.write((char*)&kaynak.bminfo.comtype,4);
     bmp.write((char*)&kaynak.bminfo.rawsize,4);
     bmp.write((char*)&kaynak.bminfo.xpmeter,4);
     bmp.write((char*)&kaynak.bminfo.ypmeter,4);
     bmp.write((char*)&kaynak.bminfo.ncolors,4);
     bmp.write((char*)&kaynak.bminfo.icolors,4);

     word buffersize=kaynak.bminfo.width*3+padding;
     byte *buffer;
     buffer=new byte [buffersize];
     for(i=0;i<buffersize;i++) { buffer[i]=0; }
     
     j=kaynak.bminfo.height-1;
     while(j>-1) {
     for(i=0;i<kaynak.bminfo.width;i++) { memcpy( (char*) buffer+3*i,  (char*) &(kaynak.pixels[i][j]), 3 );  }                    
     bmp.write((char*)buffer,buffersize);
     //bmp.write((char*)&pad,padding);
     j--;       }
     delete [] buffer;
     return true;                                                           
      }
BMP  histo_esi(BMP kaynak) {
    
    BMP im=yenim_bmp(kaynak.bminfo.width,kaynak.bminfo.height);    
    dword i=0,j=0;
    dword *histogramred;
    dword *histogramgreen;
    dword *histogramblue;
    histogramred= new dword [256];
    histogramgreen= new dword [256];
    histogramblue= new dword [256];
    
    for( i=0 ; i < 256 ; i++) { 
         histogramred[i]=0;
         histogramgreen[i]=0;
         histogramblue[i]=0; }
    
    for( j=0 ; j < kaynak.bminfo.height ; j++)
    {
    for( i=0 ; i < kaynak.bminfo.width ; i++)
      {     
         histogramred[(int)kaynak.pixels[i][j].red]++;
         histogramgreen[(int)kaynak.pixels[i][j].green]++;
         histogramblue[(int)kaynak.pixels[i][j].blue]++;//histogram ��kart�l�yor                                    
      }
    }

    for( j=0; j <255;j++) {
         histogramred[j+1]+=histogramred[j];
         histogramgreen[j+1]+=histogramgreen[j];
         histogramblue[j+1]+=histogramblue[j];
          } //k�m�latif histogram hesapla
         
    for( j=0; j <256;j++) { 
         histogramred[j]=((int)histogramred[j]*255)/(kaynak.bminfo.width*kaynak.bminfo.height);
         histogramgreen[j]=((int)histogramgreen[j]*255)/(kaynak.bminfo.width*kaynak.bminfo.height);
         histogramblue[j]=((int)histogramblue[j]*255)/(kaynak.bminfo.width*kaynak.bminfo.height);
          } //normalize etme i�lemi
    
    for( j=0 ; j < kaynak.bminfo.height ; j++)
    {
    for( i=0 ; i < kaynak.bminfo.width ; i++)
      {       
    im.pixels[i][j].red = histogramred[(int)kaynak.pixels[i][j].red]; 
    im.pixels[i][j].green = histogramgreen[(int)kaynak.pixels[i][j].green]; 
    im.pixels[i][j].blue = histogramblue[(int)kaynak.pixels[i][j].blue]; 
      }
    }
    delete [] histogramred;
    delete [] histogramgreen;
    delete [] histogramblue;
    return im;
    }

BMP  yenim_bmp(dword en,dword boy) {
     BMP im; 
     int i,j;
     byte padding=4-(3*en)%4;
     if(padding==4) { padding=0; }
     dword row=padding+(24*en)/8;
     
     im.header.bftype=19778;
     im.header.bfsize=(dword)(54+row*boy);
     im.header.bfres1=0;
     im.header.bfres2=0;
     im.header.bfoffset=54;
     im.bminfo.bisize=40;
     im.bminfo.width=en;  //x pixels of bmp
     im.bminfo.height=boy; 
     im.bminfo.biplane=1;
     im.bminfo.bppixel=24;
     im.bminfo.comtype=0;
     im.bminfo.rawsize=row*boy;
     im.bminfo.xpmeter=0;
     im.bminfo.ypmeter=0;
     im.bminfo.ncolors=0;//pow2(24);
     im.bminfo.icolors=0; 
     im.pixels=new RGBA* [en];
     for(i=0;i<en;i++) { im.pixels[i]=new RGBA [boy]; }
     for(i=0;i<en;i++) {
                       for(j=0;j<boy;j++) {
                                          im.pixels[i][j].red=0;
                                          im.pixels[i][j].green=0;
                                          im.pixels[i][j].blue=0;
                                          im.pixels[i][j].alpha=0;} }
     return im; } 
         
BMP   histo_ger(BMP kaynak,int alt,int ust) {
      BMP im=yenim_bmp(kaynak.bminfo.width,kaynak.bminfo.height);
      dword i,j;
      byte maxi=resim_max(kaynak);
      byte mini=resim_min(kaynak);
      byte fark=maxi-mini;
      byte ciki=ust-alt;
      for( j=0 ; j < kaynak.bminfo.height ; j++)
      {
      for( i=0 ; i < kaynak.bminfo.width ; i++)
      {
      im.pixels[i][j].red=  (byte) ciki*(kaynak.pixels[i][j].red-mini)/fark+alt;
      im.pixels[i][j].green=(byte) ciki*(kaynak.pixels[i][j].green-mini)/fark+alt;
      im.pixels[i][j].blue= (byte) ciki*(kaynak.pixels[i][j].blue-mini)/fark+alt; }}
      
      return im;
      }

BMP   resim_iki(BMP kaynak,byte esik) {
      BMP im=yenim_bmp(kaynak.bminfo.width,kaynak.bminfo.height);
      dword i,j;
      for( j=0 ; j < kaynak.bminfo.height ; j++)
      {
      for( i=0 ; i < kaynak.bminfo.width ; i++)
      {
      if((0.299*kaynak.pixels[i][j].red+0.587*kaynak.pixels[i][j].green+0.114*kaynak.pixels[i][j].blue)>esik) { 
                                       im.pixels[i][j].red=255;
                                       im.pixels[i][j].green=255;
                                       im.pixels[i][j].blue=255; }}}
      return im;                                   
      }         
byte resim_max(BMP kaynak) {
     dword i,j;
     byte maxi=0,maxi1;
     for( j=0 ; j < kaynak.bminfo.height ; j++)
     {
     for( i=0 ; i < kaynak.bminfo.width ; i++)
     { 
             maxi1=max(kaynak.pixels[i][j].red,kaynak.pixels[i][j].green,kaynak.pixels[i][j].blue);
             if(maxi<maxi1) {maxi=maxi1;}
             if(maxi==255) { return maxi; } 
             }}
             return maxi; }

byte resim_min(BMP kaynak) {
     dword i,j;
     byte mini=255,mini1;
     for( j=0 ; j < kaynak.bminfo.height ; j++)
     {
     for( i=0 ; i < kaynak.bminfo.width ; i++)
     {  
             byte mini1=min(kaynak.pixels[i][j].red,kaynak.pixels[i][j].green,kaynak.pixels[i][j].blue);
             if(mini>mini1) {mini=mini1;}
             if(mini==0) { return mini; }
             }}
             return mini; }

byte  otsus_esi(BMP kaynak) {
    dword i,j;  
    dword *histogram;
    double ag1,ag2,ort1=0,ort2=0,sigma=0,agtop=0,toplam,toplam2;
    byte esik;
    double agortalama[256];
    histogram= new dword [256];
    
    for( i=0 ; i < 256 ; i++) { histogram[i]=0; }
    
    for( j=0 ; j < kaynak.bminfo.height ; j++)
    {
    for( i=0 ; i < kaynak.bminfo.width ; i++)
    {     
         histogram[(int) (0.3*kaynak.pixels[i][j].red+0.59*kaynak.pixels[i][j].green+0.11*kaynak.pixels[i][j].blue)]++;
         }}
    
    agtop=kaynak.bminfo.width*kaynak.bminfo.height;
    for( j=0; j <255;j++) { toplam2+=j*histogram[j]; } 
          
    for(j=0;j<256;j++) {     
         ag1+=histogram[j];
         if(ag1==0) { continue; }
         ag2=(agtop-ag1);
         if(ag2==0) { break; }
         
         toplam += (double) j*histogram[j];
         
         ort1=toplam/ag1;
         ort2=(toplam2-toplam)/ag2;
         
         double sigmakare=ag1*ag2*(ort1-ort2)*(ort1-ort2);
         
         if(sigmakare>sigma) { 
         sigma=sigmakare;
         esik=j;             }         
     }
     return esik; 
     }
                                        
bool resim_sil(BMP kaynak) {
     int i;
     for(i=0;i<kaynak.bminfo.width;i++) {delete [] kaynak.pixels[i]; }
     delete [] kaynak.pixels;
     kaynak.bminfo.width=0;
     kaynak.bminfo.height=0;
     return true; }

  
BMP    resim_hsv(BMP kaynak) {
     
       BMP im=yenim_bmp(kaynak.bminfo.width,kaynak.bminfo.height);
       for(int i=0;i<kaynak.bminfo.width;i++) {
                         for(int j=0;j<kaynak.bminfo.height;j++) {
       byte r=kaynak.pixels[i][j].red;
       byte g=kaynak.pixels[i][j].green;
       byte b=kaynak.pixels[i][j].blue;

       byte mx=max(r,g,b);
       byte mn=min(r,g,b);

       if(mx==mn) { 
       im.pixels[i][j].red=0;
       im.pixels[i][j].green=0;
       im.pixels[i][j].blue=mx;}
       else {
       if(mx==r) { im.pixels[i][j].red=gbyte_yap(43*(g-b)/(mx-mn)); }
       if(mx==g) { im.pixels[i][j].red=gbyte_yap(43*(b-r)/(mx-mn)+85); }
       if(mx==b) { im.pixels[i][j].red=gbyte_yap(43*(r-g)/(mx-mn)+171); }
       
       im.pixels[i][j].green=255*(mx-mn)/mx;
       
       im.pixels[i][j].blue=mx;
       }
        }}
       return im;  
       }


BMP resim_rgb(BMP kaynak) {
       
       BMP im=yenim_bmp(kaynak.bminfo.width,kaynak.bminfo.height);
       
       for(int i=0;i<kaynak.bminfo.width;i++) {
                         for(int j=0;j<kaynak.bminfo.height;j++) {
       byte h=kaynak.pixels[i][j].red;
       byte s=kaynak.pixels[i][j].green;
       byte v=kaynak.pixels[i][j].blue;
       /*
       byte mx=max(h,s,v);
       byte mn=min(h,s,v);
       */
       int deger= (int)(h/43)%6;
       double f= (double) ((double)h/43-(int)(h/43));
       int p=(int)(v*(255-s)/255);
       int q=(int)(v*(255-f*s)/255);
       int t=(int)(v*(255-(1-f)*s)/255);
       
       if(deger==0) { 
                    im.pixels[i][j].red=v;
                    im.pixels[i][j].green=t;
                    im.pixels[i][j].blue=p; }
       else if(deger==1) { 
                    im.pixels[i][j].red=q;
                    im.pixels[i][j].green=v;
                    im.pixels[i][j].blue=p; }
       else if(deger==2) { 
                    im.pixels[i][j].red=p;
                    im.pixels[i][j].green=v;
                    im.pixels[i][j].blue=t; }             
       else if(deger==3) { 
                    im.pixels[i][j].red=p;
                    im.pixels[i][j].green=q;
                    im.pixels[i][j].blue=v; }
       else if(deger==4) { 
                    im.pixels[i][j].red=t;
                    im.pixels[i][j].green=p;
                    im.pixels[i][j].blue=v; }
       else if(deger==5) { 
                    im.pixels[i][j].red=v;
                    im.pixels[i][j].green=p;
                    im.pixels[i][j].blue=q; }
                    }}             
       return im;
       }

BMP resim_kon(BMP kaynak,double *filt,int k) {
    
    BMP im=yenim_bmp(kaynak.bminfo.width,kaynak.bminfo.height);
    BMP kaynako=yenim_bmp(kaynak.bminfo.width+k-1,kaynak.bminfo.height+k-1);
    int i,j,m,n;
    double red,green,blue;
            
     for (i=0;i < kaynak.bminfo.width;i++) {
         for (j=0;j < kaynak.bminfo.height;j++) {           
     kaynako.pixels[i+(k-1)/2][j+(k-1)/2].red=kaynak.pixels[i][j].red;
     kaynako.pixels[i+(k-1)/2][j+(k-1)/2].green=kaynak.pixels[i][j].green;
     kaynako.pixels[i+(k-1)/2][j+(k-1)/2].blue=kaynak.pixels[i][j].blue; }}
     
     for (i=0;i < kaynak.bminfo.width;i++) {
         for (j=0;j < kaynak.bminfo.height;j++) { 
             for (m=0;m < k; m++) {
                 for (n=0;n < k; n++) {
     
     red+=kaynako.pixels[i+m][j+n].red*(*(filt+n+m*k));
     green+=kaynako.pixels[i+m][j+n].green*(*(filt+n+m*k));
     blue+=kaynako.pixels[i+m][j+n].blue*(*(filt+n+m*k));
     }}
     im.pixels[i][j].red  =gbyte_yap(red   <0 ? -red:red);
     im.pixels[i][j].green=gbyte_yap(green <0 ? -green:green);
     im.pixels[i][j].blue =gbyte_yap(blue  <0 ? -blue:blue);
     red=0;
     green=0;
     blue=0;
     }}
            
     return im; 
     }
     
BMP  resim_ero(BMP kaynak,byte* strel,byte strellen) {
     
    BMP im=yenim_bmp(kaynak.bminfo.width,kaynak.bminfo.height);
    int i,j,m,n;
    dword sum=0;
    byte sumstrel=0;
    for(i=0;i<strellen*strellen;i++) {sumstrel+=*(strel+i);}        
     
     for (i=0;i < kaynak.bminfo.width-strellen;i++) {
         for (j=0;j < kaynak.bminfo.height-strellen;j++) { 
             for (m=0;m < strellen; m++) {
                 for(n=0;n < strellen;n++) {
     
     sum+=kaynak.pixels[i+m][j+n].red*(*(strel+m*strellen+n));
                                          } }
     if (sum==sumstrel*255) {
                                     
     im.pixels[i+(byte)(strellen/2)][j+(byte)(strellen/2)].red = 255;                             
     im.pixels[i+(byte)(strellen/2)][j+(byte)(strellen/2)].green = 255;
     im.pixels[i+(byte)(strellen/2)][j+(byte)(strellen/2)].blue = 255;                                }
     
     sum=0;
     }}
            
     return im;     
     
     }
     
BMP  resim_dil(BMP kaynak,byte* strel,byte strellen) {
     
    BMP im=yenim_bmp(kaynak.bminfo.width,kaynak.bminfo.height);
    int i,j,m,n;
    dword sum=0;       
     
     for (i=0;i < kaynak.bminfo.width-strellen;i++) {
         for (j=0;j < kaynak.bminfo.height-strellen;j++) { 
             for (m=0;m < strellen; m++) {
                 for(n=0;n < strellen;n++) {
     
     sum+=kaynak.pixels[i+m][j+n].red*(*(strel+m*strellen+n));
                                          } }
     if (sum>254) {
                                     
     im.pixels[i+(byte)(strellen/2)][j+(byte)(strellen/2)].red = 255;                             
     im.pixels[i+(byte)(strellen/2)][j+(byte)(strellen/2)].green = 255;
     im.pixels[i+(byte)(strellen/2)][j+(byte)(strellen/2)].blue = 255;                                }
     
     sum=0;
     }}
            
     return im;       
     
     }     

BMP   resim_kes(BMP kaynak,int x1,int y1,int width,int height) {
      
      BMP im=yenim_bmp(width,height);
      
      for(int i=x1;i<x1+width;i++) {
            for(int j=y1;j<y1+height;j++){
                  
                  im.pixels[i-x1][j-y1].blue=kaynak.pixels[i][j].blue;
                  im.pixels[i-x1][j-y1].green=kaynak.pixels[i][j].green;
                  im.pixels[i-x1][j-y1].red=kaynak.pixels[i][j].red;
                  im.pixels[i-x1][j-y1].alpha=kaynak.pixels[i][j].alpha;
                  }}
      
      return im;
      }

// tum baglantilari gez ve max root olani min root ile degistir
void resolve(int *list, int u, int v, int N)
{

      int i = 0;
      int m = min(list[u], list[v]);
      int n = max(list[u], list[v]);

      for (i = 1; i < N; i++)
      {
            if (list[i] == n)
            {
                  list[i] = m;
            }
      }
}

int fast_ccl(long int *matrix, int width, int height)
{

      /* Fast connected component labeling algorithm
Algorithm is based on two pass scan algorithms
Inputs:
matrix: MxN image contains zero and one as Value background(Vb) and Value object (Vo)
list  : 1xD image which D is the number of maximum object number (max: MxN/4)

Output: MxN image contains numbers from 0 to D, 0 is the background and D is the maximum object number
Return: D
*/
      int Vb = 0; //background
      int Vo = 1; //foreground

      int NewLabel = 2; //new label iterator
      int c1, c2, c3, c4, lx;

      int i, j, idx;

      // initialize list in range (0,N-1)
      int MaxCC = width * height / 4;
      int *list = (int *)malloc(MaxCC * sizeof(int));
      for (i = 0; i < MaxCC; i++)
      {
            list[i] = i;
      }

      //Provisional labels are assigned to matris
      for (j = 1; j < height - 1; j++)
      {
            idx = j * width;
            for (i = 1; i < width - 1; i++)
            {
                  //I(i,j)==1 ?
                  if (matrix[i + idx] != Vo)
                  {
                        continue;
                  }

                  c1 = matrix[i + idx - 1];
                  c2 = matrix[i + idx - 1 - width];
                  c3 = matrix[i + idx - width];
                  c4 = matrix[i + idx + 1 - width];

                  if (c3 != Vb)
                  {
                        lx = c3;
                  }
                  else if (c1 != Vb)
                  {
                        lx = c1;
                        if (c4 != Vb)
                        {
                              resolve(list, c1, c4, NewLabel);
                        }
                  } //resolve
                  else if (c2 != Vb)
                  {
                        lx = c2;
                        if (c4 != Vb)
                        {
                              resolve(list, c2, c4, NewLabel);
                        }
                  } //resolve
                  else if (c4 != Vb)
                  {
                        lx = c4;
                  }
                  else
                  {
                        lx = NewLabel;
                        NewLabel++;
                  }

                  matrix[i + idx] = lx;
            }
      } //end of the i,j loop

      // Create Consecutive Label
      MaxCC = 0;
      for (i = 1; i < NewLabel; i++)
      {
            if (i == list[i])
            {
                  list[i] = MaxCC;
                  MaxCC++;
            }
            else
            {
                  list[i] = list[list[i]];
            }
      }
      // Update matrix
      for (j = 1; j < height - 1; j++)
      {
            idx = j * width;
            for (i = 1; i < width - 1; i++)
            {
                  if (matrix[i + idx] == Vb)
                  {
                        continue;
                  }
                  matrix[i + idx] = list[matrix[i + idx]];
            }
      }

      free(list);
      return MaxCC - 1;
}

BMP resim_bbe(BMP kaynak, int *etiketsayisi)
{
      BMP im = yenim_bmp(kaynak.bminfo.width, kaynak.bminfo.height);

      long int *matris = (long int*)calloc(kaynak.bminfo.width * kaynak.bminfo.height, sizeof(long int));

      // I is a MxN logical image
      int i,j, idx = 0;
      for (j = 0; j < kaynak.bminfo.height; j++)
      {
            for (i = 0; i < kaynak.bminfo.width; i++)
            {
                  matris[idx++] = kaynak.pixels[i][j].red > 0;
            }
      }

      // do the actual computation
      (*etiketsayisi) = fast_ccl(matris, kaynak.bminfo.width, kaynak.bminfo.height);

      // printf("etiketsayisi: %d\n", etiketsayisi[0]);

      idx = 0;
      for (j = 0; j < kaynak.bminfo.height; j++)
      {
            for (i = 0; i < kaynak.bminfo.width; i++)
            {
                  int label = matris[idx++];

                  im.pixels[i][j].red = (label & 0x0000ff);
                  im.pixels[i][j].green = ((label & 0x00ff00) >> 8);
                  im.pixels[i][j].blue = ((label & 0xff0000) >> 16);
            }
      }

      free(matris);

      return im;
}
            
double*  gabor_fil(double lamda,double sigma,double gama,double fi,double theta,short int normalize,short int size) {
     
            double sx,sy;
            int x,y;
            short int indis=0;
            
            if(size==1) {gama<1 & gama>0 ? size=(short int) (8*sigma/gama) : size=(short int) (8*sigma);}
            if(size%2==0) {size=size+1;}
            
            double *gb;
            gb=new double [size*size];
                        
            for(x=(int)-(size-1)/2;x<(int)(size+1)/2;x++) { for(y=(int)-(size-1)/2;y<(int)(size+1)/2;y++) {
                                                          
                     sx=x*cos(theta)+y*sin(theta);
                     sy=-x*sin(theta)+y*cos(theta);
       
                     *(gb+indis)=exp(-(sx*sx+gama*gama*sy*sy)/(2*sigma*sigma))*cos(2*pi*sx/lamda+fi);
                     indis++;
                                                                }}
            if(normalize==1) {                                                    
            /*Normalization*/                                                    
            double norm=0;
            double possum=0,negsum=0;
            
            for(int i=0;i<size;i++) { for(int j=0;j<size;j++) {
                    if(gb[i*size+j]>0) {
                     possum+=gb[i*size+j];}
                    else {
                     negsum+=gb[i*size+j];}
                     }}
            norm=0.5*(possum-negsum);

            for(int i=0;i<size;i++) { for(int j=0;j<size;j++) {
                    
                    if(gb[i*size+j]>0) {
                     gb[i*size+j]=-1*gb[i*size+j]*negsum/norm;}
                    else if(gb[i*size+j]<0) {
                     gb[i*size+j]=gb[i*size+j]*possum/norm;}
                     }}
              
           /*Normalization*/
           }
           return gb;
     }
/*     
BMP   resim_rnk(BMP kaynak, int size, double rnk) { 
      
      BMP im = yenim_bmp(kaynak.bminfo.width, kaynak.bminfo.height);
      
      byte red[size][size];
      byte gre[size][size];
      byte blu[size][size];
      double index[size*size];
      int i,j,m,n;
      
      for (i=0;i < kaynak.bminfo.width-size;i++) {
         for (j=0;j < kaynak.bminfo.height-size;j++) { 
             for (m=0;m < size; m++) {
                 for (n=0;n < size; n++) {
                 
                 red[m][n] = kaynak.pixels[i+m][j+n].red;
                 gre[m][n] = kaynak.pixels[i+m][j+n].green;
                 blu[m][n] = kaynak.pixels[i+m][j+n].blue; }}
                 
      sirala(red,index,size*size);    
      sirala(gre,index,size*size);
      sirala(blu,index,size*size);
             
      im.pixels[i+(int) size/2][j+(int) size/2].red   = red[(int)rnk*size];
      im.pixels[i+(int) size/2][j+(int) size/2].green = gre[(int)rnk*size];
      im.pixels[i+(int) size/2][j+(int) size/2].blue  = blu[(int)rnk*size];
      }}
      
      return im;
}
*/
BMP   resim_yiq(BMP kaynak) {
      
      BMP im=yenim_bmp(kaynak.bminfo.width,kaynak.bminfo.height);
      
      for(int j=0;j<kaynak.bminfo.height;j++) { for(int i=0;i<kaynak.bminfo.width;i++) {
                            
            im.pixels[i][j].red   =  gbyte_yap(     0.30*kaynak.pixels[i][j].red+0.59*kaynak.pixels[i][j].green+0.11*kaynak.pixels[i][j].blue);
            im.pixels[i][j].green =  gbyte_yap(128+ 0.60*kaynak.pixels[i][j].red+0.28*kaynak.pixels[i][j].green-0.32*kaynak.pixels[i][j].blue);
            im.pixels[i][j].blue  =  gbyte_yap(128+ 0.21*kaynak.pixels[i][j].red-0.52*kaynak.pixels[i][j].green+0.31*kaynak.pixels[i][j].blue);
            }}
      return im;
      
} 
      
BMP   resim_ycb(BMP kaynak) {
      
      BMP im=yenim_bmp(kaynak.bminfo.width,kaynak.bminfo.height);
      
      for(int j=0;j<kaynak.bminfo.height;j++) { for(int i=0;i<kaynak.bminfo.width;i++) {
                            
            im.pixels[i][j].red   = gbyte_yap ( 0.299*kaynak.pixels[i][j].red+0.587*kaynak.pixels[i][j].green+0.114*kaynak.pixels[i][j].blue) ;
            im.pixels[i][j].green = gbyte_yap (128 -0.169*kaynak.pixels[i][j].red-0.331*kaynak.pixels[i][j].green+0.500*kaynak.pixels[i][j].blue) ;
            im.pixels[i][j].blue  = gbyte_yap (128 +0.500*kaynak.pixels[i][j].red-0.419*kaynak.pixels[i][j].green-0.081*kaynak.pixels[i][j].blue) ;
            }}
      return im;
      
      } 
      
BMP   resim_tek(BMP kaynak,int ch) {
      
      BMP im=yenim_bmp(kaynak.bminfo.width,kaynak.bminfo.height);
      if(ch==1) {
      for(int j=0;j<kaynak.bminfo.height;j++) { for(int i=0;i<kaynak.bminfo.width;i++) {
                            
            im.pixels[i][j].red   = (byte) kaynak.pixels[i][j].red ;
            im.pixels[i][j].green = (byte) kaynak.pixels[i][j].red ;
            im.pixels[i][j].blue  = (byte) kaynak.pixels[i][j].red ;
            }}}
      else if(ch==2) {      
      for(int j=0;j<kaynak.bminfo.height;j++) { for(int i=0;i<kaynak.bminfo.width;i++) {
                            
            im.pixels[i][j].red   = (byte) kaynak.pixels[i][j].green ;
            im.pixels[i][j].green = (byte) kaynak.pixels[i][j].green ;
            im.pixels[i][j].blue  = (byte) kaynak.pixels[i][j].green ;
            }}}
      else if(ch==3) {
      for(int j=0;j<kaynak.bminfo.height;j++) { for(int i=0;i<kaynak.bminfo.width;i++) {
                            
            im.pixels[i][j].red   = (byte) kaynak.pixels[i][j].blue ;
            im.pixels[i][j].green = (byte) kaynak.pixels[i][j].blue ;
            im.pixels[i][j].blue  = (byte) kaynak.pixels[i][j].blue ;
            }}} 
      else {
      for(int j=0;j<kaynak.bminfo.height;j++) { for(int i=0;i<kaynak.bminfo.width;i++) {
                            
            im.pixels[i][j].red   = (byte) kaynak.pixels[i][j].red ;
            im.pixels[i][j].green = (byte) kaynak.pixels[i][j].green ;
            im.pixels[i][j].blue  = (byte) kaynak.pixels[i][j].blue ;
            }}}   
                        
      return im;
      }         

CMAT yenim_mat( int satir, int sutun , int boyut=1)

{
	MAT	*mat;
	int 	i, j;
	satir++;
	sutun++;
    // MAT i�in bellek al     satir i�in adres  + sutun i�in adres  + matrisin sat�rlar� i�in adres al
	if ((mat = (MAT *)malloc( sizeof(long int ) + sizeof(long int ) + sizeof(double *) * satir)) == NULL)
		return (0);

	for (i=0; i<satir; i++)
	{//  Her satir i�in                       bir sutunluk bellek daha al
	if ((*( (double **)(&mat->mat) + i) = (double *)malloc(sizeof(double) * sutun * boyut)) == NULL)
		return (0);
	}
//system("pause");
	mat->satir = satir-1;
	mat->sutun = sutun-1;
	mat->boyut = boyut;

	return (&(mat->mat));// ald���n adresde matrix in adresini d�nd�r. A-3 -> satir, A-2 -> sutun, A-1 -> boyut, A -> matrix
}

CMAT resim_kum(BMP kaynak) {
         
   CMAT son = yenim_mat(kaynak.bminfo.width+1,kaynak.bminfo.height+1);
   double a,b,c,m;
   for(int j=0;j<kaynak.bminfo.height+1;j++) { for(int i=1;i<kaynak.bminfo.width+1;i++) {
            
            a = (i>0) ? son[i-1][j]:0;
            b = (j>0) ? son[i][j-1]:0;
            c = ((i>0)&&(j>0)) ? son[i-1][j-1]:0;
            m = ((i>0)&&(j>0)) ? (0.289*kaynak.pixels[i-1][j-1].red +  0.587*kaynak.pixels[i-1][j-1].green + 0.114*kaynak.pixels[i-1][j-1].blue) :0;
                      
            son[i][j] = m + a + b - c;
            
            }}
                        
   return son;       
}
                        
byte max(byte r,byte g,byte b) {
       byte max=r;
       if(g>max) { max=g; }
       if(b>max) { max=b; }
       return max; }
byte min(byte r,byte g,byte b) {
       byte min=r;
       if(g<min) { min=g; }
       if(b<min) { min=b; }
       return min; }

byte  gbyte_yap(double x) { return (byte)(x>255 ? 255 : (x>0 ? x : 0)); }   
     
void sirala(int dizi[]) {
     int temp=0,i=0;
     while(i<3) {
             if(dizi[i]>dizi[i+1]) {
             temp=dizi[i];
             dizi[i]=dizi[i+1];
             dizi[i+1]=temp;
             i=0; }
             else {i++;}}
                                 
     }                        

void sirala(double dizi[],int index[],int n) {
     
       double temp;
       int tempi;
       for (int i = 0; i < n; i++) { index[i] = i; }
       
       for (int i = 0; i < n; i++) {
           
        for (int j = 0; j < n-1; j++) {
         if(dizi[j] > dizi[j+1]) {
             
             tempi = index[j+1];       
             temp  = dizi[j+1];
             dizi[j+1] = dizi[j];
             index[j+1]= index[j];
             dizi[j] = temp;
             index[j]= tempi;
         }
     }}
     
}

double* resim_dou(BMP kaynak)
{
        double *out = (double*) malloc(3*kaynak.bminfo.width*kaynak.bminfo.height*sizeof(double));
        
        int i,j;
        int MN = kaynak.bminfo.width*kaynak.bminfo.height;
        
        for(i=0;i<kaynak.bminfo.width;i++) {
                                           
                                           int kts = kaynak.bminfo.height*i;
                                           
                                           for(j=0;j<kaynak.bminfo.height;j++) {
                                                                              *(out+j+kts)        = kaynak.pixels[i][j].red;
                                                                              *(out+MN + (j+kts)) = kaynak.pixels[i][j].green;
                                                                              *(out+2*MN +j+kts)  = kaynak.pixels[i][j].blue;
                                                                              }
                                           }
        return out;
}                        
           
BMP   resim_bmp(double *I, int M,int N,int normalize)
{
      BMP im = yenim_bmp(N,M);
      int i,j;
      
      for(i=0;i<N; i++) {
                                   
                       int kts = M*i;
                       
                       for(j=0;j<M;j++) {
                       
                                        //printf("[Ax+B]: %d-%d\n%d/%d\n",i,j,j+kts,M*N);
                                        im.pixels[i][j].red   = gbyte_yap(*(I+j+kts));
                                        im.pixels[i][j].green = gbyte_yap(*(I+M*N+j+kts));
                                        im.pixels[i][j].blue  = gbyte_yap(*(I+2*M*N+j+kts));
                                        }
                                        //printf("[Ax+B]: %d-%d\n%d/%d\n",i,j,j+kts,M*N);
                       }
                    
      return im;
}                

BMP   resim_cev(BMP kaynak,double tr[3][4]) {
      
      BMP im=yenim_bmp(kaynak.bminfo.width,kaynak.bminfo.height);
      int i=0;
      
      double tr00[256]; //1. kanal�n yeni 1. kanala etkisi
      double tr01[256]; //2. kanal�n yeni 1. kanala etkisi
      double tr02[256]; //3. kanal�n yeni 1. kanala etkisi 
      double tr03 = tr[0][3]; // 1. kanala eklenecek sabit de�er
      
      double tr10[256]; //1. kanal�n yeni 2. kanala etkisi
      double tr11[256]; //2. kanal�n yeni 2. kanala etkisi
      double tr12[256]; //3. kanal�n yeni 2. kanala etkisi 
      double tr13 = tr[1][3]; // 2. kanala eklenecek sabit de�er
      
      double tr20[256]; //1. kanal�n yeni 3. kanala etkisi
      double tr21[256]; //2. kanal�n yeni 3. kanala etkisi
      double tr22[256]; //3. kanal�n yeni 3. kanala etkisi 
      double tr23 = tr[2][3]; // 3. kanala eklenecek sabit de�er
      
      for(i=0;i<256;i++) {
                         tr00[i] = i*tr[0][0];
                         tr01[i] = i*tr[0][1];
                         tr02[i] = i*tr[0][2];
                         tr10[i] = i*tr[1][0];
                         tr11[i] = i*tr[1][1];
                         tr12[i] = i*tr[1][2];
                         tr20[i] = i*tr[2][0];
                         tr21[i] = i*tr[2][1];
                         tr22[i] = i*tr[2][2];
                         }
                         
      for(int j=0;j<kaynak.bminfo.height;j++) { for(int i=0;i<kaynak.bminfo.width;i++) {
                            
            im.pixels[i][j].red   =  gbyte_yap(tr00[kaynak.pixels[i][j].red]+tr01[kaynak.pixels[i][j].green]+tr02[kaynak.pixels[i][j].blue] + tr03);
            im.pixels[i][j].green =  gbyte_yap(tr10[kaynak.pixels[i][j].red]+tr11[kaynak.pixels[i][j].green]+tr12[kaynak.pixels[i][j].blue] + tr13);
            im.pixels[i][j].blue  =  gbyte_yap(tr20[kaynak.pixels[i][j].red]+tr21[kaynak.pixels[i][j].green]+tr22[kaynak.pixels[i][j].blue] + tr23);
            }}
      return im;
      
} 

BMP   resim_har(BMP kaynak,double tr[3][256]) {
      
      BMP im=yenim_bmp(kaynak.bminfo.width,kaynak.bminfo.height);
      int i=0;
                       
      for(int j=0;j<kaynak.bminfo.height;j++) { for(int i=0;i<kaynak.bminfo.width;i++) {
                            
            im.pixels[i][j].red   =  gbyte_yap(tr[0][kaynak.pixels[i][j].red]);
            im.pixels[i][j].green =  gbyte_yap(tr[1][kaynak.pixels[i][j].green]);
            im.pixels[i][j].blue  =  gbyte_yap(tr[2][kaynak.pixels[i][j].blue]);
            }}
      return im;
      
} 

BMP   resim_far(BMP k1,BMP k2,int absd)
{
            
      int M = k1.bminfo.height;
      int N = k1.bminfo.width;
      
      BMP im = yenim_bmp(N,M);
      int i,j;
      
      if(absd==0) {
                  
      for(i=0;i<N; i++) {
                                   
                       int kts = M*i;
                       
                       for(j=0;j<M;j++) {
                       
                                        //printf("[Ax+B]: %d-%d\n%d/%d\n",i,j,j+kts,M*N);
                                        im.pixels[i][j].red   = gbyte_yap(k1.pixels[i][j].red-k2.pixels[i][j].red);
                                        im.pixels[i][j].green = gbyte_yap(k1.pixels[i][j].green-k2.pixels[i][j].green);
                                        im.pixels[i][j].blue  = gbyte_yap(k1.pixels[i][j].blue-k2.pixels[i][j].blue);
                                        }
                                        //printf("[Ax+B]: %d-%d\n%d/%d\n",i,j,j+kts,M*N);
                       }}
       
       else {
            
      for(i=0;i<N; i++) {
                                   
                       int kts = M*i;
                       
                       for(j=0;j<M;j++) {
                       
                                        //printf("[Ax+B]: %d-%d\n%d/%d\n",i,j,j+kts,M*N);
                                        im.pixels[i][j].red   = gbyte_yap(abs(k1.pixels[i][j].red-k2.pixels[i][j].red));
                                        im.pixels[i][j].green = gbyte_yap(abs(k1.pixels[i][j].green-k2.pixels[i][j].green));
                                        im.pixels[i][j].blue  = gbyte_yap(abs(k1.pixels[i][j].blue-k2.pixels[i][j].blue));
                                        }
                                        //printf("[Ax+B]: %d-%d\n%d/%d\n",i,j,j+kts,M*N);
                       }}             
      return im;
}                

BMP resim_m2l(BMP kaynak, double *Map,int No)
{
    int M = kaynak.bminfo.height;
    int N = kaynak.bminfo.width;
    
    BMP out = yenim_bmp(N,M);
    
    
    int i,j,k,t,MapH;
    
    MapH = (int) sqrt(M*M+N*N);
    
    int count = -1;
    int AciHassasiyeti = 10;
    
    int Ainc = 1440*AciHassasiyeti/180;
    
    int SizeOfLine = (int)floor(MapH*No/9*Ainc);
    
    int *p = (int*) malloc(SizeOfLine*sizeof(int));
    int *teta = (int*) malloc(SizeOfLine*sizeof(int));
    int *val  = (int*) malloc(SizeOfLine*sizeof(int));
    
    
    for(j=0;j<No;j+=Ainc) {
                        for(i=0;i<MapH;i+=9) {
                                          count++; 
                                          int max = 0;
                                          for(t=0;t<Ainc;t++) {
                                                           for(k=0;k<9;k++) {
                                                                            if((*(Map+(t+j)*MapH+i+k)) > max) {
                                                                                                        max = (int) *(Map+(t+j)*MapH+i+k);
                                                                                                        *(p+count) = i+k;
                                                                                                        *(teta+count) = (int)180*(j+t)/No;
                                                                                                        *(val+count)  = (int)max;
                                                                                                        }
                                                                             }
                                                            }
                                               }
                        }                                                                                                  
    int threshold = 50;
    
    for(i=0;i<SizeOfLine;i++) {
                              if(*(val+i) > threshold) { printf("%d. cizgi aci: %d, p: %d\n",i,*(teta+count),*(p+count)); }
                              }
    return out;
}              
