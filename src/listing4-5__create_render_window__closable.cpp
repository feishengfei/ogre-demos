#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgreInput.h>
#include <OgreCameraMan.h>
#include <OgreTrays.h>

using namespace Ogre;
using namespace OgreBites;

class RenderWindowApp : public ApplicationContext, public InputListener {
public:
    RenderWindowApp() : ApplicationContext("RenderWindowApp") {}

    void setup() override {
        // Base setup
        ApplicationContext::setup();
        addInputListener(this);

        // create custom log file
        LogManager* logMgr = LogManager::getSingletonPtr();
        // debugOutput=true, suppressFileOutput=false
        Log* myLog = logMgr->createLog("myapp.log", true, true, false);  
        myLog->setDebugOutputEnabled(true);
        myLog->logMessage("*** Custom log created and ready.");

        // Create root and scene manager
        Root* root = getRoot();
        SceneManager* sceneMgr = root->createSceneManager();

        // Create camera
        Camera* cam = sceneMgr->createCamera("MainCam");
        cam->setNearClipDistance(5);
        cam->setAutoAspectRatio(true);

        // Create viewport and attach camera
        RenderWindow* window = getRenderWindow();
        Viewport* vp = window->addViewport(cam);
        vp->setBackgroundColour(ColourValue(0, 0, 0));
        cam->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

        // Optional: create log message
        LogManager::getSingleton().logMessage("Render window created using ApplicationContext.");
    }

    // Handle key events (optional)
    bool keyPressed(const KeyboardEvent& evt) override {
        if (evt.keysym.sym == SDLK_ESCAPE) {
            LogManager::getSingleton().logMessage("ESC pressed. Exiting...");
            getRoot()->queueEndRendering();
        }
        return true;
    }
};

int main() {
    RenderWindowApp app;
    app.initApp();              // Initializes Ogre, window, resources, etc.
    app.getRoot()->startRendering(); // Starts render loop
    app.closeApp();             // Cleans up
    return 0;
}
