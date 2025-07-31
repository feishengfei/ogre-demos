#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreLogManager.h>
#include <OgreWindowEventUtilities.h>
#include <iostream>

using namespace Ogre;

int main() {
    // Create LogManager and default log
    LogManager* logMgr = new LogManager();
    logMgr->createLog("ogre_renderwindow.log", true, true, false);

    // Create the Root object with empty plugin/config paths
    Root* root = new Root("/usr/bin/plugins.cfg", "", "ogre_renderwindow.log");

    //  Load the Vulkan RenderSystem plugin (adjust if using other backend)
    const RenderSystemList& renderers = root->getAvailableRenderers();
    if (renderers.empty()) {
        std::cerr << "No RenderSystems found. Make sure plugins.cfg is configured properly." << std::endl;
        delete root;
        delete logMgr;
        return 1;
    }

    // Select the first available RenderSystem
    root->setRenderSystem(renderers[0]);

    // Initialize the Root without creating a window
    root->initialise(false);

    // Define render window parameters
    NameValuePairList params;
    params["FSAA"] = "4";                           // Enable anti-aliasing
    params["vsync"] = "true";                       // Enable vertical sync
    params["title"] = "OGRE RenderWindow Example";

    // Create the render window manually
    RenderWindow* window = root->createRenderWindow("MyRenderWindow", 800, 600, false, &params);

    // Basic message to log
    LogManager::getSingleton().logMessage("Render window created.");

    // Main loop (runs until the window is closed) FIXME, window will ignore [x] close action
    while (!window->isClosed()) {
        // Process window events
        WindowEventUtilities::messagePump();

        // Render one frame
        root->renderOneFrame();
    }

    // cleanup
    delete root;
    delete logMgr;

    return 0;
}

