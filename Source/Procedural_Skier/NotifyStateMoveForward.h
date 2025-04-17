// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skier_Character.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NotifyStateMoveForward.generated.h"

class ASkier_Character;

/**
 * 
 */
UCLASS()
class PROCEDURAL_SKIER_API UNotifyStateMoveForward : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	UPROPERTY()
	ASkier_Character* SkierCharacter;
	
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	
};
