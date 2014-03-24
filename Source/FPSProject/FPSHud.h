// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/HUD.h"
#include "FPSHud.generated.h"

/**
 * 
 */
UCLASS()
class AFPSHud : public AHUD
{
	GENERATED_UCLASS_BODY()

	public:
		/** primary draw call for the hud */
		virtual void DrawHUD() OVERRIDE;

	private:
		/** crosshair asset pointer */
		UTexture2D* CrosshairTex;
};
