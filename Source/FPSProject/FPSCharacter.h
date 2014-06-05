// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "BlockState.h"
#include "AbilityCard.h"
#include "FPSCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AFPSCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	public:
		virtual void BeginPlay() OVERRIDE;
		virtual void Tick(float DeltaTime) OVERRIDE;

		virtual void ReceiveHit(
			class UPrimitiveComponent* MyComp,
			class AActor* Other,
			class UPrimitiveComponent* OtherComp,
			bool bSelfMoved,
			FVector HitLocation,
			FVector HitNormal,
			FVector NormalImpulse,
			const FHitResult& Hit
			) OVERRIDE;

		FString TypeString();

		/** First Person Camera */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
			TSubobjectPtr<UCameraComponent> FirstPersonCameraComponent;
		/** Pawn Mesh: 1st person view (arms, only seen by self) */
		UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
			TSubobjectPtr<USkeletalMeshComponent> FirstPersonMesh;
		/** gun muzzle's offset from the camera location */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
			FVector MuzzleOffset;
		/** arm distance */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
			float PlayerInteractionDistance;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
			bool BlockGrabbed;
		
		// block we've picked up
		ABlockState* Pickup;
		// block we're standing on
		ABlockState* Standing;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
			TSubobjectPtr<UPhysicsHandleComponent> PhysicsHandleComponent;

		UPROPERTY(EditAnywhere, Category = Inventory)
			TArray< TEnumAsByte<ECardType::Type> > ItemInventory; // Inventory

		UPROPERTY(EditAnywhere, Category = Inventory)
			int32 SelectedInventoryItem; // Inventory

	protected:
		virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) OVERRIDE;

		bool DoTrace(FVector Start, FVector End, FHitResult* RV_Hit, FCollisionQueryParams* RV_TraceParams);
		void StandingOn();
		FString GetCardEnumAsString(ECardType::Type EnumValue);

		//handles moving forward/backward
		UFUNCTION()
			void MoveForward(float Val);
		//handles strafing
		UFUNCTION()
			void MoveRight(float Val);

		//sets jump flag when key is pressed
		UFUNCTION()
			void OnStartJump();
		//clears jump flag when key is released
		UFUNCTION()
			void OnStopJump();

		//handles firing
		UFUNCTION()
			void OnFire();

		// handles using
		UFUNCTION()
			void OnUse();

		UFUNCTION()
			void InvLeft();

		UFUNCTION()
			void InvRight();

	private:
};