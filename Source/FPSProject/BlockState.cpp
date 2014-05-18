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

void ABlockState::ReceiveCard(ECardType::Type UsedCard)
{
	if (UsedCard == ECardType::Card_Grow)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Magenta, TEXT("Grow"));
		}

		FVector oldScale = StaticMeshComponent->GetComponentScale();
		StaticMeshComponent->SetWorldScale3D(oldScale * 1.7f);
	}

	if (UsedCard == ECardType::Card_Shrink)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Magenta, TEXT("Shrink"));
		}
	}
}