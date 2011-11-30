#include <android-receiver.h>
#include <android/log.h>
#include "event.h"
#include <irrlicht.h>
#include "rabbit.h"
#include <stdlib.h>
#include "platforms.h"
#include "text.h"
#include "carrots.h"
#include "stringaling.h"
#include "misc.h"
#include "launcher/DeviceSettings.h"


extern IrrlichtDevice *device;
extern scene::ISceneManager *smgr;
extern video::IVideoDriver *driver;
extern gui::IGUIEnvironment *guienv;
extern core::vector3df firstCameraPosition;
extern core::vector3df firstCameraTarget;
extern int carrotsCollected;

int state = 0;
/*
Text *startBlinkText;

gui::IGUIStaticText *tEndCongrats;
gui::IGUIStaticText *tEndScore;
gui::IGUIStaticText *tEndCarrots;
Text *textNewHighScore;
gui::IGUIStaticText *tLastHighScore;
*/
extern int gAppAlive;

//#ifdef IS_DEMO
enum {B_BUY, B_NOBUY};

extern bool OPEN_WEBPAGE_ON_EXIT;
//#endif 

bool AndroidEventReceiver::OnEvent(const SEvent& e)
{
//    __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "AndroidEventReceiver OnEvent()");

#ifdef IS_DEMO
        if (e.EventType == EET_GUI_EVENT)
        {   
        __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "AndroidEventReceiver OnEvent()1");
            s32 id = e.GUIEvent.Caller->getID();
            
            switch(e.GUIEvent.EventType)
            {
                case gui::EGET_BUTTON_CLICKED:
                    if (id == B_BUY)
                    {
                        device->closeDevice();
                        OPEN_WEBPAGE_ON_EXIT = true;
                        return true;
                    }
                    if (id == B_NOBUY)
                    {
                        device->closeDevice();
                        return true;
                    }
                    break;
            }
        }
#endif
        if (e.KeyInput.Key == KEY_ESCAPE && e.KeyInput.PressedDown)
        {
#ifdef IS_DEMO
            __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "AndroidEventReceiver OnEvent()2");
            if (state != IN_DEMOSCREEN)
            {
                if (state == IN_ENDSCREEN)
                {
                    tEndCongrats->remove();
                    tEndScore->remove();
                    tEndCarrots->remove();
                    if (textNewHighScore) removeText(textNewHighScore);
                    tLastHighScore->remove();
                }
                removeAllTexts();
                
                state = IN_DEMOSCREEN;
                
                removePlatforms();
                removeRabbit();
                removeCarrotDisplay();
                
                smgr->getActiveCamera()->setInputReceiverEnabled(false);
                device->getCursorControl()->setVisible(true);
                
                int bWidth1 = 210;
                int bWidth2 = 260;
                int bHeight = 50;
                int bGap = 10;
                int bLow = 20;
                int centreX = screenHW()-25;
                
                video::ITexture *tex = driver->getTexture("sdcard/Irrlicht/media/demo.png");
                core::dimension2d<s32> dim = tex->getSize();
                gui::IGUIImage *img = guienv->addImage(
                    core::rect<s32>(
                        screenHW()-dim.Width/2, screenHH()-dim.Height/2-50,
                        screenHW()+dim.Width/2, screenHH()+dim.Height/2-50
                        )
                    );
                img->setImage(tex);
                
                guienv->getSkin()->setColor(gui::EGDC_BUTTON_TEXT, video::SColor(255,255,77,0));
                guienv->getSkin()->setColor(gui::EGDC_3D_FACE, video::SColor(255,255,255,100));
                guienv->getSkin()->setColor(gui::EGDC_3D_DARK_SHADOW, video::SColor(255,150,150,60));
                guienv->getSkin()->setFont(guienv->getFont("sdcard/Irrlicht/media/blinkfont.xml"));
                
                device->getGUIEnvironment()->addButton(
                    core::rect<s32>(
                        centreX-bGap-bWidth1, screenHH()*2-bLow-bHeight,
                        centreX-bGap, screenHH()*2-bLow
                    ),
                    0, B_BUY, L"Buy Now");
                
                device->getGUIEnvironment()->addButton(
                    core::rect<s32>(
                        centreX+bGap, screenHH()*2-bLow-bHeight,
                        centreX+bGap+bWidth2, screenHH()*2-bLow
                    ),
                    0, B_NOBUY, L"No, thanks");
                
                //setMusic("media/demo.ogg");
            }
            else
            {
                device->closeDevice();
            }
#else
            __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "AndroidEventReceiver OnEvent()3");
            device->closeDevice();
#endif
            return true;
        }
        if (
            (
            (e.EventType == EET_KEY_INPUT_EVENT && e.KeyInput.PressedDown)
            ||
            (e.EventType == EET_MOUSE_INPUT_EVENT && (
            e.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN
            || e.MouseInput.Event == EMIE_MMOUSE_PRESSED_DOWN
            || e.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN))
            )
            && state != IN_DEMOSCREEN // otherwise this prevents GUI button clicking, for some reason
            )
        {
    __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "AndroidEventReceiver OnEvent()4");
            if (state == IN_GAME)
            {
    __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "AndroidEventReceiver OnEvent()5");
                jumpRabbit();
            }
            else if (state == IN_STARTSCREEN)
            {
                state = START_TO_GAMESCREEN;
/*    __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "AndroidEventReceiver OnEvent()6");
                removeText(startBlinkText);
                
                state = IN_GAME;
               // printf("NEW GAME %i\n", rand());
                __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "New Game");
                createRabbit();
                addCarrotDisplay();
*/                
              //  setMusic("media/ingame_loop.ogg");
            }
            else if (state == IN_ENDSCREEN)
            {
    __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "AndroidEventReceiver OnEvent()7");
              state = END_TO_STARTSCREEN;
              //  printf("START SCREEN %i\n", rand());
/*                
                __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "Start Screen");
                tEndCongrats->remove();
                tEndScore->remove();
                tEndCarrots->remove();
                if (textNewHighScore) removeText(textNewHighScore);
                tLastHighScore->remove();
                
                removeCarrotDisplay();
                resetCarrotsCollected();
                startScreen();
*/
            }
            return true;
        }
        return false;


/*    
    if (event.EventType == EET_GUI_EVENT)
    {
        s32 id = event.GUIEvent.Caller->getID();
        IGUIEnvironment* env = Context.device->getGUIEnvironment();

        __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "Lolo 2: %d %d %d %d", event.GUIEvent.EventType, EGET_BUTTON_CLICKED, id , GUI_ID_QUIT_BUTTON);
        switch(event.GUIEvent.EventType)
        {
            case EGET_BUTTON_CLICKED:
                __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "Lolo 3");
                switch(id)
                {
                    case GUI_ID_QUIT_BUTTON:
                        __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "Lolo 4");
                        if(gAppAlive ==7){
                        	 gAppAlive = 0;
                        }else{
                        	gAppAlive++;
                        }

                        return true;

                    default:
                        break;
                }
        }
    }
    return false;
*/
}
