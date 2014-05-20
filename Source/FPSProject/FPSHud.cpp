// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "FPSProject.h"
#include "CardInvUIWidget.h"
#include "FPSHud.h"


AFPSHud::AFPSHud(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("Texture2D'/Game/UI/crosshair.crosshair'"));
	CrosshairTex = CrosshairTexObj.Object;
}

void AFPSHud::BeginPlay()
{
	// So far only TSharedPtr<SCardInvUIWidget> has been created, now create the actual object
	// Create a SCardInvUIWidget on heap, our CardInvUIWidget shared pointer provides a handle to object
	// widget will not self-destruct unless the hud's sharedptr (and all other sharedptrs) destruct first
	SAssignNew(CardInvUIWidget, SCardInvUIWidget).OwnerHUD(this);

	// pass our viewport a weak ptr to our widget
	if (GEngine->IsValidLowLevel())
	{
		GEngine->GameViewport->
			// viewport's weak ptr will not give viewport ownership of widget
			AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(CardInvUIWidget.ToSharedRef()));
	}

	if (CardInvUIWidget.IsValid())
	{
		// set widget's properties as visible (sets child widget properties recursively
		CardInvUIWidget->SetVisibility(EVisibility::Visible);
	}
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


