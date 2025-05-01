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

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ScoreWidgetClass;
	
private:
	int32 Score = 0;
	
	UPROPERTY()
	UScoreWidget* ScoreWidget;
};
