//Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "FPSProject.h"
#include "CardInvUIWidget.h"


void SCardInvUIWidget::Construct( const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;

	// if the code below doesn't look like c++ to you it's because it kind of isn't.
	// slate makes use of c++ preprocessor(macros) and operator overloading,
	// epic is trying to make our lives easier, look up the macro/operator definitions to see why
	ChildSlot
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Center)
			[
				SNew(STextBlock)
				.ShadowColorAndOpacity(FLinearColor::Black)
				.ColorAndOpacity(FLinearColor::White)
				.ShadowOffset(FIntPoint(-1,1))
				.Font(FSlateFontInfo("Verdana", 16)) // not sure if works, see rama's tut
				.Text(FText::FromString("Hello, Slate!"))
			]
		];
}