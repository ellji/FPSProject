// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "BlockState.h"
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

		/** First Person Camera */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
			TSubobjectPtr<UCameraComponent> FirstPersonCameraComponent;
		/** Pawn Mesh: 1st person view (arms, only seen by self) */
		UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
			TSubobjectPtr<USkeletalMeshComponent> FirstPersonMesh;
		/** gun muzzle's offset from the camera location */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
			FVector MuzzleOffset;
		/** projectile class to spawn */
		UPROPERTY(EditDefaultsOnly, Category = Projectile)
			TSubclassOf<class AFPSProjectile> ProjectileClass;
		/** arm distance */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
			float PlayerInteractionDistance;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
			bool BlockGrabbed;
		
		ABlockState* Pickup;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
			TSubobjectPtr<UPhysicsHandleComponent> PhysicsHandleComponent;

	protected:
		virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) OVERRIDE;

		bool DoTrace(FHitResult* RV_Hit, FCollisionQueryParams* RV_TraceParams);

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

	private:
};