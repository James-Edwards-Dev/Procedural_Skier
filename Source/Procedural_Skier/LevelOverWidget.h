// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURAL_SKIER_API ULevelOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category="Score")
	void UpdateScore(int32 NewScore);
};
