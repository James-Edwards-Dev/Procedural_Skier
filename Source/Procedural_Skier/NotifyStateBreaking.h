// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skier_Character.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NotifyStateBreaking.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURAL_SKIER_API UNotifyStateBreaking : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	ASkier_Character* SkierCharacter;
	
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

};
