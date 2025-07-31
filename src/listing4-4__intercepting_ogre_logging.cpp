#include <OgreRoot.h>
#include <OgreLogManager.h>
#include <OgreLog.h>
#include <OgreString.h>
#include <iostream>

using namespace Ogre;

class MyLogListener : public LogListener
{
public:
    /*
    void write(const String& name, const String& message,
            LogMessageLevel level, bool maskDebug) override
    */
    void messageLogged( const String& message, LogMessageLevel lml,
            bool maskDebug, const String &logName, bool& skipThisMessage ) override
    {
        std::cout << "[MyLogListener] (" << logName << "):" << message << std::endl;
    };
};

int main() {
    LogManager* logMgr = new LogManager();

    
    Log* log = LogManager::getSingleton().createLog("mylog.log", true, false, true);
    log->setLogDetail(LL_NORMAL);

    MyLogListener *myListener = new MyLogListener();
    log->addListener(myListener);

    log->logMessage("Log system initialized.");

    Root* root = new Root("", "", "mylog.log");
    log->logMessage("Ogre Root created.");

    delete root;
    log->logMessage("Ogre Root deleted.");
    delete logMgr;
    delete myListener;
    return 0;
}
