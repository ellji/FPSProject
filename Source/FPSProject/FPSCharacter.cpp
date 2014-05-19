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

	SelectedInventoryItem = 0;
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

	InputComponent->BindAction("InvLeft", IE_Pressed, this, &AFPSCharacter::InvLeft);
	InputComponent->BindAction("InvRight", IE_Pressed, this, &AFPSCharacter::InvRight);
}

void AFPSCharacter::Tick(float DeltaTime)
{
	// let's grab a line forwards
	FHitResult RV_Hit(ForceInit);
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	if (Pickup)
		RV_TraceParams.AddIgnoredActor(Pickup);

	// get the camera transform
	FVector CameraLoc;
	FRotator CameraRot;
	GetActorEyesViewPoint(CameraLoc, CameraRot);

	FVector Start = CameraLoc;
	FVector End = CameraLoc + (CameraRot.Vector() * PlayerInteractionDistance);
	
	bool DidTrace = DoTrace(Start, End, &RV_Hit, &RV_TraceParams);

	if (!DidTrace)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::Red, "Didn't hit");

		//End = CameraLoc + (CameraRot.Vector() * PlayerInteractionDistance);
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::Green, "Hit");

		End = RV_Hit.Location;
	}

	DrawDebugLine(GetWorld(),
		Start + FVector(0, 0, -10),
		End,
		FColor(255, 0, 0),
		false, -1, 0,
		5.0f);

	DrawDebugSphere(GetWorld(),
		End,
		10.0f,
		16,
		FColor(255, 0, 0));

	if (PhysicsHandleComponent.IsValid())
	{
		FVector PhysHandleLoc = FVector::ZeroVector;
		FRotator PhysHandleRot = FRotator::ZeroRotator;
		PhysicsHandleComponent->GetTargetLocationAndRotation(PhysHandleLoc, PhysHandleRot);
		DrawDebugSphere(GetWorld(),
			PhysHandleLoc,
			20.0f,
			16,
			FColor(0, 255, 0));

		if (GEngine)
		{
			FString Debug = "Handle Off";
			if (PhysicsHandleComponent->IsActive())
				FString Debug = "Handle On";
			
			GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::Magenta, Debug);
		}

		if (BlockGrabbed)
		{
			// see what we're standing on
			StandingOn();

			if (Pickup && Pickup != Standing)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::Green, "BlockGrabbed && Pickup");

				FRotator CurrentRotation = Pickup->GetActorRotation();
				FRotator NewRotation = FRotator(CurrentRotation.Pitch, CameraRot.Yaw, CurrentRotation.Roll);
				
				PhysicsHandleComponent->SetTargetLocation(End);
			}
			else
			{
				// drop the block - because we're already holding the block, calling onuse should drop it
				OnUse();
			}
		}
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
	// get the camera transform
	FVector CameraLoc;
	FRotator CameraRot;
	GetActorEyesViewPoint(CameraLoc, CameraRot);

	FVector Start = CameraLoc;
	FVector End = CameraLoc + (CameraRot.Vector() * PlayerInteractionDistance);

	// re-init hit info, do the trace, and grab the usable item
	FHitResult RV_Hit(ForceInit);
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	DoTrace(Start, End, &RV_Hit, &RV_TraceParams);

	ABlockState* HitBlock = Cast<ABlockState>(RV_Hit.GetActor());

	if (HitBlock && ItemInventory.Num() > 0)
	{
		HitBlock->ReceiveCard(ItemInventory[SelectedInventoryItem].GetValue());
	}
}

void AFPSCharacter::OnUse()
{
	if (!BlockGrabbed)
	{
			// see what we're standing on
		StandingOn();

		if (Standing)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, Standing->GetHumanReadableName());
		}

		// get the camera transform
		FVector CameraLoc;
		FRotator CameraRot;
		GetActorEyesViewPoint(CameraLoc, CameraRot);

		FVector Start = CameraLoc;
		FVector End = CameraLoc + (CameraRot.Vector() * PlayerInteractionDistance);

		// re-init hit info, do the trace, and grab the usable item
		FHitResult RV_Hit(ForceInit);
		FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
		DoTrace(Start, End, &RV_Hit, &RV_TraceParams);

		Pickup = Cast<ABlockState>(RV_Hit.GetActor());
		if (Pickup && Pickup != Standing) // we actually hit a pickup
		{
			BlockGrabbed = true;

			Pickup->SetActorLocation(End, true);
			
			PhysicsHandleComponent->GrabComponent(Pickup->StaticMeshComponent, RV_Hit.BoneName, End, true);
			PhysicsHandleComponent->Activate(true);

			Pickup->OnUsed(this->Controller); // call the interface so the object can do whatever it does when its used

			FBodyInstance* BodyInst = Pickup->StaticMeshComponent->GetBodyInstance();
			BodyInst->SetEnableGravity(false);
		}
	}
	else
	{
		PhysicsHandleComponent->ReleaseComponent();
		PhysicsHandleComponent->Activate(false);
		BlockGrabbed = false;

		FBodyInstance* BodyInst = Pickup->StaticMeshComponent->GetBodyInstance();
		BodyInst->SetEnableGravity(true);

		Pickup = NULL;
	}
}

bool AFPSCharacter::DoTrace(FVector Start, FVector End, FHitResult* RV_Hit, FCollisionQueryParams* RV_TraceParams)
{
	if (Controller == NULL) // access the controller, make sure we have one
	{
		return false;
	}

	RV_TraceParams->bTraceComplex = true;
	RV_TraceParams->bTraceAsyncScene = true;
	RV_TraceParams->bReturnPhysicalMaterial = true;

	//  do the line trace
	bool DidTrace = GetWorld()->LineTraceSingle(
		*RV_Hit,		//result
		Start,		//start
		End,		//end
		ECC_Pawn,	//collision channel
		*RV_TraceParams
		);

	return DidTrace;
}

void AFPSCharacter::StandingOn()
{
	FVector CurrentLocation = this->GetActorLocation();
	FVector CurrentUp = this->GetActorUpVector();

	// re-init hit info, do the trace, and grab the usable item
	FHitResult RV_Hit(ForceInit);
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	DoTrace(CurrentLocation, CurrentLocation - (CurrentUp * 120.0f), &RV_Hit, &RV_TraceParams);

	Standing = Cast<ABlockState>(RV_Hit.GetActor());

	DrawDebugLine(GetWorld(),
		CurrentLocation,
		CurrentLocation - (CurrentUp * 120.0f),
		FColor(0, 0, 255),
		false, -1, 0,
		5.0f);

	DrawDebugSphere(GetWorld(),
		CurrentLocation - (CurrentUp * 120.0f),
		10.0f,
		16,
		FColor(0, 0, 255));
}

void AFPSCharacter::ReceiveHit(
	class UPrimitiveComponent* MyComp,
	class AActor* Other,
	class UPrimitiveComponent* OtherComp,
	bool bSelfMoved,
	FVector HitLocation,
	FVector HitNormal,
	FVector NormalImpulse,
	const FHitResult& Hit
	)
{
	AAbilityCard *HitCard = Cast<AAbilityCard>(Other);

	if (HitCard)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, HitCard->TypeString() );
		}
		
		ItemInventory.Add(HitCard->CardType);
		HitCard->Destroy();
	}
}

void AFPSCharacter::InvLeft()
{
	if (ItemInventory.Num() > 0)
	{
		if (SelectedInventoryItem > 0)
		{
			SelectedInventoryItem--;
		}
		else
		{
			SelectedInventoryItem = ItemInventory.Num() - 1;
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Yellow, TypeString());
		}
	}
}

void AFPSCharacter::InvRight()
{
	if (ItemInventory.Num() > 0)
	{
		if (SelectedInventoryItem < ItemInventory.Num() - 1)
		{
			SelectedInventoryItem++;
		}
		else
		{
			SelectedInventoryItem = 0;
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Yellow, TypeString());
		}
	}
}

FString AFPSCharacter::TypeString()
{
	FString OutputString;

	if (ItemInventory[SelectedInventoryItem].GetValue() == ECardType::Card_Grow)
	{
		OutputString = "Grow";
	}

	if (ItemInventory[SelectedInventoryItem].GetValue() == ECardType::Card_Shrink)
	{
		OutputString = "Shrink";
	}

	return OutputString;
}