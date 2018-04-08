#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main()
{
#ifdef _PROGRAMMABLE_RENDERER
    ofGLFWWindowSettings settings;
    settings.setGLVersion(4, 1);
    settings.setSize(1024, 768);
    ofCreateWindow(settings);
#else
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1024,768, OF_WINDOW);
#endif
    ofRunApp(new ofApp());
}
