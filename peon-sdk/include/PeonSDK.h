/**

Peon-SDK
---------

Peon-SDK - Copyright (c) 2002-2011 Erik Yuzwa - erik@erikyuzwa.com

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
   you must not claim that you wrote the original software.
   If you use this software in a product, an acknowledgment
   in the product documentation would be appreciated but
   is not required.

2. Altered source versions must be plainly marked as such,
   and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
   source distribution.
**/
#ifndef __PEONSDK_H_
#define __PEONSDK_H_

/**
* \mainpage Peon SDK
*
* \section intro_sec Introduction
*
* The Peon SDK collection of objects is meant to help the beginner learn and
* understand the fundamentals of game programming. The goal of the engine
* is to help you do things.<strong>It is not meant to be a heavy framework</strong>.
*
* I'm not against other frameworks (don't get me wrong), but sometimes they
* become so laden and burdened that it becomes difficult for the newbie
* to simply pick up the codebase and run with it quickly.
*
* \section license_sec License
*
* This game development SDK is licensed under the Creative Commons License.
*
*
* \section dll_sec DLL
*
* Why is this a Dynamically Linked Library?
* Although the <a href="http://wwww.libsdl.org/">Simple DirectMedia Layer</a> that the PeonSDK is built upon
* is a platform independent library, the Win32 version of the Peon SDK
* library compiles as a DLL. I chose this route, simply because it can make
* the support of this library easier in terms of upgrades for any customer
* buying/using your game built with Peon. Because the code links with the
* DLL during the runtime of your game, you are free to update/upgrade any
* code within so long as you don't change how the functions/classes are
* interfaced with your modules.
*
* \section dependencies_sec Dependencies
*
* The Peon SDK library/collection of objects is built upon some existing popular
* libraries that are already being used by professional game developers today!
*
* <i>Note: The license that comes with Peon SDK only covers the Peon SDK library itself.
* Each dependency has its own licensing terms that you are responsible for.</i>
*
* For the most part these are all successfull open source projects which
* are released under the LGPL (the same as Peon). Provided you retain the
* copyright headers of each project you can still release commercial
* software using them and retain all the bling.
*
* Here is a list of links to each of the license information pages
* for any dependent sub-projects the Peon library includes/uses.
*
*
* - SDL : <a href="http://www.libsdl.org/license.php">SDL v1.2.13</a>
* - Boost : <a href="http://www.boost.org/">Boost</a>
*
* \section install_sec Installation
*
* Check Appendix A in the book <b><i><a href="http://gameprogrammingstarttofinish.wazooinc.com/">"Game Programming in C++: Start To Finish"</a></i></b> for
* some instructions on getting this setup.
*
* Generally just make sure you let your IDE know where to find the /PeonSDK/include
* and /PeonSDK/lib folder. The /PeonSDK/lib folder contains the compiled
* version of this library, along with the DLL's that you need to distribute with
* any application using Peon.
*
* Having your installer copy them to the customer's windows\system32 folder is
* an option, or you can leave them all in the same folder as your binary executable.
* The choice is yours and just depends on how you want to support your own product.
* ie. your installer should "be nice" and remove them during an uninstall of
* your game, etc.
*
* \section building_sec Building the Library
*
* There should be the necessary project files for building the Peon library
* in:
* - Visual Studio .NET 2005 : PeonSDK_Home\win32\VS2005\PeonSDK_vs2005.sln
* - Visual Studio .NET 2008 : PeonSDK_Home\win32\VS2008\PeonSDK_vs2008.sln
*
*
* \section usage_sec General Usage
*
*
* The general usage for this library is meant to be (hopefully) very simple as
* I mentioned above. The basic practice is:
*
* - Derive a new application instance from IApplicationState
* - Toss it into the EngineCore
* - Enjoy game programming!
*
*
* Note: Always check <a href="http://www.erikyuzwa.com">http://www.erikyuzwa.com</a> for
* the latest version of this library and any updates.
*
* \section further_sec Further Resources
*
* For additional resources into 3D engines, feel free to download and inspect
* the source code to these awesome engines.
*
* <i>Note that this list is not ranked in any order</i>
*
* - <a href="http://www.ogre3d.com">Ogre3D - a rendering engine capable of OpenGL/D3D support</a>
* - <a href="http://irrlicht.sourceforge.net/">Irrlicht</a>
* - <a href="http://www.genesis3d.com/">Genesis3D</a>
* - <a href="http://www.crystalspace3d.org/tikiwiki/tiki-view_articles.php">Crystal Space 3D</a>
* - <a href="http://www.geometrictools.com">Wild Magic Software</a> - A fantastic collection of
* helpful open source math objects, and some additional rendering and pipeline design code.
*
* For a further list of 3D engines and their specifications, be sure
* to also visit the 3D engines database maintained at <a href="http://www.devmaster.net/engines">Devmaster.net</a>.
*
*/

#include "main/Peonstdafx.h"

#include "audio/PeonAudioEngine.h"
#include "main/PeonIniConfig.h"
#include "main/PeonEngineCore.h"
#include "main/PeonFileLogger.h"
#include "graphics/PeonMatrix44.h"
#include "graphics/PeonMatrix33.h"
#include "graphics/PeonSceneRenderer.h"
#include "graphics/PeonSceneFont.h"
#include "graphics/PeonSprite.h"
#include "graphics/PeonVector2.h"
#include "graphics/PeonVector3.h"
#include "graphics/PeonVector4.h"


/*
// This class is exported from the PeonSDK.dll
class PEONSDK_API CPeonSDK {
public:
	CPeonSDK(void);
	// TODO: add your methods here.
};

extern PEONSDK_API int nPeonSDK;

PEONSDK_API int fnPeonSDK(void);
*/

#endif
