// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Terrain_Generator.generated.h"

UCLASS()
class PROCEDURAL_SKIER_API ATerrain_Generator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATerrain_Generator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Landscape Settings")
	int32 SectionSize = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Landscape Settings")
	int32 SectionsPerComponent = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Landscape Settings")
	int32 ComponentCountX = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Landscape Settings")
	int32 ComponentCountY = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	float Frequency = 0.05f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	float Amplitude = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	uint8 Octaves = 2;
};
