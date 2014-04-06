// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "FPSProject.h"
#include "BlockState.h"


ABlockState::ABlockState(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
	this->CanJump = false;
	this->JumpTimer = 0.0f;
}

void ABlockState::BeginPlay()
{
	Super::BeginPlay();
}

void ABlockState::OnUsed(AController * user)
{
	if (!StaticMeshComponent)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Can't find Usable item"));
		return;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Blue, this->GetName());
	}
}

void ABlockState::DisplayPrompt(UCanvas * Canvas, AController * user)
{

}

void ABlockState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanJump)
	{
		if (JumpTimer < TimeBetweenJumps)
		{
			JumpTimer += DeltaTime;
		}
		else
		{
			JumpTimer = 0.0f;

			FBodyInstance* BodyInst = StaticMeshComponent->GetBodyInstance();
			
			if (!BodyInst)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Can't find body instance"));
				return;
			}

			BodyInst->AddImpulse(FVector(0, 0, JumpHeight), true);

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Tick"));
			}
		}
	}
}