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

		//Called as soon as game starts, create SCompoundWidget and give Viewport access
		void BeginPlay();

		//Reference to an SCompoundWidget, TSharedPtr adds to the refcount of MyUIWidget
		//MyUIWidget will not self-destruct as long as refcount > 0
		//MyUIWidget refcount will be (refcout-1) if HUD is destroyed.
		TSharedPtr<class SCardInvUIWidget> CardInvUIWidget;

	private:
		/** crosshair asset pointer */
		UTexture2D* CrosshairTex;
};
