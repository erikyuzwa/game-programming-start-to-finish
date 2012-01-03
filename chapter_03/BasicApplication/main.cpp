/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* It demonstrates the basic startup way of getting a Peon run application to work.
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
*
* @Goal: The goal of this sample is to demonstrate a real small sample of the Peon 
* engine.
*/
#include "PeonMain.h"

int main( int argc, char* argv[] )
{	
	
	new peon::EngineCore();
	
	if(!peon::EngineCore::getSingleton().loadEngine("BasicApplication", "data\\system.ini"))
	{
		return -1;
	}


	return( peon::EngineCore::getSingleton().runEngine() );

}

