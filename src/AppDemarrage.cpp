/*
 * AppDemarrage.cpp
 *
 *  Created on: 8 mai 2014
 *      Author: sym
 */

#include "AppDemarrage.h"

AppDemarrage::AppDemarrage(): mRoot(0), mWindow(0), mSceneMgr(0), mCamera(0), mInputListener(0), mNodePersonnage(0) {

}

AppDemarrage::~AppDemarrage() {
	delete mRoot;
//	delete mWindow;
//	delete mSceneMgr;
//	delete mCamera;
//	delete mInputListener;
}

bool AppDemarrage::start() {
	//création du root
	mRoot = new Ogre::Root("plugins.cfg", "ogre.cfg", "Ogre.log");

	//chargement des ressources
	Ogre::ConfigFile configFile;
	configFile.load("resources.cfg");

	//Ajout des path des ressources dans le ResourceGroupManager
	Ogre::ConfigFile::SectionIterator seci = configFile.getSectionIterator();
	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
	    secName = seci.peekNextKey();
	    Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
	    Ogre::ConfigFile::SettingsMultiMap::iterator i;
	    for (i = settings->begin(); i != settings->end(); ++i)
	    {
	        typeName = i->first;
	        archName = i->second;
	        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
	            archName, typeName, secName);
	    }
	}

	//-----------------------------------
	//Chargement du moteur en lui-même :
	//-----------------------------------
	if(!(mRoot->restoreConfig() || mRoot->showConfigDialog())) //showConfigDialog = fenêtre choix de configuration qui renvoie true si utilisateur clique sur OK
	{
	    return false;
	}

	//Configuration manuelle du RenderSystem :

	/*Ogre::RenderSystem *rs = mRoot->getRenderSystemByName("Direct3D9 Rendering Subsystem"); // Pour OpenGL : "OpenGL Rendering Subsystem"
	mRoot->setRenderSystem(rs);
	rs->setConfigOption("Full Screen", "No");
	rs->setConfigOption("Video Mode", "1280 x 800 @ 32-bit colour");
	rs->setConfigOption("VSync", "Yes");*/

	// Création de la fenêtre
	mWindow = mRoot->initialise(true, "Ma fenetre"); // 1er param : ogre crée la fenêtre lui-même ou pas, 2e param: titre de la fenêtre

	// Initialisation des resources et du nombre de MipMap par défaut:
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// Création du scene manager :
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager", "Mon Scene Manager");
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));

	// creation de la camera et du viewport
	createCamera();
	createViewports();

	//remplissage de la scene
	createScene();

	// instancie le FrameListener
	createFrameListener();

	// Boucle infinie "toute faite" par Ogre
	// mRoot->startRendering();

	//Boucle infinie manuelle :
	while(true)
	{
		// Methode qui prend en charge le raffraichissement de la fenêtre :
		Ogre::WindowEventUtilities::messagePump();

		// Pour stopper le processus quand on ferme la fenêtre:
		if(mWindow->isClosed())
		    return false;

		// Pour faire le rendu
		if(!mRoot->renderOneFrame()) // Fait le rendu d'une image
			return false;
	}

	return true;
}

void AppDemarrage::createCamera()
{
	mCamera = mSceneMgr->createCamera("Ma Camera");
	// Position de départ
    
    
    mNodePersonnage = mSceneMgr->getRootSceneNode()->createChildSceneNode("nodePersonnage");
    
    mNodePersonnage->setPosition(Ogre::Vector3(0, 9 , 0));
    mNodeCamera = mNodePersonnage->createChildSceneNode("nodeCamera");
    mNodeCamera->attachObject(mCamera);
    mCamera->setPosition(Ogre::Vector3(0, 7, -20));
    
	// Direction vers laquelle regarde la caméra au départ
	mCamera->lookAt(Ogre::Vector3(0.0, 7.0, 0.0));
	// Distance la plus proche à afficher
	mCamera->setNearClipDistance(1);
	// Distance la plus loin à afficher
	mCamera->setFarClipDistance(20000);
	// On vérifie si l'ordinateur en est capable, si c'est le cas, on fixe la distance de vue à l'infini.
//	if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
//	    mCamera->setFarClipDistance(0);
}
void AppDemarrage::createViewports()
{
	// Viewport = "une vue 3D"
	Ogre::Viewport *vue = mWindow->addViewport(mCamera);
	vue->setBackgroundColour(Ogre::ColourValue(0.2, 0.3, 1.0));
	mCamera->setAspectRatio(Ogre::Real(vue->getActualWidth()) / Ogre::Real(vue->getActualHeight()));
}

void AppDemarrage::createScene()
{
    // Réglage de la précision des textures pour les matériaux (ici anisotrope).
//    Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
//    Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(8);
    
	// Lumière ambiante :
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.3, 0.3, 0.3));

	// Lumière diffuse :
	/*Ogre::Light* light = mSceneMgr->createLight("light1");
	light->setPosition(100, 300, 200);
	light->setDiffuseColour(1.0, 1.0, 1.0);*/
    
    mLight = mSceneMgr->createLight("light1");
    mLight->setType(Ogre::Light::LT_DIRECTIONAL);
    mLight->setDirection(10.0, -20.0, -5);
    mLight->setDiffuseColour(0.8, 0.8, 0.8);
    
    mLight->setCastShadows(true);

	// Ajout des modèles 3D
    Object3D *ent = new Object3D(mSceneMgr, "protecteur", "Cylinder.mesh", mNodePersonnage, Ogre::Vector3(0, -5, 0));
//    Ogre::Entity *ent = mSceneMgr->createEntity("protecteur", "Cylinder.mesh");
//    mNode->setPosition(Ogre::Vector3(0, 0, 100));
//	mNode->attachObject(ent);
    
    //Ajout d'un plan pour le sol
    Ogre::Plane plan(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("sol", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plan, 500, 500, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);
    Ogre::Entity *entSol= mSceneMgr->createEntity("EntiteSol", "sol");
    mNodeSol = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mNodeSol->attachObject(entSol);
    entSol->setMaterialName("grassFloor");
    
    // Ajout du robot
	mRobot = mSceneMgr->createEntity("Robot", "robot.mesh");
	mRobotNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("RobotNode");
	mRobotNode->attachObject(mRobot);
    mRobotNode->translate(Ogre::Vector3(100,0,100));
    
//    createTerrain();
}

void AppDemarrage::createFrameListener()
{
	mAnimationState = mRobot->getAnimationState("Walk");
	mAnimationState->setLoop(true);
	mAnimationState->setEnabled(false);

    mInputListener = new InputListener(mWindow, mCamera, mNodePersonnage, mAnimationState);
    mRoot->addFrameListener(mInputListener);
}

//void AppDemarrage::createTerrain()
//{
//    mTerrain = OGRE_NEW Ogre::Terrain(mSceneMgr);
//    
//    // options globales
//    mTerrainOptions = OGRE_NEW Ogre::TerrainGlobalOptions();
//    mTerrainOptions->setMaxPixelError(10);
//    mTerrainOptions->setCompositeMapDistance(8000);
//    mTerrainOptions->setLightMapDirection(mLight->getDerivedDirection());
//    mTerrainOptions->setCompositeMapAmbient(mSceneMgr->getAmbientLight());
//    mTerrainOptions->setCompositeMapDiffuse(mLight->getDiffuseColour());
//    
//    Ogre::Image img;
//    img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
//    
//    // informations géométriques
//    Ogre::Terrain::ImportData imp;
//    imp.inputImage = &img;
//    imp.terrainSize = img.getWidth();
//    imp.worldSize = 8000;
//    imp.inputScale = 600;
//    imp.minBatchSize = 33;
//    imp.maxBatchSize = 65;
//    
//    // textures
//    imp.layerList.resize(1);
//    imp.layerList[0].worldSize = 100;
//    imp.layerList[0].textureNames.push_back("terrain.png");
//    imp.layerList[0].textureNames.push_back("terrain.png");
//    mTerrain->prepare(imp);
//    mTerrain->load();
//    
//    // plaquage de texture
//    Ogre::TerrainLayerBlendMap* blendMap1 = mTerrain->getLayerBlendMap(1);
//    float* pBlend1 = blendMap1->getBlendPointer();
//    
//    for (Ogre::uint16 y = 0; y < mTerrain->getLayerBlendMapSize(); ++y)
//    {
//        for (Ogre::uint16 x = 0; x < mTerrain->getLayerBlendMapSize(); ++x)
//        {
//            *pBlend1++ = 150;
//        }
//    }
//    
//    blendMap1->dirty();
//    blendMap1->update();
//    
//    mTerrain->freeTemporaryResources();
//}
