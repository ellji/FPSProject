// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "FPSProject.h"
#include "FPSCharacter.h"
#include "FPSProjectile.h"

AFPSCharacter::AFPSCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// tick!
	PrimaryActorTick.bCanEverTick = true;

	// create a camera component
	FirstPersonCameraComponent = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = CapsuleComponent;
	// position the camera a bit above the eyes
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 50.0f + BaseEyeHeight);

	// create a mesh component that will be used when being viewed from a '1st person' view when controlling this pawn
	FirstPersonMesh = PCIP.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetOnlyOwnerSee(true); // only the player owning this mesh can see this
	FirstPersonMesh->AttachParent = FirstPersonCameraComponent;
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;

	// everybody but the owner can see the regular body mesh
	Mesh->SetOwnerNoSee(true);

	BlockGrabbed = false;

	PhysicsHandleComponent = PCIP.CreateDefaultSubobject<UPhysicsHandleComponent>(this, TEXT("PhysicsHandleComponent"));
	PhysicsHandleComponent->bAutoActivate = true;
	PhysicsHandleComponent->RegisterComponent();

}

void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	InputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);
	InputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);
	
	InputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::OnStartJump);
	InputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::OnStopJump);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::OnFire);

	InputComponent->BindAction("Use", IE_Pressed, this, &AFPSCharacter::OnUse);
}

void AFPSCharacter::Tick(float DeltaTime)
{
	if (BlockGrabbed)
	{
		FVector CameraLoc;
		FRotator CameraRot;
		GetActorEyesViewPoint(CameraLoc, CameraRot);

		FVector Start = CameraLoc;
		FVector End = CameraLoc + (CameraRot.Vector() * PlayerInteractionDistance);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Blue, End.ToString());
		}

		PhysicsHandleComponent->SetTargetLocation(End);
	}
}

void AFPSCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		FRotator Rotation = Controller->GetControlRotation();
		// limit pitch when walking or falling
		if (CharacterMovement->IsMovingOnGround() || CharacterMovement->IsFalling())
		{
			Rotation.Pitch = 0.0f;
		}
		// add movement in that direction
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AFPSCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AFPSCharacter::OnStartJump()
{
	bPressedJump = true;
}

void AFPSCharacter::OnStopJump()
{
	bPressedJump = false;
}

void AFPSCharacter::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		// get the camera transform
		FVector CameraLoc;
		FRotator CameraRot;
		GetActorEyesViewPoint(CameraLoc, CameraRot);
		// muzzle offset is in camera space so transform it to world space before offsetting from the camera to find the final muzzle position
		FVector const MuzzleLocation = CameraLoc + FTransform(CameraRot).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRot;
		MuzzleRotation.Pitch += 10.0f;
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			// spawn the projectile at the muzzle
			AFPSProjectile* const Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// find launch direction
				FVector const LaunchDir = MuzzleRotation.Vector();
				Projectile->InitVelocity(LaunchDir);
			}
		}
	}
}

void AFPSCharacter::OnUse()
{
	if (!BlockGrabbed)
	{
		// re-init hit info, do the trace, and grab the usable item
		FHitResult RV_Hit(ForceInit);
		DoTrace(&RV_Hit);

		Pickup = Cast<ABlockState>(RV_Hit.GetActor());
		if (Pickup) // we actually hit a pickup
		{
			BlockGrabbed = true;

			FVector CameraLoc;
			FRotator CameraRot;
			GetActorEyesViewPoint(CameraLoc, CameraRot);

			FVector Start = CameraLoc;
			FVector End = CameraLoc + (CameraRot.Vector() * PlayerInteractionDistance);
			
			PhysicsHandleComponent->GrabComponent(Pickup->StaticMeshComponent, RV_Hit.BoneName, RV_Hit.Location, true);

			Pickup->OnUsed(this->Controller); // call the interface so the object can do whatever it does when its used
		}
	}
	else
	{
		PhysicsHandleComponent->ReleaseComponent();
		BlockGrabbed = false;
		Pickup = NULL;
	}
}

void AFPSCharacter::DoTrace(FHitResult* RV_Hit)
{
	if (Controller == NULL) // access the controller, make sure we have one
	{
		return;
	}

	// get the camera transform
	FVector CameraLoc;
	FRotator CameraRot;
	GetActorEyesViewPoint(CameraLoc, CameraRot);

	FVector Start = CameraLoc;
	FVector End = CameraLoc + (CameraRot.Vector() * PlayerInteractionDistance);

	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bTraceAsyncScene = true;
	RV_TraceParams.bReturnPhysicalMaterial = true;

	//  do the line trace
	GetWorld()->LineTraceSingle(
		*RV_Hit,		//result
		Start,		//start
		End,		//end
		ECC_Pawn,	//collision channel
		RV_TraceParams
		);
}