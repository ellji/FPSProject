// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "Materials/Material.h"
#include "AbilityCard.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
namespace ECardType
{
	// max 256 entries, numbered 0-255
	enum Type
	{
		Card_Grow			UMETA(DisplayName = "Grow"),
		Card_Shrink			UMETA(DisplayName = "Shrink"),
		Card_Jump			UMETA(DisplayName = "Jump"),
		Card_Tangible		UMETA(DisplayName = "Tangible"),
		Card_Intangible		UMETA(DisplayName = "Intangible"),
		Card_Intensifier	UMETA(DisplayName = "Intensifier"),
		Card_Multiplier		UMETA(DisplayName = "Multiplier"),
		Card_Heavy			UMETA(DisplayName = "Heavy"),
		Card_Light			UMETA(DisplayName = "Light")
	};
}

UCLASS(Blueprintable, BlueprintType)
class AAbilityCard : public AStaticMeshActor
{
	GENERATED_UCLASS_BODY()

	public:
		virtual void BeginPlay() OVERRIDE;
		virtual void Tick(float DeltaTime) OVERRIDE;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Type")
			TEnumAsByte<ECardType::Type> CardType;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
			float RotationSpeed;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
			float BobbingSpeed;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
			float BobbingAmount;

	private:
		float Timer;

		void RotateAndBob(float DeltaTime);
};
