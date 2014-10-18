#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"

USING_NS_CC;

int APIENTRY _tWinMain (HINSTANCE hInstance,
                        HINSTANCE hPrevInstance,
                        LPTSTR    lpCmdLine,
                        int       nCmdShow)
{
    UNREFERENCED_PARAMETER (hPrevInstance);
    UNREFERENCED_PARAMETER (lpCmdLine);

    AllocConsole();
    freopen ("CONIN$", "r", stdin);
    freopen ("CONOUT$", "w", stdout);
    freopen ("CONOUT$", "w", stderr);

    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setViewName ("cigar");
    eglView->setFrameSize (480 , 320 );
    return CCApplication::sharedApplication()->run();
}
