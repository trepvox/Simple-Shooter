// Fill out your copyright notice in the Description page of Project Settings.
//entirely written out by Myles Moriarty

#include "ShooterAIController.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();

    if (AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior);

        APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

        //GetPawn inside of Controller.h
        
        //found in BlackboardComponent.h
        GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
    }



}

void AShooterAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

// ----Being replaced by Behavior Tree Functionality----
    //Need to get playerPawn again in this fuction
    

    //All of this has been worked into the Behavior tree and isn't needed hear anymore.
    /**APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (LineOfSightTo(PlayerPawn))
    {
        // Setting playerLocation
        GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
        //UE_LOG(LogTemp, Warning, TEXT("PlayerLocation %f"), PlayerPawn->GetActorLocation());
        GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
        // Setting LastKnownLocation
    }
    else
    {
        //clear playerLocation
        //ClearValue from BlackboardComponent.h
        GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
    }**/

}

bool AShooterAIController::IsDead() const
{
    AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
    if (ControlledCharacter != nullptr)
    {
        return ControlledCharacter->IsDead();
    }

    return true;
}