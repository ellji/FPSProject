//Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "FPSHud.h"
#include "Slate.h"

class SCardInvUIWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SCardInvUIWidget)
	{}
	
	/* see private declaration of OwnerHUD below*/
	SLATE_ARGUMENT(TWeakObjectPtr<AFPSHud>, OwnerHUD)

	SLATE_END_ARGS()

public:
	// needed for every widget
	// builds this widget and any of it's children
	void Construct(const FArguments& InArgs);

private:
	// pointer to our parent hud
	// to make sure HUD's lifetime is controlled elsewhere, use 'weak' ptr
	// HUD has 'strong' ownership to widget
	// circular ownership would prevent/break self-destruction of hud/widget (cause memory leak)
	TWeakObjectPtr<class AFPSHud> OwnerHUD;
};