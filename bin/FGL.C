#include<malloc.h>

#ifndef FGL_LOADED

#include "FGL.h"
/*
   This is the open-source library for DOS (16 bit) Graphics Programming .
   This library cannot be used in windows etc... . The code is open to be edited
   any illegal changes and any violant act would violate the laws of the source code.
   You can use the code in any program you want and even can change the source code
   according to your need.
*/

int GetModeGL( )
{
   char cur_m = 0;

   asm mov ah , 0x0f ;
   asm int 0x10 ;
   asm mov [cur_m] , al ;

   return cur_m;
}

void * allocGL(int size)
{
  return malloc(size);
}

int GetScreenGL( )
{
    char cur_vp = 0;

    asm mov ah , 0x0f ;
    asm int 0x10 ;
    asm mov [cur_vp] , bh   ;

    return cur_vp;
}

void mouseEnableGL()
{
   asm mov ax , 1 ;
   asm int 0x33 ;
}

void mouseDisableGL( )
{
  asm mov ax , 2 ;
  asm int 0x33 ;
}

struct Vertex mouseGetPosition( )
{
  int x = 0;
  int y = 0;
  struct Vertex v;
  
  asm mov ax , 3 ;
  asm int 0x33 ;
  asm mov [x] , cx ;
  asm mov [y] , dx ;

  v.__Px = x;
  v.__Py = y;

  return v;
}

struct MouseState getMouseState( )
{
    struct MouseState s;
	int _s = 0;

	asm mov ax , 3 ;
	asm int 0x33 ;
	asm mov [_s] , bx ;

	if (_s == 0)
	{
		s.__lb = 1;
		s.__rb = 0;
	}
	else
	{
		s.__lb = 0;
		s.__rb = 1;
	}

	return s;
}

void xInitializeGL(struct Device *dev)
{
   dev->__mode = 0x3;
   dev->__vdu_txt_ar = (char far *) 0xb0008000;
}

void InitializeGL(struct Device *dev)
{
    asm mov ah , 0 ;
    asm mov al , 0x12 ;
    asm int 0x10 ;

    dev->__mode = 0x12;
    dev->__adapterSize = 0;
}

int WriteCharGL(struct Device *dev,char ch,int _ch_col)
{
    if (dev->__mode == 0x0 || dev->__mode == 0x1 || dev->__mode == 0x2 || dev->__mode == 0x3 || dev->__mode == 0x7)
    {
	 *(dev->__vdu_txt_ar) = ch; // Write the current character .
	 dev->__vdu_txt_ar++;            // Advance to the next block.
	 *(dev->__vdu_txt_ar) = _ch_col;  // Write the colour of the character

	 dev->__vdu_txt_ar++;       // Advance to the next block.

     return 1;
   }
   else
   {
     return NULL; // Error
   }
}

int WriteStringGL(struct Device *dev,const char *__str,int _ch_col)
{
   int cnt = 0;
   
   if (dev->__mode == 0x0 || dev->__mode == 0x1 || dev->__mode == 0x2 || dev->__mode == 0x3 || dev->__mode == 0x7)
   {
     for(;;)
     {
	if (*(__str + cnt) == NULL)
	{
	 break;
	}
	else
	{
	 WriteCharGL(dev,*(__str + cnt),_ch_col);
	}

	  cnt++;
     }

     return 1;
   }
   else
   {
     return NULL; // Error
   }
}

struct Data * resizeAdapterGL(struct Device *dev,int newsize)
{
   struct Data *_dat = (struct Data *) allocGL(sizeof(struct Data) * newsize);
   int cnt = 0;
 
   for(;cnt < dev->__adapterSize;cnt++)
   {
    _dat[cnt].__P.__Px = dev->__data_ar[cnt].__P.__Px;
	_dat[cnt].__P.__Py = dev->__data_ar[cnt].__P.__Py;
	_dat[cnt]._R = dev->__data_ar[cnt]._R;
	_dat[cnt]._G = dev->__data_ar[cnt]._G;
	_dat[cnt]._B = dev->__data_ar[cnt]._B;
	_dat[cnt]._I = dev->__data_ar[cnt]._I;
   }

   /* for(cnt = dev->__adapterSize;cnt < newsize;cnt++)
   {
     struct Data dat;

     dat.__P.__Px = 0;
     dat.__P.__Py = 0;

     dat._R = 0;
     dat._G = 0;
     dat._B = 0;
     dat._I = 0;

     _dat[cnt] = dat;
   } The existance of this block of code is same as if it dont exist. */
   
   return _dat;
}

int PlotPixelGL(struct Device *dev,struct Data pix_dat)
{
  if (dev->__mode == 0x12)
  {
	dev->__data_ar = resizeAdapterGL(dev,dev->__adapterSize + 1);
	dev->__data_ar[dev->__adapterSize] = pix_dat;
	dev->__adapterSize++;
	return 1;
  }
  else
  {
     return NULL;
  }
}

struct Pixel ReadPixelGL(struct Device *dev,int pos_x,int pos_y)
{
   struct Pixel px;

   if (dev->__mode == 0x12)
   {
   char _col;
   char sc = GetScreenGL( );

   asm mov ah , 0x0d ;
   asm mov bh , [sc] ;
   asm mov cx , [pos_x] ;
   asm mov dx , [pos_y] ;
   asm int 0x10 ;
   asm mov [_col] , al ;

   px.pos.__Px = pos_x;
   px.pos.__Py = pos_y;
   px._col = _col;
   }

   return px;
}

int LoadLineGL(struct Device *dev,int start_x,int start_y,int end_x,int end_y,struct Data pix_dat)
{
  if (dev->__mode == 0x12)
  {

  int cnt_x = start_x,cnt_y = start_y;

  while((cnt_x < end_x || cnt_x > end_x) || (cnt_y < end_y || cnt_y > end_y))
  {
    struct Data dat;

    if (cnt_x < end_x)
    {
       cnt_x++;
    }
    else if (cnt_x > end_x)
    {
       cnt_x--;
    }

    if (cnt_y < end_y)
    {
      cnt_y++;
    }
    else if (cnt_y > end_y)
    {
      cnt_y--;
    }

    dat.__P.__Px = cnt_x;
    dat.__P.__Py = cnt_y;
    dat._R = pix_dat._R;
    dat._G = pix_dat._G;
    dat._B = pix_dat._B;
    dat._I = pix_dat._I;

    PlotPixelGL(dev,dat);
  }
   return 1;
  }
  else
  {
    return NULL;
  }
}

int ImageLoadPixelGL(struct Device *dev,struct Image *img,struct Data pix_dat)
{
   if (dev->__mode == 0x12)
   {
    const struct Data *dat_ar = img->__data_ar;
    int cnt = 0;

    img->__data_ar = NULL;
    img->__data_ar = (struct Data *) allocGL(sizeof(struct Data) * (img->__size + 1));

    for(;cnt < img->__size;cnt++)
    {
       img->__data_ar[cnt] = dat_ar[cnt];
    }

	img->__data_ar[img->__size] = pix_dat;
	img->__size++;
	
     return 1;
   }
   else
   {
     return NULL;
   }
} 

int ImageLoadGL(struct Device *dev,struct Image img)
{
   if (dev->__mode == 0x12)
   {
      int cnt = 0;

      for(;cnt < img.__size;cnt++)
      {
		dev->__data_ar = resizeAdapterGL(dev,dev->__adapterSize + 1);
		dev->__data_ar[dev->__adapterSize] = img.__data_ar[cnt];
		dev->__adapterSize++;
      }

      return 1;
   }
   else
   {
     return NULL;
   }
} 


int ClearViewportGL(struct Device *dev,int x,int y,int width,int height)
{

  if (dev->__mode == 0x12)
  {
    char cur_page = GetScreenGL( );
    int _x = x;
    int _y = y;

    for(;_y <= height;_y++)
    {
      for(;_x <= width;_x++)
      {
	asm mov ah , 0x0c ;
	asm mov bh , [cur_page] ;
	asm mov al , 0 ;
	asm mov cx , [_x] ;
	asm mov dx , [_y] ;
	asm int 0x10 ;
      }
    }

    return 1;
  }
  else
  {
    return NULL;
  }
}

int RenderGL(struct Device *dev)
{
  if (dev->__mode == 0x12)
  {

  int cntr = 0;
  char cur_sc = GetScreenGL();

  for(;cntr < dev->__adapterSize;cntr++)
  {
     int pX = dev->__data_ar[cntr].__P.__Px;
     int pY = dev->__data_ar[cntr].__P.__Py;
     char _col = MAKE_COL(dev->__data_ar[cntr]._R,dev->__data_ar[cntr]._G,dev->__data_ar[cntr]._B,dev->__data_ar[cntr]._I);

     asm mov ah , 0x0c ;
     asm mov bh , [cur_sc] ;
     asm mov al , [_col] ;
     asm mov cx , [pX] ;
     asm mov dx , [pY] ;
     asm int 0x10 ;
  }

    dev->__data_ar = NULL;

    dev->__adapterSize = 0;

    return 1;
  }
  else
  {
    return NULL;
  }
}

#endif
