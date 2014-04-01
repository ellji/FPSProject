// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Usable.generated.h"

/** This interface has to be added to every Actor that can be used, 
 *  you have to implement the OnUsed function
 */

UINTERFACE(MinimalAPI)
class UUsable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IUsable
{
	GENERATED_IINTERFACE_BODY()

	// this function will be called when the user uses the object
	virtual void OnUsed(AController * user);

	// this function is called each fram from the hud
	// it should be used to put messages to the screen
	// like the USE prompt in UDK
	virtual void DisplayPrompt(UCanvas * Canvas, AController * user);	
};
