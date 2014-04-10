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
