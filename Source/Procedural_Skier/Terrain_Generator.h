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
	
	void BeginGeneration();
	
	UFUNCTION(BlueprintCallable)
	void SpawnCheckpoint();

protected:
	// Called when the game starts or when spawned

	void InitializeWorleyPoints(float SizeX, float SizeY);
	float WorleyNoise2D(float X, float Y);
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perlin Noise")
	float Frequency = 0.035f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perlin Noise")
	float PerlinAmplitude = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perlin Noise")
	uint8 Octaves = 4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perlin Noise")
	float Lacunarity = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perlin Noise")
	float Persistence = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Worley Noise")
	float WorleyAmplitude = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Worley Noise")
	int32 PointCount = 10;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, Category = "Checkpoints")
	uint8 CheckpointCount = 4;
	UPROPERTY(EditAnywhere, Category = "Checkpoints")
	float CheckpointSpawnHeight = 10000.f;
	UPROPERTY(EditAnywhere, Category = "Checkpoints")
	UStaticMesh* CheckpointMesh;
	UPROPERTY(EditAnywhere, Category = "Checkpoints")
	float CheckpointScale = 0.15;
	UPROPERTY(EditAnywhere, Category = "Checkpoints")
	float CheckpointTriggerRadius = 2250.f;

private:
	int32 MaxX = 0;
	int32 MaxY = 0;

	TArray<FVector2d> WorleyPoints = {};
};
