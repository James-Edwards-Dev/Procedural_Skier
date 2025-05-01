// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScoreWidget.h"
#include "GameFramework/GameModeBase.h"
#include "SkiGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURAL_SKIER_API ASkiGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	void AddScore(int32 Amount);

private:
	virtual void FinishGame();

protected:
	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float GameTimeLimit = 120.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ScoreWidgetClass;
	
private:
	FTimerHandle GameTimerHandle;
	
	int32 Score = 0;
	
	UPROPERTY()
	UScoreWidget* ScoreWidget;
};
