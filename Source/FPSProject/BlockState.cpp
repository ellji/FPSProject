// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "FPSProject.h"
#include "BlockState.h"


ABlockState::ABlockState(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;

	this->CanJump = false;
	this->JumpTimer = 0.0f;
	this->JumpTimer = 5.0f;
	this->JumpHeight = 10.0f;

	this->ScaleAmount = 1.0f;

	this->DoGrow = false;
	this->GrowFactor = 1.33f;

	this->DoShrink = false;
	this->ShrinkFactor = 0.66f;
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

	if (DoGrow)
	{
		ScaleAmount += DeltaTime;
		StaticMeshComponent->SetWorldScale3D(OriginalScale * ScaleAmount);
		if (ScaleAmount >= GrowFactor)
		{
			DoGrow = false;
			ScaleAmount = 1.0f;
		}
	}

	if (DoShrink)
	{
		ScaleAmount -= DeltaTime;
		StaticMeshComponent->SetWorldScale3D(OriginalScale * ScaleAmount);
		if (ScaleAmount <= ShrinkFactor)
		{
			DoShrink = false;
			ScaleAmount = 1.0f;
		}
	}
}

void ABlockState::ReceiveCard(ECardType::Type UsedCard)
{
	if (UsedCard == ECardType::Card_Grow)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Magenta, TEXT("Grow"));
		}
		
		OriginalScale = StaticMeshComponent->GetComponentScale();
		DoGrow = true;
	}

	if (UsedCard == ECardType::Card_Shrink)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Magenta, TEXT("Shrink"));
		}

		OriginalScale = StaticMeshComponent->GetComponentScale();
		DoShrink = true;
	}
}