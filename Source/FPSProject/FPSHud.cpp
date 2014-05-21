// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "FPSProject.h"
#include "CardInvUIWidget.h"
#include "FPSHud.h"


AFPSHud::AFPSHud(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("Texture2D'/Game/UI/Textures/crosshair.crosshair'"));
	CrosshairTex = CrosshairTexObj.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D> GrowTexObj(TEXT("Texture2D'/Game/UI/Textures/gui_card_grow.gui_card_grow'"));
	UICardTexAtlas.Add(GrowTexObj.Object);

	static ConstructorHelpers::FObjectFinder<UTexture2D> ShrinkTexObj(TEXT("Texture2D'/Game/UI/Textures/gui_card_shrink.gui_card_shrink'"));
	UICardTexAtlas.Add(ShrinkTexObj.Object);

	static ConstructorHelpers::FObjectFinder<UTexture2D> JumpTexObj(TEXT("Texture2D'/Game/UI/Textures/card_jump.card_jump'"));
	UICardTexAtlas.Add(JumpTexObj.Object);
}

void AFPSHud::BeginPlay()
{
	if (GetOwningPlayerController() != NULL)
	{
		PlayerController = Cast<AFPSCharacter>(GetOwningPlayerController()->GetPawn());
	}

	//// So far only TSharedPtr<SCardInvUIWidget> has been created, now create the actual object
	//// Create a SCardInvUIWidget on heap, our CardInvUIWidget shared pointer provides a handle to object
	//// widget will not self-destruct unless the hud's sharedptr (and all other sharedptrs) destruct first
	//SAssignNew(CardInvUIWidget, SCardInvUIWidget).OwnerHUD(this);

	//// pass our viewport a weak ptr to our widget
	//if (GEngine->IsValidLowLevel())
	//{
	//	GEngine->GameViewport->
	//		// viewport's weak ptr will not give viewport ownership of widget
	//		AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(CardInvUIWidget.ToSharedRef()));
	//}

	//if (CardInvUIWidget.IsValid())
	//{
	//	// set widget's properties as visible (sets child widget properties recursively
	//	CardInvUIWidget->SetVisibility(EVisibility::Visible);
	//}
}

void AFPSHud::DrawHUD()
{
	Super::DrawHUD();

	if (!Canvas) return;

	DrawCrosshair();

	if (PlayerController->ItemInventory.IsValidIndex(PlayerController->SelectedInventoryItem))
	{
		DrawCard(PlayerController->ItemInventory[PlayerController->SelectedInventoryItem].GetValue(), FVector2D(100.0f, 100.0f));
	}
}

void AFPSHud::DrawCrosshair()
{
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

void AFPSHud::DrawCard(ECardType::Type CardType, FVector2D Position)
{
	int32 Card = 0;

	switch (CardType)
	{
		case ECardType::Card_Grow:
			Card = 0; break;
		case ECardType::Card_Shrink:
			Card = 1; break;
		case ECardType::Card_Jump:
			Card = 2; break;
		default:
			Card = 0;
	}

	//Text and Font
	FCanvasTextItem NewText(
		FVector2D(10.0f, 100.0f),
		FText::FromString("Hello"),
		TextFont,
		FColor::Red
		);

	NewText.bOutlined = true;
	NewText.OutlineColor = FColor::Black;
	NewText.OutlineColor.A = FColor::Red.A * 2;

	Canvas->DrawItem(NewText);

	FCanvasTileItem TileItem(Position, UICardTexAtlas[Card]->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);
}



