// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyStateMoveForward.h"

void UNotifyStateMoveForward::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "NotifyBegin");

	if (MeshComp && MeshComp->GetOwner())
	{
		SkierCharacter = Cast<ASkier_Character>(MeshComp->GetOwner());
		if (SkierCharacter)
		{
			SkierCharacter->AddForwardForce();
		}
	}
	
}
