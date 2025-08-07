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
        ogreNode = sceneMgr->getRootSceneNode()->createChildSceneNode("head");
        ogreNode->attachObject(ogreEntity);
        ogreNode->setScale(1.0f, 1.0f, 1.0f);  // 可调节大小

        // light
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

    // 鼠标按下
    bool mousePressed(const MouseButtonEvent& evt) override {
        if (evt.button == BUTTON_LEFT) {
            rotating = true;
            lastMouseX = evt.x;
            lastMouseY = evt.y;
        }
        return true;
    }

    // 鼠标释放
    bool mouseReleased(const MouseButtonEvent& evt) override {
        if (evt.button == BUTTON_LEFT) {
            rotating = false;
        }
        return true;
    }

    // 鼠标移动
    bool mouseMoved(const MouseMotionEvent& evt) override {
        if (rotating && ogreNode) {
            int dx = evt.x - lastMouseX;
            int dy = evt.y - lastMouseY;

            Radian angleX = Degree(-0.2f * dx); // 鼠标 X → Yaw
            Radian angleY = Degree(-0.2f * dy); // 鼠标 Y → Pitch

            ogreNode->yaw(angleX, Node::TS_LOCAL);
            ogreNode->pitch(angleY, Node::TS_LOCAL);

            lastMouseX = evt.x;
            lastMouseY = evt.y;
        }
        return true;
    }

    bool mouseWheelRolled(const MouseWheelEvent& evt) override {
        if (!ogreNode) return true;

        // 每次滚动单位的缩放比例（根据实际设备可能需要调整）
        float scaleStep = 0.1f; // 缩放步长

        // 正值表示向上滚（放大），负值表示向下滚（缩小）
        float direction = evt.y;

        Vector3 currentScale = ogreNode->getScale();
        float newScaleFactor = 1.0f + scaleStep * direction;

        // 限制最小/最大缩放
        float minScale = 0.05f;
        float maxScale = 10.0f;

        Vector3 newScale = currentScale * newScaleFactor;

        if (newScale.x >= minScale && newScale.x <= maxScale) {
            ogreNode->setScale(newScale);
        }

        return true;
    }

private:
    SceneNode* ogreNode = nullptr;
    bool rotating = false;
    int lastMouseX = 0;
    int lastMouseY = 0;
};

int main() {
    RenderWindowApp app;
    app.initApp();              // Initializes Ogre, window, resources, etc.
    app.getRoot()->startRendering(); // Starts render loop
    app.closeApp();             // Cleans up
    return 0;
}
