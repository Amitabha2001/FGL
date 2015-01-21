#define FGL_LOADED
#define FGL_GRAPHICS 0x12
#define NULL 0
#define MAKE_COL(R,G,B,I) ((R * 8) + (G * 4) + (B * 2) + I)
#define VDU_TEXT 0xb0008000

/*
   This is the open-source library for DOS (16 bit) Graphics Programming .
   This library cannot be used in windows etc... . The code is open to be edited
   any illegal changes and any violant act would violate the laws of the source code.
   You can use the code in any program you want and even can change the source code
   according to your need.
*/

struct Vertex
{
   int __Px;
   int __Py;
};

struct Data
{
   struct Vertex __P;
   int _R;
   int _G;
   int _B;
   int _I;
};

struct xVertex  // Special Vertex
{
   struct Vertex __nv;
   struct Vertex _link;
   struct Data px__dat;
};

struct Device
{
   int __mode;
   int __adapterSize;
   struct Data * __data_ar;
};

struct Image
{
  int __size;
  struct Data * __data_ar;
};

struct MouseState
{
   int __lb;
   int __rb;
};

struct Pixel
{
   struct Vertex pos;
   int _col;
};

// SOME LOW-LEVEL NAMES ;
typedef struct Data *
                  __PIX_AR;
typedef struct xVertex *
                  __xVERT_AR;
typedef struct Vertex *
		  __VERT_AR;

int GetModeGL( );
void * allocGL(int size);
int GetScreenGL( );
void mouseEnableGL( );
void mouseDisableGL( );
struct Vertex mouseGetPosition( );
struct MouseState getMouseState( );
void xInitializeGL(struct Device *dev );
void InitializeGL(struct Device *dev);
int WriteCharGL(struct Device *dev,char ch , int _ch_col);
int WriteStringGL(struct Device *dev,const char *__str,int _ch_col);
struct Data * resizeAdapterGL(struct Device *dev,int newsize);
int PlotPixelGL(struct Device *dev,struct Data pix_dat);
struct Pixel ReadPixelGL(struct Device *dev,int pos_x,int pos_y);
int LoadLineGL(struct Device *dev,int start_x,int start_y,int end_x,int end_y,struct Data pix_dat);
int ImageLoadPixelGL(struct Device *dev,struct Image *img,struct  Data pix_dat);
int ImageLoadGL(struct Device *dev,struct Image img);
int ClearViewportGL(struct Device *dev,int x,int y,int width,int height);
int RenderGL(struct Device *dev);
