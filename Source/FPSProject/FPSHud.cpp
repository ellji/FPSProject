// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "FPSProject.h"
//#include "CardInvUIWidget.h"
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

	LeftInset = 20.0f;
	BottomInset = 150.0f;
	HighlightScale = 1.2f;
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
	DrawInventory();
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

void AFPSHud::DrawInventory()
{
	FVector2D CardScale = FVector2D(1.0f, 1.0f);

	for (int32 i = 0; i < PlayerController->ItemInventory.Num(); i++)
	{
		// get the card type to draw
		ECardType::Type CardType = PlayerController->ItemInventory[i].GetValue();

		CardScale.X = UICardTexAtlas[CardType]->GetSurfaceWidth() * 0.2f;
		CardScale.Y = UICardTexAtlas[CardType]->GetSurfaceHeight() * 0.2f;
		
		FVector2D CardPosition = FVector2D(
			LeftInset + (i * UICardTexAtlas[CardType]->GetSurfaceWidth() * 0.2f),
			Canvas->ClipY - BottomInset);

		// make the card larger if it's the selected card
		if (i == PlayerController->SelectedInventoryItem)
		{
			// move the card up because it's selected
			CardPosition.Y = CardPosition.Y - ((CardScale.Y * HighlightScale) - CardScale.Y);

			CardScale.X = CardScale.X * HighlightScale;
			CardScale.Y = CardScale.Y * HighlightScale;
		}
		else if (i > PlayerController->SelectedInventoryItem)
		{
			CardPosition.X = CardPosition.X + (CardScale.X - (CardScale.X / HighlightScale));
		}

		DrawCard(CardType, CardPosition, CardScale);
		
		//reset
		CardScale = FVector2D(1.0f, 1.0f);
	}
}

void AFPSHud::DrawCard(ECardType::Type CardType, FVector2D Position, FVector2D Scale)
{
	FCanvasTileItem TileItem(Position, UICardTexAtlas[CardType]->Resource, Scale, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);
}

void AFPSHud::DrawTextString()
{
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
}



