/*
 * AppDemarrage.cpp
 *
 *  Created on: 8 mai 2014
 *      Author: sym
 */

#include "AppDemarrage.h"

AppDemarrage::AppDemarrage(): mRoot(0), mWindow(0), mSceneMgr(0), mCamera(0) {

}

AppDemarrage::~AppDemarrage() {
	delete mRoot;
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
		if(!mRoot->renderOneFrame())
			return false;
	}

	return true;
}

void AppDemarrage::createCamera()
{
	mCamera = mSceneMgr->createCamera("Ma Camera");
	// Position de départ
	mCamera->setPosition(Ogre::Vector3(0, 0, 100));
	// Direction vers laquelle regarde la caméra au départ
	mCamera->lookAt(Ogre::Vector3(0.0, 0.0, 0.0));
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
	// Lumière ambiante :
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.1, 0.1, 0.1));

	// Lumière diffuse :
	Ogre::Light* light = mSceneMgr->createLight("light1");
	light->setPosition(100, 300, 200);
	light->setDiffuseColour(1.0, 1.0, 1.0);

	// Ajout des modèles 3D
	Ogre::Entity *ent = mSceneMgr->createEntity("protecteur", "Cylinder.mesh");
	Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(ent);
}

void AppDemarrage::createFrameListener()
{
    mInputListener = new InputListener(mWindow, mCamera);
    mRoot->addFrameListener(mInputListener);
}
