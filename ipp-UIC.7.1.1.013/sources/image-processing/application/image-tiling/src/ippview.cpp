/*/////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2002-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#include <string>
#include <stdexcept>
#include <fstream>
using namespace std;

#include <windows.h>
#include <stdlib.h>
#include <ipp.h>

#if defined( _MSC_VER ) && defined( _DEBUG )
   #include <crtdbg.h>

   #define SET_REPORT_OUT \
      _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT ); \
      _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT )

   #define TRACE0(msg) _RPT1( _CRT_WARN, "--- %s\n", msg )

   #define REPORT   _CrtDumpMemoryLeaks()

#else

   #define SET_REPORT_OUT
   #define TRACE0(msg)
   #define REPORT
   #define _ASSERTE

#endif

typedef enum { Error = -1, Fail = 0, Good = 1 } ipStatus;

static const char* g_imgErrStr = "Problem to draw IPPI image";
static const char* g_vecErrStr = "Invalid data to draw";
static const char* g_name      = "IPPI Viewer";

static char g_strbuf[256];

/// class to simplify a work with WIN32 CriticalSection
/// Now the following declaration is needed only to
/// execute Enter and Leave procedures. Example of using
///   CritSect cs( a_section_ptr );

class CritSect {
   CRITICAL_SECTION* m_section;
public:
   CritSect( CRITICAL_SECTION* section )  : m_section(section) {
      EnterCriticalSection( m_section );
   }
   ~CritSect() { LeaveCriticalSection( m_section ); }
};

struct IppiImage {
   Ipp8u*   pData;
   int      step;
   IppiSize roi;
   int      planes;
};

static const int spWidth   = 400;
static const int spHeight  = 256;
static const int XGRD      = 4;
static const int YGRD      = 4;

struct IppsVector {
   float *pData;
   int len, xmin, xmax;
   float ymin, ymax;
};

ipStatus ipDraw( HDC hdc, IppiImage* img );
ipStatus spDraw( HDC hdc, IppsVector* vec );

/// Thread Data item

struct ThreadData {
   DWORD id;                           /// thread-owner identifier
   bool isModalView;                   /// is modal or thread viewer
   bool drawImage;
   IppiImage img;                      /// image to draw
   IppsVector vec;
   char text[128];                     /// text about IplImage image
   /// non standard copy operator
   void operator=( const ThreadData & thd ) {
      id = thd.id;
      isModalView = thd.isModalView;
      drawImage = thd.drawImage;
      img = thd.img;
      vec = thd.vec;
      lstrcpy( text, thd.text );
   }
};

/// Array of thread data

class ThreadDataArray {

   enum { NUMOF = 10 };                /// num of threads allowed

   ThreadData m_data[ NUMOF ];         /// data of threads
   CRITICAL_SECTION m_cs_data;         /// its critical section

   int m_numofThreads;                 /// current num of threads
   CRITICAL_SECTION m_cs_numof;        /// its critical section

public:
   /// ctr inits crit sects, event, id's
   ThreadDataArray();
   /// dtr dels crit sects, close event
   ~ThreadDataArray() {
      CritSect csn( &m_cs_numof );
      if( m_numofThreads <= 0 ) {
         ::DeleteCriticalSection( &m_cs_numof );
         ::DeleteCriticalSection( &m_cs_data );
         ::CloseHandle( m_hevent );
      }
   }
   /// find id thread data, id may be 0
   ThreadData* operator[] (DWORD id);
   /// find id thread, del img, dec counter, clear id
   bool close( DWORD idthread );
   /// find id=0, copy thread data, inc counter
   bool open ( const ThreadData& thd );
   /// check if no more place
   bool isfull() {
      CritSect csn( &m_cs_numof );
      return m_numofThreads >= NUMOF;
   }
   /// num of threads which are being executed
   int numof() {
      CritSect csn( &m_cs_numof );
      return m_numofThreads;
   }

   HANDLE m_hevent;                    /// event to wait for dialog inited
};

/// Create two critical sections and event
ThreadDataArray::ThreadDataArray() : m_numofThreads(0) {

   InitializeCriticalSection( &m_cs_data );
   InitializeCriticalSection( &m_cs_numof );

   /// security_attr=NULL, manual_reset=FALSE, init_state=busy=FALSE
   m_hevent = ::CreateEvent( NULL, FALSE, FALSE, "Thread Data Array Event");

   for( int i=0; i<NUMOF; ++i ) m_data[i].id = 0;
}

/// id may be 0 as well, then a free cell is found
ThreadData* ThreadDataArray::operator[] ( DWORD id ) {

   CritSect csd( &m_cs_data );
   for( int i=0; i<NUMOF; ++i )
      if( m_data[i].id == id ) return &m_data[i];
   return NULL;
}

/// find id thread, delete img, clear id, dec counter
bool ThreadDataArray::close( DWORD idthread ) {

   CritSect csd( &m_cs_data );
   for( int i=0; i<NUMOF; ++i )
      if( m_data[i].id == idthread ) {
         //iplDeallocate( m_data[i].img, IPL_IMAGE_ALL );
         if ( m_data[i].img.pData ) free( m_data[i].img.pData );
         if ( m_data[i].vec.pData ) free( m_data[i].vec.pData );
         CritSect csn( &m_cs_numof );
         m_data[i].id = 0;
         --m_numofThreads;
         return true;
      }
   return false;
}

/// find 0 id, copy thread data, inc counter
bool ThreadDataArray::open( const ThreadData& thd ) {

   CritSect csd( &m_cs_data );
   for( int i=0; i<NUMOF; ++i )
      if( 0 == m_data[i].id ) {
         CritSect csn( &m_cs_numof );
         m_data[i] = thd;
         ++m_numofThreads;
         return true;
      }
   return true;
}

/// Global thread data array
static ThreadDataArray g_threads;

/* //////////////////////////////////////////////////////////////////////
// Name:       ViewProc
// Purpose:    Dialog Procedure
// Context:    IPLib viewer
// Returns:    TRUE as a rule
// Parameters:
      HWND hdlg - handle of dialog window
      UINT message - message code
      WPARAM wParam - command code
      LPARAM - not used
//
//  Notes:

*/

static LRESULT CALLBACK ViewProc( HWND hdlg, UINT message, WPARAM wParam, LPARAM ) {

   switch( message ) {

   case WM_INITDIALOG : {

      ThreadData *threadData = g_threads[ ::GetCurrentThreadId() ];
      _ASSERTE( NULL != threadData );

      int offset = g_threads.numof() * 15;

      /// Resize and move dialog window
      RECT rect = {0,0,0,0};
      if( threadData->drawImage ) {
         rect.right = threadData->img.roi.width;
         rect.bottom = threadData->img.roi.height;
      } else {
         rect.right = spWidth;
         rect.bottom = spHeight;
      }

      /// define window size if client region size is given
      ::AdjustWindowRect(
         &rect,                              // pointer to client-rect structure
         GetWindowLong( hdlg, GWL_STYLE ),   // window styles
         FALSE                               // menu-present flag
      );
      /// window position depends on viewing mode.
      /// The dialog style has DS_CENTER bit, so the place of
      /// the dialog window is center if mode is modal
      /// It doesn't work if Borland is used
      ::SetWindowPos( hdlg, HWND_TOPMOST, offset, offset,
         rect.right - rect.left, rect.bottom - rect.top,
         SWP_NOZORDER | (threadData->isModalView ? SWP_NOMOVE : 0 ));

      /// Set window caption
      ::SetWindowText( hdlg, threadData->text );
      /// signal that dialog has been initialized
      ::SetEvent( g_threads.m_hevent );
      return TRUE;
   }

   case WM_COMMAND :
      switch( wParam ) {
      case IDOK : case IDCANCEL :
         EndDialog( hdlg, 0 );
         g_threads.close( ::GetCurrentThreadId() );
         return TRUE;
      }
      break;

   case WM_PAINT : {

      ThreadData *threadData = g_threads[ ::GetCurrentThreadId() ];
      _ASSERTE( NULL != threadData );

      PAINTSTRUCT ps;
      HDC hdc = ::BeginPaint( hdlg, &ps );
      if( threadData->drawImage )
         ipDraw( hdc, &threadData->img );
      else
         spDraw( hdc, &threadData->vec );
      ::EndPaint( hdlg, &ps );
      return TRUE;
   }

   }
   return FALSE;
}

/* //////////////////////////////////////////////////////////////////////
// Name:       ViewerThread
// Purpose:    Thread Procedure
// Context:    IPLib viewer
// Returns:    If the DialogBoxIndirectParam fails, the return value is -1
// Parameters: ThreadData* - not used
//
//  Notes:
*/

static DWORD WINAPI ViewerThread( ThreadData* ) {

   static char buf[1024] = {0};
   static DLGTEMPLATE* dlg = (DLGTEMPLATE*)buf;

   dlg->style = DS_CENTER | WS_POPUPWINDOW | WS_CAPTION | DS_MODALFRAME |  DS_SETFOREGROUND;
   dlg->dwExtendedStyle = 0;
   dlg->cdit = 0;
   dlg->x = dlg->y = 0;
   dlg->cx = 100;
   dlg->cy = 100;

   /// create a modal dialog box from a dialog box template in memory
   return (DWORD)::DialogBoxIndirectParam(
      ::GetModuleHandle(NULL),         // handle to application instance
      dlg,                            // identifies dialog box template
      NULL,                            // handle to owner window
      (DLGPROC)ViewProc,               // pointer to dialog box procedure
      (DWORD)0 );                 // initialization value
}

class bexception : public exception {
public:
   bexception( bool should_delete ) : m_should_delete( should_delete ) {}
   bool m_should_delete;
};


/* //////////////////////////////////////////////////////////////////////
// Name:       ippView
// Purpose:    to show images
// Context:    IPPI viewer
// Returns:    thread handle if there is not a modal view and all is ok
// Parameters:
      const Ipp8u* pSrc - pointer to data
      const char* caption - window caption
      bool isModalView - modal viewer if true, thread waits for a
         viewer closing. Thread doesn't wait. The following viewer
         can be started.
//
//  Notes:     Uses only one depth "8u" and one order "plane" for
               output image to draw. On the enter the following IPLib
               images can be: 8s|8u|16s|16u|32s + pixel|plane
*/

HANDLE ipView_8u_C3R(const Ipp8u* pData, int step, IppiSize roi,
                     const char* caption, int isModalView) {
   if( !pData ) return 0;

   if( g_threads.isfull() ) {
      MessageBox( NULL, "Too many threads are beign executed. "
         "Please decrease number of viewers or use Modal mode.",
         g_name, MB_OK | MB_ICONSTOP );
      return 0;
   }

   IppiImage img8u;
   HANDLE hThread = 0;

   /// Note we need not clone in the DEPTH_1U case

   try {
      /// Create header for the image to be drawn
      int abit      = (4 << 3) - 1;
      int amask     = ~abit;
      int widthstep = ((roi.width * 8 * 3 + abit) & amask)>>3;
      int imagesize = widthstep * roi.height;
      img8u.pData = (Ipp8u*)malloc(imagesize);
      img8u.step = step;
      img8u.roi = roi;
      ippiCopy_8u_C3R(pData,step,img8u.pData,widthstep,roi);

      static ThreadData param;
      lstrcpy( param.text, NULL == caption ? "IPPI image" : caption );
      param.img.pData = img8u.pData;
      param.img.step = step;
      param.img.roi = roi;
      param.img.planes = 3;
      param.isModalView = isModalView > 0;
      param.drawImage = 1;
      param.vec.pData = 0;

      /// prepare for waiting
      ::ResetEvent( g_threads.m_hevent );

      hThread = CreateThread(
         NULL,                                  // pointer to thread security attributes
         0,                                     // initial thread stack size, in bytes
         (LPTHREAD_START_ROUTINE) ViewerThread, // pointer to thread function
         (LPVOID)(0),                           // argument for new thread
         0,                                     // creation flags, runs immediately
         &param.id);                            // pointer to returned thread identifier

      if( !hThread ) throw bexception( true );

      /// set thread data for thread has been started
      if( false == g_threads.open( param )) {
         ::CloseHandle( hThread );
         throw bexception( true );
      }

      if( isModalView ) {
         /// wait for the thread will be ended
         ::WaitForSingleObject( hThread, INFINITE );
         ::CloseHandle( hThread );
         return 0;
      }
      else
         /// wait for dialog window will be initialized
         /// thread will be alive after that
         ::WaitForSingleObject ( g_threads.m_hevent, 10000 );
   }
   catch( bexception e ) {
      /// if success then img8u is deleted by the close procedure
      /// when dialog window will be closed by user
      /// else we should delete img8u
      free( img8u.pData );
   }
   ///

   return hThread;
}

HANDLE ipView_8u_C1R(const Ipp8u* pData, int step, IppiSize roi,
                     const char* caption, int isModalView) {
   if( !pData ) return 0;

   if( g_threads.isfull() ) {
      MessageBox( NULL, "Too many threads are beign executed. "
         "Please decrease number of viewers or use Modal mode.",
         g_name, MB_OK | MB_ICONSTOP );
      return 0;
   }

   IppiImage img8u;
   HANDLE hThread = 0;

   /// Note we need not clone in the DEPTH_1U case

   try {
      /// Create header for the image to be drawn
      int abit      = (4 << 3) - 1;
      int amask     = ~abit;
      int widthstep = ((roi.width * 8 + abit) & amask)>>3;
      int imagesize = widthstep * roi.height;
      img8u.pData = (Ipp8u*)malloc(imagesize);
      img8u.step = step;
      img8u.roi = roi;
      ippiCopy_8u_C1R(pData,step,img8u.pData,widthstep,roi);

      static ThreadData param;
      lstrcpy( param.text, NULL == caption ? "IPPI image" : caption );
      param.img.pData = img8u.pData;
      param.img.step = step;
      param.img.roi = roi;
      param.img.planes = 1;
      param.isModalView = isModalView > 0;
      param.drawImage = 1;
      param.vec.pData = 0;

      /// prepare for waiting
      ::ResetEvent( g_threads.m_hevent );

      hThread = CreateThread(
         NULL,                                  // pointer to thread security attributes
         0,                                     // initial thread stack size, in bytes
         (LPTHREAD_START_ROUTINE) ViewerThread, // pointer to thread function
         (LPVOID)(0),                           // argument for new thread
         0,                                     // creation flags, runs immediately
         &param.id);                            // pointer to returned thread identifier

      if( !hThread ) throw bexception( true );

      /// set thread data for thread has been started
      if( false == g_threads.open( param )) {
         ::CloseHandle( hThread );
         throw bexception( true );
      }

      if( isModalView ) {
         /// wait for the thread will be ended
         ::WaitForSingleObject( hThread, INFINITE );
         ::CloseHandle( hThread );
         return 0;
      }
      else
         /// wait for dialog window will be initialized
         /// thread will be alive after that
         ::WaitForSingleObject ( g_threads.m_hevent, 10000 );
   }
   catch( bexception e ) {
      /// if success then img8u is deleted by the close procedure
      /// when dialog window will be closed by user
      /// else we should delete img8u
      free( img8u.pData );
   }
   ///

   return hThread;
}


/* //////////////////////////////////////////////////////////////////////
// Name:       ipDraw
// Purpose:    draw 8u pixel IPLib images
// Context:    IPLib viewer
// Returns:    Error if NULL, Fail if wrong parameters
// Parameters:
      HDC hdc - defines device context
      IplImage* - IPLib image to draw
//
//  Notes:
      supports
         1 channel, IPL_DEPTH_8U  with color mode "GRAY"
         3,4 channels, IPL_DEPTH_8U with color mode "RGB" or "RGBA"
*/

ipStatus ipDraw( HDC hdc, IppiImage* img ) {

   if( !img || !img->pData ) return Error;

   bool isrgb = (1 != img->planes);

   bool isgray = (1 == img->planes);

   if( !isgray && !isrgb ) return Fail;

   char buf[ sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256 ];
   BITMAPINFOHEADER* dibhdr = (BITMAPINFOHEADER*)buf;
   COLORREF* rgb = (COLORREF*)( buf + sizeof(BITMAPINFOHEADER) );

   if( isgray ) for( int i = 0; i < 256; i++) rgb[i] = RGB( i,i,i );

   dibhdr->biSize = sizeof( BITMAPINFOHEADER );
   dibhdr->biWidth = img->roi.width;
   dibhdr->biHeight = img->roi.height;
   dibhdr->biPlanes = 1;
   dibhdr->biBitCount = (WORD)(8 * img->planes/*1 3*/);
   dibhdr->biCompression = BI_RGB;
   dibhdr->biSizeImage = img->roi.width*img->roi.height;
   dibhdr->biXPelsPerMeter = 0;
   dibhdr->biYPelsPerMeter = 0;
   dibhdr->biClrUsed = 0;
   dibhdr->biClrImportant = 0;

   ::SetDIBitsToDevice( hdc, 0,0,img->roi.width,img->roi.height,
      0,0,0,img->roi.height, img->pData, (BITMAPINFO*)dibhdr, DIB_RGB_COLORS );

   return Good;
}

/* //////////////////////////////////////////////////////////////////////
// Name:       ipLoad
// Purpose:    load bmp-file
// Context:    IPLib tutorial
// Returns:    bmp
// Parameters: fname - bmp-file name to read
//
//  Notes:
*/

BITMAPINFOHEADER* ipLoad( const char* fname ) {

   if( !fname ) return NULL;

   BITMAPINFOHEADER* infohdr = NULL;

   ifstream fsrc;

   try {
      BITMAPFILEHEADER filehdr;
      BITMAPINFOHEADER bmphdr;

      fsrc.open( fname, ios::in | ios::binary );
      if( fsrc.fail() ) throw runtime_error("Problem to open source file");


      fsrc.read( (char*)&filehdr, sizeof(BITMAPFILEHEADER) );
      if( fsrc.fail() ) throw runtime_error("Problem to read bmp-file header from source file");

      if( 0x4d42 != filehdr.bfType ) throw runtime_error("Wrong type of source file");

      fsrc.read( (char*)&bmphdr, sizeof(BITMAPINFOHEADER) );
      if( fsrc.fail() ) throw runtime_error("Problem to read bmp-header from source file");

      if( 0 == bmphdr.biSizeImage )
         bmphdr.biSizeImage =
            (((( bmphdr.biWidth * bmphdr.biBitCount ) + 31) & ~31) >> 3) * bmphdr.biHeight;

      if( 0 == bmphdr.biClrUsed ) {
         if( BI_BITFIELDS == bmphdr.biCompression )
            bmphdr.biClrUsed = 3;
         else
            bmphdr.biClrUsed = bmphdr.biBitCount < 16 ? 1 << bmphdr.biBitCount : 0;
      }

      int colorbytes = sizeof(RGBQUAD) * bmphdr.biClrUsed;
      int totalbytes = sizeof(BITMAPINFOHEADER) + colorbytes + bmphdr.biSizeImage;

      infohdr = (BITMAPINFOHEADER*)malloc( totalbytes );
      if( !infohdr ) throw runtime_error("Problem to allocate memory for DIB data");

      char* quads = (char*)infohdr + sizeof(BITMAPINFOHEADER);
      char* pixels = (char*)quads + colorbytes;

      memcpy( infohdr, &bmphdr, sizeof( bmphdr) );

      fsrc.read( (char*)quads, colorbytes );
      fsrc.read( (char*)pixels, bmphdr.biSizeImage );

      fsrc.close();
   }
   catch( runtime_error e ) {
      TRACE0( e.what() );
      if( infohdr ) {
         free( infohdr );
         infohdr = 0;
      }
   }

   return infohdr;
}

HANDLE spViewMinMax_32f( const float* signal, int len, const char* caption, int isModal,
   float ymn, float ymx );

HANDLE spView_32f( const float* signal, int len, const char* caption, int isModal ) {
   return spViewMinMax_32f( signal, len, caption, isModal, 0, 0 );
}

HANDLE spViewMinMax_32f( const float* signal, int len, const char* caption, int isModal,
   float ymn, float ymx )
{
   if( !signal || len < 4 ) return 0;

   if( g_threads.isfull() ) {
      MessageBox( NULL, "Too many threads are beign executed. "
         "Please decrease number of viewers or use Modal mode.",
         g_name, MB_OK | MB_ICONSTOP );
      return 0;
   }

   HANDLE hThread = 0;
   static ThreadData param;

   /// Note we need not clone in the DEPTH_1U case

   try {
      float ymin, ymax;
      if( ymx > ymn ) {
         ymin = ymn;
         ymax = ymx;
      } else {
         ymin = ymax = signal[0];
         for( int n=1; n<len; n++) {
            float ytmp = signal[n];
            if(ytmp > ymax) ymax = ytmp;
            if(ytmp < ymin) ymin = ytmp;
         }
      }
      if( ymax <= ymin) throw bexception( true );
      param.vec.ymin = ymin; param.vec.ymax = ymax;
      param.vec.xmin = 0;  param.vec.xmax = len;
      param.vec.pData = (float*)malloc( len * sizeof(float) );
      memcpy( param.vec.pData, signal, len * sizeof(float) );

      lstrcpy( param.text, NULL == caption ? "IPPS signal" : caption );
      param.isModalView = isModal > 0;
      param.vec.len = len;
      param.drawImage = 0;
      param.img.pData = 0;


      /// prepare for waiting
      ::ResetEvent( g_threads.m_hevent );

      hThread = CreateThread(
         NULL,                                  // pointer to thread security attributes
         0,                                     // initial thread stack size, in bytes
         (LPTHREAD_START_ROUTINE) ViewerThread, // pointer to thread function
         (LPVOID)(0),                           // argument for new thread
         0,                                     // creation flags, runs immediately
         &param.id);                            // pointer to returned thread identifier

      if( !hThread ) throw bexception( true );

      /// set thread data for thread has been started
      if( false == g_threads.open( param )) {
         ::CloseHandle( hThread );
         throw bexception( true );
      }

      if( isModal ) {
         /// wait for the thread will be ended
         ::WaitForSingleObject( hThread, INFINITE );
         ::CloseHandle( hThread );
         return 0;
      }
      else
         /// wait for dialog window will be initialized
         /// thread will be alive after that
         ::WaitForSingleObject ( g_threads.m_hevent, 10000 );
   }
   catch( bexception e ) {
      /// if success then img8u is deleted by the close procedure
      /// when dialog window will be closed by user
      /// else we should delete img8u
      free( param.vec.pData );
   }
   ///

   return hThread;
}

static void drawAsLine( float* y, int len, HDC hdc, float ax, float bx,
   float ay, float by, int color )
{
   HGDIOBJ pen = ::SelectObject( hdc, CreatePen( PS_SOLID, 1, color ) );
   ::MoveToEx( hdc, (int)(ax+bx*0), (int)(ay-by*y[0]), 0 );
   for( int n=1; n<len; n++) ::LineTo( hdc, (int)(ax+bx*n), (int)(ay-by*y[n]) );
   ::SelectObject( hdc, pen );
}

ipStatus spDraw( HDC hdc, IppsVector* vec ) {

   if( !vec || !vec->pData ) return Error;

   RECT r = { 0, 0, spWidth, spHeight };
   int i;

   ::PatBlt( hdc, 0, 0, spWidth, spHeight, BLACKNESS );
   ::InflateRect( &r, -1, -1 );
   int dx = r.right - r.left;
   int dy = r.bottom - r.top;
   int ddx = dx / XGRD;
   int ddy = dy / YGRD;

   if( ddx >0 && ddy >0 ) {
      HGDIOBJ  brush = ::SelectObject( hdc, CreateSolidBrush( 0x007F7F7F ) );
      for( i=1; i<YGRD; i++) ::PatBlt( hdc, r.left+ddx*i, r.top, 1, dy, PATCOPY );
      for( i=1; i<XGRD; i++) ::PatBlt( hdc, r.left, r.top+ddy*i, dx, 1, PATCOPY );
      ::SelectObject( hdc, brush );

      // Curves
      float bx = (float)dx / (vec->xmax - vec->xmin);
      float ax = r.left - bx*vec->xmin;
      float by = 0.0;
      if((vec->ymax - vec->ymin) >0.0) by = dy/(vec->ymax - vec->ymin);
      float ay = -by*vec->ymax;

      drawAsLine( vec->pData, vec->len, hdc, ax, bx, -ay, by, 0x0000FF00 );

      COLORREF oldBk = ::SetBkColor( hdc, 0x00000000 );
      int oldMode = ::SetBkMode( hdc, OPAQUE );
      COLORREF oldTextColor = ::SetTextColor( hdc, 0x007F7F7F );

      sprintf( g_strbuf, "%0.2f", vec->ymax );
      ::SetTextAlign( hdc, TA_RIGHT|TA_TOP );
      ::TextOut( hdc, r.right, r.top+2, g_strbuf, (int)(strlen( g_strbuf )) );

      sprintf( g_strbuf, "%0.2f", vec->ymin );
      ::SetTextAlign( hdc, TA_RIGHT|TA_BOTTOM );
      ::TextOut( hdc, r.right, r.bottom-2, g_strbuf, (int)(strlen( g_strbuf )) );

      ::SetTextColor( hdc, oldTextColor );
      ::SetBkMode( hdc, oldMode );
      ::SetBkColor( hdc, oldBk );

      // axis mark
      return Good;
   }
   ::TextOut( hdc, r.left+15, r.top+15, g_vecErrStr, (int)(strlen( g_vecErrStr )) );
   return Fail;
}
