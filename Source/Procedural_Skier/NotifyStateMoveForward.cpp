// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyStateMoveForward.h"

void UNotifyStateMoveForward::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		SkierCharacter = Cast<ASkier_Character>(MeshComp->GetOwner());
		if (SkierCharacter)
		{
			// Player Cannot Transition To Leaning or Idle when pushing
			SkierCharacter->Pushing = true;
			SkierCharacter->AddForwardForce();
		}
	}
}

void UNotifyStateMoveForward::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		SkierCharacter = Cast<ASkier_Character>(MeshComp->GetOwner());
		if (SkierCharacter)
		{
			// Player Cannot Transition To Leaning or Idle when pushing
			SkierCharacter->Pushing = false;
		}
	}
}
