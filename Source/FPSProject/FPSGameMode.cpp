// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "FPSProject.h"
#include "FPSGameMode.h"
#include "FPSCharacter.h"
#include "FPSHud.h"

AFPSGameMode::AFPSGameMode(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// set default pawn class to a blueprinted character
	static ConstructorHelpers::FObjectFinder<UBlueprint> PlayerPawnObject(TEXT("Blueprint'/Game/Blueprints/BP_FPSCharacter.BP_FPSCharacter'"));
	if (PlayerPawnObject.Object != NULL)
	{
		DefaultPawnClass = (UClass*)PlayerPawnObject.Object->GeneratedClass;
	}

	HUDClass = AFPSHud::StaticClass();
}

void AFPSGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("FUCK THIS"));
	}
}