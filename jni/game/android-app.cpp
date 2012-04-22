// Copyright (C) 2002-2009 Laurent Mallet & Maxime Biais
// This file is part of the "Irrlicht Android Port".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <android/log.h>
#include <stdint.h>
#include <android-receiver.h>
#include <android-logger.h>
#include <importgl.h>

#include <irrlicht.h>
#include <os.h>

#include "event.h"
#include "rabbit.h"
#include "platforms.h"
#include "carrots.h"
#include "text.h"
#include "misc.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "android-app", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "android-app", __VA_ARGS__))

using namespace irr;

using namespace os;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

// global variables
int  gWindowWidth  = 320;
int  gWindowHeight = 480;
int  gAppAlive   = 1;
bool  OPEN_WEBPAGE_ON_EXIT = 0;
stringc gSdCardPath = "/sdcard/Irrlicht/";
int counter = 0;
int gameState = IN_STARTSCREEN;
IrrlichtDevice *device = NULL;
IVideoDriver* driver = NULL;
ISceneManager* smgr = NULL;

core::vector3df firstCameraPosition = core::vector3df(0,14.0,-20.0);
core::vector3df firstCameraTarget = core::vector3df(0,0,0);

ITimer *timer;
IAnimatedMeshSceneNode *nodeSydney;
IGUIEnvironment* guienv = NULL;
IAnimatedMesh* mesh = NULL;
IAnimatedMesh* roomMesh = NULL;
ISceneNode* node = NULL;
IAnimatedMeshSceneNode* roomNode = NULL;
ICameraSceneNode* camera = NULL;
IGUIStaticText* diagnostics = NULL;
SAppContext context;
AndroidEventReceiver *receiver;

Text *startBlinkText;

gui::IGUIStaticText *tEndCongrats;
gui::IGUIStaticText *tEndScore;
gui::IGUIStaticText *tEndCarrots;
gui::IGUIStaticText *tPosition;
Text *textNewHighScore;
gui::IGUIStaticText *tLastHighScore;

int lastFPS = -1;
u32 time_start;

static long _getTime(void) {
	struct timeval now;
	gettimeofday(&now, NULL);
	return (long) (now.tv_sec * 1000 + now.tv_usec / 1000);
}

void initQuake() {
	//appInit();

	smgr = device->getSceneManager();

	LOGI("getSceneManager r=%d", smgr);

	guienv = device->getGUIEnvironment();

	LOGI("getGUIEnvironment r=%d",guienv);

	//为什么这种方法，增加地图无效呢
	stringc quakeMapFilename = "/Irrlicht/map-20kdm2.pk3";
	//device->getFileSystem()->addZipFileArchive((gSdCardPath+quakeMapFilename).c_str());
	device->getFileSystem()->addFileArchive("/sdcard/irrlicht/map-20kdm2.pks");

	mesh = smgr->getMesh("20kdm2.bsp");
	//mesh = smgr->getMesh("/sdcard/Irrlicht/room.3ds");

	if (mesh) {
		LOGI("mesh available");
		node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
	} else {
		LOGI("mesh not available");
	}

	if (node) {
		LOGI( "node available");
		node->setPosition(core::vector3df(-1300, -144, -1249));
	} else {
		LOGI( "node not available");
	}

	camera = smgr->addCameraSceneNodeFPS();

	stringc logoFilename = "/Irrlicht/irrlichtlogoalpha2.tga";
	guienv->addImage(driver->getTexture((gSdCardPath + logoFilename).c_str()),
			core::position2d<s32>(10, 40));

	diagnostics = guienv->addStaticText(L"Ceci est un text", core::rect<s32>(
			10, 100, 10 + 400, 100 + 20));
	diagnostics->setOverrideColor(video::SColor(255, 255, 255, 0));

	guienv->addButton(rect<s32> (10, 10, 110, 10 + 32), 0, GUI_ID_QUIT_BUTTON,
			L"Quit", L"Exits Program");

	context.device = device;
	context.counter = 0;
	context.listbox = 0;

	// Then create the event receiver, giving it that context structure.
	receiver = new AndroidEventReceiver(context);

	// And tell the device to use our custom event receiver.
	device->setEventReceiver(receiver);

}

void initIrrFile() {

	LOGI( "initIrrFile1");
	smgr = device->getSceneManager();

	smgr->loadScene("sdcard/Irrlicht/ghdOne.irr");

	camera = smgr->addCameraSceneNodeFPS(0, 50.0f
			,0.1f);

	scene::IMetaTriangleSelector * meta = smgr->createMetaTriangleSelector();

	core::array<scene::ISceneNode *> nodes;
	smgr->getSceneNodesFromType(scene::ESNT_ANY, nodes); // Find all nodes

	LOGI("nodes.size === %d",nodes.size());
    scene::ISceneNode * rootNode = nodes[1];
	for (u32 i = 0; i < nodes.size(); ++i) {
		scene::ISceneNode * node = nodes[i];
		scene::ITriangleSelector * selector = 0;
        
	LOGI( "node.name ==%s",node->getName());
	LOGI( "node.position x== %d y== %d z == %d",node->getPosition().X,
                node->getPosition().Y, node->getPosition().Z);
		switch (node->getType()) {
		case scene::ESNT_CUBE:
		case scene::ESNT_ANIMATED_MESH:
		LOGI( "ESNT_CUBE/ ESNT_ANIMATED_MESH");
			// Because the selector won't animate with the mesh,
			// and is only being used for camera collision, we'll just use an approximate
			// bounding box instead of ((scene::IAnimatedMeshSceneNode*)node)->getMesh(0)
			selector = smgr->createTriangleSelectorFromBoundingBox(node);
			break;

		case scene::ESNT_MESH:
		case scene::ESNT_SPHERE: // Derived from IMeshSceneNode
		LOGI( "ESNT_MESH/ ESNT_SPHERE");
			selector = smgr->createTriangleSelector(
					((scene::IMeshSceneNode*) node)->getMesh(), node);
			break;

		case scene::ESNT_TERRAIN:
		LOGI( "ESNT_SPHERE");
			selector = smgr->createTerrainTriangleSelector(
					(scene::ITerrainSceneNode*) node);
			break;

		case scene::ESNT_OCTREE:
		LOGI( "ESNT_OCTREE");
			selector = smgr->createOctreeTriangleSelector(
					((scene::IMeshSceneNode*) node)->getMesh(), node);
			break;

		default:
			// Don't create a selector for this node type
		LOGI( "Don't create a selector for this node type");
			break;
		}

		if (selector) {
			// Add it to the meta selector, which will take a reference to it
			meta->addTriangleSelector(selector);
			// And drop my reference to it, so that the meta selector owns it.
			selector->drop();
		}
	}

	LOGI( "initIrrFile4");
//	scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
//			meta, camera, core::vector3df(5, 5, 5), core::vector3df(0, 0, 0));
//	meta->drop(); // I'm done with the meta selector now

//    scene::ISceneNodeAnimator* anim = 
//                    smgr->createFlyCircleAnimator(core::vector3df(0,0,0), 300 );

//	camera->addAnimator(anim);
//	anim->drop(); // I'm done with the animator now


	// And set the camera position so that it doesn't start off stuck in the geometry
	camera->setPosition(core::vector3df( 0.0f, 10.0f, 1.0f));
    //camera->setRotation(core::vector3df( 0.0f, 90.0f, 0.0f));
    // camera->setTarget(rootNode->getPosition());
	// Point the camera at the cube node, by finding the first node of type ESNT_CUBE
/*	scene::ISceneNode * cube = smgr->getSceneNodeFromType(scene::ESNT_CUBE);
	if (cube)
		camera->setTarget(cube->getAbsolutePosition());
*/
    smgr->setActiveCamera(camera);
    gameState = IN_GAME;
}

void initSydney() {
	LOGI( "initSydney");

	smgr = device->getSceneManager();

	guienv = device->getGUIEnvironment();

	stringc sydneyFilename = "/Irrlicht/sydney.md2";
	//stringc sydneyFilename = "/Irrlicht/room.3ds";

	stringc roomFilename = "/Irrlicht/room.3ds";

	mesh = smgr->getMesh((gSdCardPath + sydneyFilename).c_str());

	roomMesh = smgr->getMesh((gSdCardPath + roomFilename).c_str());

	u32 frameCount = mesh->getFrameCount();
	LOGI( "frameCount = %d",
			frameCount);

	//////////////////////////////////////////////////////////////////////////////
	//light node
/*	ISceneNode* lightNode = smgr->addLightSceneNode(0,
			core::vector3df(0, 0, 0), video::SColorf(0.0f, 1.0f, 0.0f, 0.0f),
			100.0f);

	scene::ISceneNodeAnimator* anim = 0;
	anim = smgr->createFlyCircleAnimator(core::vector3df(0, 150, 0), 250.0f);
	lightNode->addAnimator(anim);
	anim->drop();

	lightNode = smgr->addBillboardSceneNode(node,
			core::dimension2d<f32>(50, 50));
	lightNode->setMaterialFlag(video::EMF_LIGHTING, false);
	lightNode->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	lightNode->setMaterialTexture(0, driver->getTexture(
			"/sdcard/Irrlicht/particlewhite.bmp"));
*/
	//////////////////////////////////////////////////////////////////////////
	// I tried to add a particle system

	IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false);

	IParticleEmitter* em = ps->createBoxEmitter(core::aabbox3d<f32>(-7, 0, -7,
			7, 1, 7), // emitter size
			core::vector3df(0.0f, 0.06f, 0.0f), // initial direction
			80, 100, // emit rate
			video::SColor(0, 255, 255, 255), // darkest color
			video::SColor(0, 255, 255, 255), // brightest color
			800, 2000, 0, // min and max age, angle
			core::dimension2df(10.f, 10.f), // min size
			core::dimension2df(20.f, 20.f)); // max size

	ps->setEmitter(em); // this grabs the emitter
	em->drop(); // so we can drop it here without deleting it

	IParticleAffector* paf = ps->createFadeOutParticleAffector();

	ps->addAffector(paf); // same goes for the affector
	paf->drop();

	ps->setPosition(core::vector3df(-70, 60, 40));
	ps->setScale(core::vector3df(1, 1, 1));
	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("/sdcard/Irrlicht/fire.bmp"));
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);

	/////////////////////////////////////////////////////////////////////////////////
	//add room
	if (!roomMesh) {
		device->drop();
		LOGI( "cannot getMesh");
		return;
	} else {
		//roomNode = smgr->addOctreeSceneNode( roomMesh );

		roomNode = smgr->addAnimatedMeshSceneNode(roomMesh);
	}
	if (roomNode) {
		roomNode->setMaterialFlag(EMF_LIGHTING, false);
		stringc roomTextureFilename = "/Irrlicht/wall.jpg";
		roomNode->setMaterialTexture(0, driver->getTexture((gSdCardPath
				+ roomTextureFilename).c_str()));
		LOGI( "add texture");
	}

	////////////////////////////////////////////////////////////////////////////
	// add sydney
	if (!mesh) {
		device->drop();
		LOGI( "cannot getMesh");
		return;
	} else {
//		addAnimatedMeshSceneNode (IAnimatedMesh *mesh, ISceneNode *parent=0,
//				s32 id=-1, const core::vector3df &position=core::vector3df(0, 0, 0),
//				const core::vector3df &rotation=core::vector3df(0, 0, 0),
//				const core::vector3df &scale=core::vector3df(1.0f, 1.0f, 1.0f),
//				bool alsoAddIfMeshPointerZero=false)=0
		nodeSydney = smgr->addAnimatedMeshSceneNode(mesh,0,-1,
                        core::vector3df(-80, 70, 40));
    //    nodeSydney = smgr->addAnimatedMeshSceneNode(mesh);
	}
	if (nodeSydney) {
		f32 animationSpeed = nodeSydney->getAnimationSpeed();
		LOGI( "animationSpeed %f",
				animationSpeed);

		s32 endFrame = nodeSydney->getEndFrame();
		LOGI( "endFrame = %d",
				endFrame);

		f32 frameNr = nodeSydney->getFrameNr();
		LOGI( "frameNr = %f",
				frameNr);

		s32 startFrame = nodeSydney->getStartFrame();
		LOGI( "startFrmae = %d",
				startFrame);

		LOGI(
				"add texture111111111");
		nodeSydney->setMaterialFlag(EMF_LIGHTING, false);
    
        nodeSydney->setScale(vector3df(4.0f, 4.0f, 4.0f));
       
        nodeSydney->setRotation(vector3df(0,90,0));
     
		//--// no animation
		//nodeSydney->setMD2Animation(scene::EMAT_STAND);

		// with animation
		 nodeSydney->setMD2Animation(scene::EMAT_RUN);

		stringc sydneyTextureFilename = "/Irrlicht/sydney.bmp";

		nodeSydney->setMaterialTexture(0, driver->getTexture((gSdCardPath
				+ sydneyTextureFilename).c_str()));
		LOGI( "add texture");
	}

	//smgr->addCameraSceneNodeFPS(0, 0.0f, 0.1f);
	//ICameraSceneNode* CSceneManager::addCameraSceneNode(ISceneNode* parent,
	//const core::vector3df& position, const core::vector3df& lookat, s32 id,
	//bool makeActive)
        smgr->addCameraSceneNode(0, vector3df(0, 80, -400), vector3df(0, 80, 0));
	//    smgr->addCameraSceneNode(0,vector3df(0,0,0),vector3df(0,0,0));


	//以下三段代码，都会引起 ”called unimplemented OpenGL ES API“ Log

	stringc logoFilename = "sdcard/Irrlicht/irrlichtlogoalpha2.tga";
	guienv->addImage(driver->getTexture((gSdCardPath + logoFilename).c_str()),
			core::position2d<s32>(10, 40));

	diagnostics = guienv->addStaticText(L"Ceci est un text", core::rect<s32>(
			10, 100, 10 + 400, 100 + 20));
	diagnostics->setOverrideColor(video::SColor(255, 255, 255, 0));

	guienv->addButton(rect<s32> (10, 10, 110, 10 + 32), 0, GUI_ID_QUIT_BUTTON,
			L"Quit", L"Exits Program");

	context.device = device;
	context.counter = 0;
	context.listbox = 0;

	// Then create the event receiver, giving it that context structure.
	receiver = new AndroidEventReceiver(context);

	// And tell the device to use our custom event receiver.
	device->setEventReceiver(receiver);
}

//这个例子就是运行不成功
void initIrr() {

	// Printer::Logger = new CAndroidLogger();

	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	stringc _3dsFilename = "/Irrlicht/room.3ds";
	mesh = smgr->getMesh((gSdCardPath + _3dsFilename).c_str());
	LOGI( "mesh: %d", mesh);

	// smgr->getMeshManipulator()->makePlanarTextureMapping(mesh->getMesh(0), 0.004f);

	if (!mesh) {
		device->drop();
		LOGI( "cannot getMesh");
		return;
	}

	nodeSydney = smgr->addAnimatedMeshSceneNode(mesh);
	stringc _3dsTextureFilename = "/Irrlicht/wall.jpg";

	nodeSydney->setMaterialTexture(0, driver->getTexture((gSdCardPath
			+ _3dsTextureFilename).c_str()));
	nodeSydney->getMaterial(0).SpecularColor.set(0, 0, 0, 0);

	/*
	 mesh = smgr->addHillPlaneMesh( "myHill",
	 core::dimension2d<f32>(20,20),
	 core::dimension2d<u32>(40,40), 0, 0,
	 core::dimension2d<f32>(0,0),
	 core::dimension2d<f32>(10,10));

	 node = smgr->addWaterSurfaceSceneNode(mesh->getMesh(0), 3.0f, 300.0f, 30.0f);
	 node->setPosition(core::vector3df(0,7,0));

	 stringc stonesFilename = "/Irrlicht/stones.jpg";
	 node->setMaterialTexture(0, driver->getTexture( (gSdCardPath+stonesFilename).c_str() ));
	 stringc waterFilename = "/Irrlicht/water.jpg";
	 node->setMaterialTexture(1, driver->getTexture( (gSdCardPath+waterFilename).c_str() ));

	 node->setMaterialType(video::EMT_REFLECTION_2_LAYER);

	 // create light

	 node = smgr->addLightSceneNode(0, core::vector3df(0,0,0),
	 video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 800.0f);
	 scene::ISceneNodeAnimator* anim = 0;
	 anim = smgr->createFlyCircleAnimator (core::vector3df(0,150,0),250.0f);
	 node->addAnimator(anim);
	 anim->drop();
	 */

	camera = smgr->addCameraSceneNodeFPS();
	//camera->setPosition(core::vector3df(-50,50,-150));
	camera->setPosition(core::vector3df(-50, 0, -50));

}

void natvieDrawIterationIrrFile() {
	//LOGI( "natvieDrawIterationIrrFile");
	device->run();
	if (counter == 0) {
		initIrrFile();
	}

	driver->beginScene(true, true, SColor(255, 0, 0, 0));
//	driver->beginScene(true, true, SColor(0,200,200,200));
	smgr->drawAll();
	driver->endScene();

	counter++;
}

//static int counter = 0;
/* Call to render the next GL frame */
void nativeDrawIterationSydney() {
	device->run();
	if (counter == 0) {
		initSydney();
	}

	if (gAppAlive == 1) {
		if (nodeSydney)
			nodeSydney->setMD2Animation(scene::EMAT_STAND);
	} else if (gAppAlive == 2) {
		if (nodeSydney)
			nodeSydney->setMD2Animation(scene::EMAT_RUN);
	}

	// SColor is the background color
	driver->beginScene(true, true, SColor(255, 0, 0, 0));

	smgr->drawAll();
	guienv->drawAll();

	driver->endScene();

	counter++;

	int fps = driver->getFPS();
	 //LOGI( "fps=%d", fps);

}

/* Call to render the next GL frame */
void nativeDrawIterationQuake() {
	device->run();
	if (counter == 0) {
		initQuake();
	}

	if (node) {
		node->setPosition(core::vector3df(-1300, -144, -1249));
		camera->setRotation(core::vector3df(0, counter, 0));
	}

	driver->beginScene(true, true, SColor(255, 0, 0, 0));

	smgr->drawAll();
	guienv->drawAll();
	driver->endScene();

	counter++;

	int fps = driver->getFPS();
	LOGI( "fps=%d", fps);

}

void nativeDrawIterationIrr() {
	device->run();
	if (counter == 0) {
		LOGI( "Load irr scene");
		initIrr();
	}

	driver->beginScene(true, true, SColor(255, 0, 0, 0));

	smgr->drawAll();
	guienv->drawAll();
	driver->endScene();

	counter++;

	int fps = driver->getFPS();
	//   LOGI( "fps=%d", fps);

}


void preloadStuff()
{
	LOGI( "preloadStuff");
    timer->stop();
    //gui::IGUIStaticText *loadText = staticText(L"Loading...", screenHW(),screenHH(), 4, "media/bigfont.xml");
    driver->beginScene(true, true, video::SColor(0,255,255,120));
    smgr->drawAll();
    guienv->drawAll();
    driver->endScene();
    
    smgr->getMesh("sdcard/Irrlicht/plat0.obj");
    smgr->getMesh("sdcard/Irrlicht/plat1.obj");
    smgr->getMesh("sdcard/Irrlicht/plat2.obj");
    loadFonts();
    driver->getTexture("sdcard/Irrlicht/demo.png");
    
    //loadText->remove();
    timer->start();
}


void initRabbit(){
    
	LOGI( "initRabit");
	smgr = device->getSceneManager();
    driver = device->getVideoDriver();
	guienv = device->getGUIEnvironment();
    timer = device->getTimer();

    smgr->loadScene("sdcard/Irrlicht/ghdOne.irr");

    camera = smgr->addCameraSceneNodeFPS(NULL, 0,0);                                            
    
    camera->setFOV(M_PI/2.0);
    
    scene::ILightSceneNode *light = smgr->addLightSceneNode(camera, core::vector3df(-100,1000,0),
        video::SColorf(0.5f, 0.5f, 0.5f, 0.5f), 400.0f);
    video::SLight &slight = light->getLightData();
    slight.AmbientColor = video::SColorf(0.8,0.8,0.8,0.8);
    
    preloadStuff();
    
	// Then create the event receiver, giving it that context structure.
	receiver = new AndroidEventReceiver(context);

	// And tell the device to use our custom event receiver.
	device->setEventReceiver(receiver);

    startScreen();
    
    time_start = timer->getTime();
    
    srand(time_start);
    
    lastFPS = -1;
	
/*	// Then create the event receiver, giving it that context structure.
	receiver = new AndroidEventReceiver(context);

	LOGI( "initRabbit5");
	// And tell the device to use our custom event receiver.
	device->setEventReceiver(receiver);
*/
}

void initGameScreen(){
	initRabbit();
}

void demoGameScreen(){
}

void runningGameScreen(){
}

void endGameScreen(){
}

void nativeDrawIterationRabbit(){

    double dt;
    core::stringw windowCaption = L"Run Rabbit RUN!";

	if (counter == 0) {
		LOGI( "Load irr scene");
        initGameScreen();
	}

	counter++;

    device->run(); 

    //while(device->run())
    {
//		LOGI( "running");
        if (device->isWindowActive())
        {
//		LOGI( "running1");
            if (timer->isStopped())
            {
                timer->start();
            }

////////////////////////////////////////////////////////////////////
//enum {IN_STARTSCREEN = 0, IN_GAME = 1, IN_ENDSCREEN = 2, IN_DEMOSCREEN = 3};

            if ( (timer->getTime() - time_start >= 1000.0 / 60.0) && device->isWindowActive())
            {

//		LOGI( "running2");
                dt = (double)(timer->getTime() - time_start) / 1000.0f;

               if (state != IN_DEMOSCREEN)
                {

//		LOGI( "running3");
                    createPlatformUpdate(timer->getTime()); // platform generator
                    updatePlatforms(dt);
                    rotateCarrotIcon();
                }
  

                if(state == START_TO_GAMESCREEN) {

                    removeText(startBlinkText);
                
                    state = IN_GAME;
                    // printf("NEW GAME %i\n", rand());
                     LOGI( "New Game");
                     createRabbit();
                     addCarrotDisplay();

                     setMusic("media/ingame_loop.ogg");

                }
                else if (state == IN_GAME)
                {
//		LOGI( "running4");
                    updatePlatformDifficulty(dt);
                    updateRabbit(dt);
                    
                    int steps = 10;
                    for (int i = 0; i < steps; i ++)
                    {
                        updateRabbit(dt/(double)steps);
                    }
                    
                    updateCarrots();
                    
                    if (rabbitDied())// && state != IN_ENDSCREEN)
                    {
                        removeRabbit();
                        removePlatforms();
                        endScreen();
                    //    printf("END GAME %i\n", rand());
                    }
                }
                else if(state == END_TO_STARTSCREEN){
                     LOGI( "Start Screen");
                     tEndCongrats->remove();
                     tEndScore->remove();
                     tEndCarrots->remove();
                     if (textNewHighScore) removeText(textNewHighScore);
                     tLastHighScore->remove();
                
                     removeCarrotDisplay();
                     resetCarrotsCollected();
                     startScreen();
                }               
 
                updateTexts(dt);
                
                driver->beginScene(true, true, video::SColor(0,255,255,120));
                smgr->drawAll();
                guienv->drawAll();
                driver->endScene();
                
                time_start = timer->getTime();
            }
////////////////////////////////////////////////////////////////////

            int fps = driver->getFPS();
            
            if (lastFPS != fps)
            {
                core::stringw str = windowCaption;
                str += " - [";
                str += driver->getName();
                str += "] FPS:";
                str += fps;
                device->setWindowCaption(str.c_str());
                lastFPS = fps;
            }
        }
        else
        {
            if (!timer->isStopped())
            {
                timer->stop();
            }
        }
    }
    
//    stopMusic();
//    device->drop();
//    sengine->drop();
    
}

void nativeDrawIteration() {
	//	if(gAppAlive == 3){
	//		nativeDrawIterationIrr();
	//	}else if(gAppAlive ==1){
//	nativeDrawIterationSydney();
	//	}else if(gAppAlive ==5){
//			nativeDrawIterationQuake();
	//	}else if(gAppAlive ==2){
	//		natvieDrawIterationIrrFile();
	//	}


//		natvieDrawIterationIrrFile();
    nativeDrawIterationRabbit();
}

