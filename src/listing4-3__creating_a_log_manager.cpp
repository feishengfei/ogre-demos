#include <OgreRoot.h>
#include <OgreLogManager.h>

int main() {
    Ogre::LogManager* logMgr = new Ogre::LogManager();
    Ogre::Log* log = Ogre::LogManager::getSingleton().createLog("mylog.log", true, true, false);
    log->logMessage("Log system initialized.");

    Ogre::Root* root = new Ogre::Root("", "");
    log->logMessage("Ogre Root created.");

    delete root;
    log->logMessage("Ogre Root deleted.");
    delete logMgr;
    return 0;
}
