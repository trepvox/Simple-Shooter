// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	//Needs to include Gun.h
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	//Adding a child to this via blueprint
					//(FName BoneName, EPhysBodyOp PhyBodyOption)
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	//HideBoneByName lives in SkinnedMeshComponent.h
	//We're hiding the base gun model that is a part of the character skin this way
	//Added a weapon socket onto the existing weapon_r bone in Wraith_Skeleton

				//(USceneComponent* Parent, const FAttachmentTransformRules& AttachmentRules, FName SocketName = NAME_None)
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
//existing Transform rules ARE KeepRelativeTransform, KeepWorldTransform, SnapToTargetNotIncludingScale, and SnapToTargetIncludingScale
	Gun->SetOwner(this);
	//sets the owner of the 
}

bool AShooterCharacter::IsDead() const
{
	
	return Health <= 0;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MovingLeftAndRight"), this, &AShooterCharacter::MovingLeftAndRight);
	
	//Shorter way to implement the pitch look around rather than similar to moveforwards implementation
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookLeftAndRight"), this, &APawn::AddControllerYawInput);
	//for controllers
	PlayerInputComponent->BindAxis(TEXT("LookUpAndDownRate"), this, &AShooterCharacter::LookUpAndDownRate);
	PlayerInputComponent->BindAxis(TEXT("LookLeftAndRightRate"), this, &AShooterCharacter::LookLeftAndRightRate);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);

}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser)
{
	//Because of the override in the header, need to start with a super for this one.
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("Health left %f"), Health);

	if (IsDead())
	{
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//creating end of game systems

	}

	return DamageToApply;
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	//UE_LOG(LogTemp, Warning, TEXT("Before AddMovement"));
	AddMovementInput(GetActorForwardVector() * AxisValue);
	//UE_LOG(LogTemp, Warning, TEXT("After Add Movement %f"), AxisValue);
	//UE_LOG(LogTemp, Warning, TEXT("PhysicsHandle working %s" *LineTraceEnd.ToString()));
}

void AShooterCharacter::MovingLeftAndRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

//controller support locking speed to looking around, should be independent of framerate
void AShooterCharacter::LookUpAndDownRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}
void AShooterCharacter::LookLeftAndRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Shoot()
{
	Gun->PullTrigger();
}

//void AShooterCharacter::LookUp(float AxisValue)
//{
//	AddControllerPitchInput(AxisValue);
//}
