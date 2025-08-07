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
        RenderWindow* window = getRenderWindow();

        // Creata scene manager 
        SceneManager* sceneMgr = root->createSceneManager();
        RTShader::ShaderGenerator::getSingleton().addSceneManager(sceneMgr);

        // Create camera
        Camera* cam = sceneMgr->createCamera("MainCam");
        cam->setNearClipDistance(1);
        cam->setAutoAspectRatio(true);

        // Create viewport and attach camera
        Viewport* vp = window->addViewport(cam);
        vp->setBackgroundColour(ColourValue(1.0, 1.0, 1.0));

        SceneNode * camNode = sceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0, 0, 140));
        camNode->attachObject(cam);      
        camNode->lookAt(Vector3(0, 0, 0), Node::TS_WORLD);

        // load entity
        Entity* ogreEntity = sceneMgr->createEntity("ogrehead.mesh");
        SceneNode* ogreNode = sceneMgr->getRootSceneNode()->createChildSceneNode("head");
        ogreNode->attachObject(ogreEntity);
        ogreNode->setScale(1.0f, 1.0f, 1.0f);  // 可调节大小

        // 打光
        sceneMgr->setAmbientLight(ColourValue(0.3f, 0.3f, 0.3f));
        Light* light = sceneMgr->createLight("MainLight");
        SceneNode* lightNode = sceneMgr->getRootSceneNode()->createChildSceneNode("light");
        lightNode->setPosition(20, 40, 20);
        lightNode->attachObject(light);
    
        // ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
        // cam->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

        // Optional: create log message
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
