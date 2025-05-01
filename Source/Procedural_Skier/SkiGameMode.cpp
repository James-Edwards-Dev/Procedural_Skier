// Fill out your copyright notice in the Description page of Project Settings.

#include "SkiGameMode.h"

#include "ScoreWidget.h"

void ASkiGameMode::BeginPlay()
{
	Super::BeginPlay();

	ScoreWidget = CreateWidget<UScoreWidget>(GetWorld(), ScoreWidgetClass);
	ScoreWidget->AddToViewport();
	ScoreWidget->UpdateScore(Score);
}

void ASkiGameMode::AddScore(int32 Amount)
{
	Score += Amount;
	ScoreWidget->UpdateScore(Score);
}
