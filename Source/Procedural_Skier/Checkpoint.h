// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

UCLASS()
class PROCEDURAL_SKIER_API ACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

protected:

	void GenerateNewCheckpoint();

public:
	
	UPROPERTY(EditAnywhere)
	class USphereComponent* Trigger;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	int32 MaxX;
	int32 MaxY;

	int32 SpawnHeight;
};
