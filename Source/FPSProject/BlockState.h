// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "Tickable.h"
#include "Usable.h"
#include "AbilityCard.h"
#include "BlockState.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ABlockState : public AStaticMeshActor, public IUsable
{
	GENERATED_UCLASS_BODY()

	virtual void BeginPlay() OVERRIDE;

	public:

		// IUsable
		virtual void OnUsed(AController * user) OVERRIDE;
		virtual void DisplayPrompt(UCanvas * Canvas, AController * user) OVERRIDE;
		virtual void Tick(float DeltaTime) OVERRIDE;
		void ReceiveCard(ECardType::Type UsedCard);

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
			bool CanJump;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
			float JumpTimer;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
			float TimeBetweenJumps;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
			float JumpHeight;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
			float GrowFactor;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
			float ShrinkFactor;

	private:
		bool DoGrow;
		bool DoShrink;
		float ScaleAmount;
		FVector OriginalScale;
};