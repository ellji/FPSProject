// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "FPSProjectile.generated.h"

/**
 * 
 */
UCLASS()
class AFPSProjectile : public AActor
{
	GENERATED_UCLASS_BODY()

	public: 
		/** sphere collision component */
		UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
			TSubobjectPtr<USphereComponent> CollisionComp;

		/** projectile movement component */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
			TSubobjectPtr<class UProjectileMovementComponent> ProjectileMovement;

		/** inits velocity of the projectile in the shoot direction*/
		void InitVelocity(const FVector& ShootDirection);

		/** called when the projectile hits something */
		UFUNCTION()
			void OnHit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
