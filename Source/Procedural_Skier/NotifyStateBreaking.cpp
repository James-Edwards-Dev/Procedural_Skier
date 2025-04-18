// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyStateBreaking.h"

void UNotifyStateBreaking::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		SkierCharacter = Cast<ASkier_Character>(MeshComp->GetOwner());
		if (SkierCharacter)
		{
			// Add Break Force
			SkierCharacter->AddBreakForce(FrameDeltaTime);
		}
	}
}
