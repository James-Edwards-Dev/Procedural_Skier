// Fill out your copyright notice in the Description page of Project Settings.

#include "SkiGameMode.h"

#include "GameWidget.h"
#include "LevelOverWidget.h"
#include "Terrain_Generator.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"

ASkiGameMode::ASkiGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASkiGameMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld* world = GetWorld();

	// Find Terrain Generator
	ATerrain_Generator* Terrain_Generator = nullptr;
	for (TActorIterator<ATerrain_Generator> It(world); It; ++It)
	{
		Terrain_Generator = *It;
		break;
	}

	if (Terrain_Generator)
	{
		// Generate Terrain
		Terrain_Generator->BeginGeneration();

		// Get Suggested Player Location
		FVector SpawnLocation = Terrain_Generator->SuggestPlayerSpawnLocation();

		// Set Player Spawn Location
		FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation);
		APawn* NewPawn = SpawnDefaultPawnAtTransform(GetWorld()->GetFirstPlayerController(), SpawnTransform);

		GetWorld()->GetFirstPlayerController()->Possess(NewPawn);
	}

	world->GetTimerManager().SetTimer(
		GameTimerHandle,
		this,
		&ASkiGameMode::FinishGame,
		GameTimeLimit,
		false);
	
	// Add Score Widget to hud
	GameWidget = CreateWidget<UGameWidget>(world, ScoreWidgetClass);
	GameWidget->AddToViewport();
	GameWidget->UpdateScore(Score);
}

// Called every frame
void ASkiGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	GameWidget->UpdateTimer(GetWorld()->GetTimerManager().GetTimerRemaining(GameTimerHandle));
}

void ASkiGameMode::AddScore(int32 Amount)
{
	Score += Amount;
	GameWidget->UpdateScore(Score);
}

void ASkiGameMode::FinishGame()
{
	UWorld* world = GetWorld();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	
	LevelOverWidget = CreateWidget<ULevelOverWidget>(world, LevelOverWidgetClass);
	LevelOverWidget->AddToViewport();
	LevelOverWidget->UpdateScore(Score);

	PlayerController->bShowMouseCursor = true;

	// Focus on widget and Unlock Mouse
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(LevelOverWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputMode);
	
	PlayerController->SetPause(true);
}
