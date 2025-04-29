// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Landscape.h"

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

	ALandscape* CreateLandscape(int32 QuadsPerComponent, int32 SizeX, int32 SizeY);
	void GenerateCheckpoints(int32 SizeX, int32 SizeY);

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
	float Frequency = 0.035f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	float Amplitude = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	uint8 Octaves = 4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	float Lacunarity = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	float Persistence = 4.0f;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, Category = "Checkpoints")
	uint8 CheckpointCount = 4;
	UPROPERTY(EditAnywhere, Category = "Checkpoints")
	UClass* Checkpoint;
};
