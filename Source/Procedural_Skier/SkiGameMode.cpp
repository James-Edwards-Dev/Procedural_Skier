// Fill out your copyright notice in the Description page of Project Settings.

#include "SkiGameMode.h"

#include "GameWidget.h"
#include "Kismet/GameplayStatics.h"

ASkiGameMode::ASkiGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASkiGameMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld* world = GetWorld();

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
	GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Green, "Game is over");
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}
