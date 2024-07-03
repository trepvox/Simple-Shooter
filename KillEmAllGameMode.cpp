// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"


void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
    Super::PawnKilled(PawnKilled);

    //UE_LOG(LogTemp, Warning, TEXT("A pawn was killed"));

    APlayerController*  PlayerController= Cast<APlayerController>(PawnKilled->GetController());
    //if player has died
    if (PlayerController != nullptr)
    {
        //From PlayerController.h GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override
        //PlayerController->GameHasEnded(nullptr, false);
        EndGame(false);
    }

    // For loop over ShotoerAI in the World:
    for (AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld()))
    {
        //Is not dead?
        if (!Controller->IsDead())
        {
            //return
            return;
        }
    
    }
    //end game
    EndGame(true);
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
    //returns a range/list of all controllers in level
    for (AController* Controller : TActorRange<AController>(GetWorld()))
    {
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(),bIsWinner);

        //is the same as above but is simplier above
        /**bool bIsPlayerController = Controller->IsPlayerControlled();
        if (bIsPlayerWinner)
        {
            Controller->GameHasEnded(nullptr, bIsPlayerController);
        }
        else 
        {
            Controller->GameHasEnded(nullptr, !bIsPlayerController);
        }**/
    }
}