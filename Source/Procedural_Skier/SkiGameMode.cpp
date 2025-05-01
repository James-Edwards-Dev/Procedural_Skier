// Fill out your copyright notice in the Description page of Project Settings.

#include "SkiGameMode.h"

#include "ScoreWidget.h"

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
	ScoreWidget = CreateWidget<UScoreWidget>(world, ScoreWidgetClass);
	ScoreWidget->AddToViewport();
	ScoreWidget->UpdateScore(Score);
}

void ASkiGameMode::AddScore(int32 Amount)
{
	Score += Amount;
	ScoreWidget->UpdateScore(Score);
}

void ASkiGameMode::FinishGame()
{
	GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Green, "Game is over");
}
