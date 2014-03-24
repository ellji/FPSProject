// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "FPSProject.h"
#include "FPSHud.h"


AFPSHud::AFPSHud(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("Texture2D'/Game/UI/crosshair.crosshair'"));
	CrosshairTex = CrosshairTexObj.Object;
}

void AFPSHud::DrawHUD()
{
	Super::DrawHUD();

	// draw simple crosshair
	// in the middle of the screen
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	// offset by half the texture's dimensions to align in the centre
	const FVector2D CrosshairDrawPosition(
		(Center.X - CrosshairTex->GetSurfaceWidth() * 0.5f),
		(Center.Y - CrosshairTex->GetSurfaceHeight() * 0.5f));
	// draw
	FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);
}


