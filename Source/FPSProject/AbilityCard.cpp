// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "FPSProject.h"
#include "AbilityCard.h"

AAbilityCard::AAbilityCard(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;

	Timer = 0.0f;
}

void AAbilityCard::BeginPlay()
{
	Super::BeginPlay();
}

void AAbilityCard::Tick(float DeltaTime)
{
	RotateAndBob(DeltaTime);
}

void AAbilityCard::RotateAndBob(float DeltaTime)
{
	FRotator CurrentRot = this->GetActorRotation();
	FRotator NewRot = CurrentRot.Add(0.0f, DeltaTime * RotationSpeed, 0.0f);
	this->SetActorRotation(NewRot);

	float WaveSlice = FMath::Sin(Timer);
	Timer += BobbingSpeed;
	if (Timer > PI * 20)
	{
		Timer -= PI * 20;
	}

	float TranslateChange = WaveSlice * BobbingAmount;
	FVector CurrentLoc = this->GetActorLocation();
	FVector NewLoc = CurrentLoc;
	NewLoc.Z += TranslateChange;
	this->SetActorLocation(NewLoc, false);
}

FString AAbilityCard::TypeString()
{
	FString OutputString;

	if (CardType.GetValue() == ECardType::Card_Grow)
	{
		OutputString = "Grow";
	}

	if (CardType.GetValue() == ECardType::Card_Shrink)
	{
		OutputString = "Shrink";
	}

	if (CardType.GetValue() == ECardType::Card_Jump)
	{
		OutputString = "Jump";
	}

	if (CardType.GetValue() == ECardType::Card_Tangible)
	{
		OutputString = "Tangible";
	}

	if (CardType.GetValue() == ECardType::Card_Intangible)
	{
		OutputString = "Intangible";
	}

	if (CardType.GetValue() == ECardType::Card_Intensifier)
	{
		OutputString = "Intensifier";
	}

	if (CardType.GetValue() == ECardType::Card_Multiplier)
	{
		OutputString = "Multiplier";
	}

	if (CardType.GetValue() == ECardType::Card_Heavy)
	{
		OutputString = "Heavy";
	}

	if (CardType.GetValue() == ECardType::Card_Light)
	{
		OutputString = "Light";
	}
	
	return OutputString;
}