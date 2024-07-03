// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//BlueprintPure does a little more than BlueprintCallable
	UFUNCTION(BlueprintPure)
	bool IsDead() const;
	
	//This gets the percentage needed to keep track of your health for the healthbar from it's built in scale of 0.00 to 1.0
	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//implemented in Actor.h under public so make sure it's public here too
	//(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	//override at the end to override what is established in the parent files

	void Shoot();


private:

	void MoveForward(float AxisValue);
	void MovingLeftAndRight(float AxisValue);
	//void LookUp(float AxisValue);
	void LookLeftAndRight(float AxisValue);
	void LookUpAndDownRate(float AxisValue);
	void LookLeftAndRightRate(float AxisValue);


	//this is to put a limit on controllers right stick so they can be running the same rate with joysticks consistently
	UPROPERTY(EditAnywhere)
	float RotationRate = 10;
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(EditDefaultsOnly)
	//Will restrict classes to only be of the gun subclass
	TSubclassOf<AGun> GunClass;

	UPROPERTY()
	AGun* Gun;
	
};
