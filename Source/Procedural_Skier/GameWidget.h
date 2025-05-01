// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURAL_SKIER_API UGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category="Score")
	void UpdateScore(int32 NewScore);

	UFUNCTION(BlueprintImplementableEvent, Category="Timer")
	void UpdateTimer(float NewTime);
};
