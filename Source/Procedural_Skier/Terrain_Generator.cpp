// Fill out your copyright notice in the Description page of Project Settings.


#include "Terrain_Generator.h"

#include "Landscape.h"
#include "LandscapeProxy.h"
#include "Checkpoint.h"
#include "Components/SphereComponent.h"

// Sets default values
ATerrain_Generator::ATerrain_Generator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ATerrain_Generator::SpawnCheckpoint()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Black, "Spawning Checkpoint");
	
	int32 Checkpoint_X = FMath::RandRange(200, MaxX - 200);
	int32 Checkpoint_Y = FMath::RandRange(200, MaxY - 200);

	// Use Raycast to get position of ground at position
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(
		Hit,
		FVector(Checkpoint_X, Checkpoint_Y, CheckpointSpawnHeight),
		FVector(Checkpoint_X, Checkpoint_Y, 0),
		ECC_WorldStatic);

	// Randomize Flag Yaw
	FRotator CheckpointRotation = FRotator(0, FMath::RandRange(0.f, 360.f), 0);
	
	ACheckpoint* checkpoint = GetWorld()->SpawnActor<ACheckpoint>(Hit.Location, CheckpointRotation);
	
	checkpoint->Mesh->SetStaticMesh(CheckpointMesh);
	checkpoint->Mesh->SetRelativeScale3D(FVector(CheckpointScale, CheckpointScale,CheckpointScale));
	checkpoint->Trigger->SetSphereRadius(CheckpointTriggerRadius);

	checkpoint->MaxX = MaxX;
	checkpoint->MaxY = MaxY;
	checkpoint->SpawnHeight = CheckpointSpawnHeight;
}

// Called when the game starts or when spawned
void ATerrain_Generator::BeginGeneration()
{
	// Calculate QuadsPerComponent, SizeX and SizeY
	int32 QuadsPerComponent = SectionSize  *  SectionSize;
	int32 SizeX = ComponentCountX * QuadsPerComponent + 1;
	int32 SizeY = ComponentCountY * QuadsPerComponent + 1;
	
	InitializeWorleyPoints(SizeX, SizeY);
	
	ALandscape* Landscape = CreateLandscape(QuadsPerComponent, SizeX, SizeY);
	
	GenerateCheckpoints(SizeX, SizeY);
}

FVector ATerrain_Generator::SuggestPlayerSpawnLocation()
{
	return FVector(MaxX/2, MaxY/2, 2000);
}

void ATerrain_Generator::InitializeWorleyPoints(float SizeX, float SizeY)
{
	WorleyPoints.SetNum(PointCount);
	for (FVector2D& Point : WorleyPoints)
	{
		Point = FVector2D(FMath::RandRange(0.f, SizeX), FMath::RandRange(0.f, SizeY));
	}
}

float ATerrain_Generator::WorleyNoise2D(float X, float Y)
{
	float MinDistance = FLT_MAX;
	for (FVector2D& Point : WorleyPoints)
	{
		float distance = FVector2D::Distance(Point, FVector2D(X, Y));
		if (distance < MinDistance)
		{
			MinDistance = distance;
		}
	}

	return MinDistance;
}

ALandscape* ATerrain_Generator::CreateLandscape(int32 QuadsPerComponent, int32 SizeX, int32 SizeY)
{
	TArray<FLandscapeImportLayerInfo>MaterialImportLayers;
	MaterialImportLayers.Reserve(0);

	TMap<FGuid, TArray<uint16>> HeightDataPerLayers;
	TMap<FGuid, TArray<FLandscapeImportLayerInfo>> MaterialLayerDataPerLayers;

	FVector2D PerlinOffset(FMath::RandRange(-10000.f, 10000.f), FMath::RandRange(-10000.f, 10000.f));

	TArray<uint16> HeightMap;
	HeightMap.SetNum(SizeX * SizeY);
	for (int32 i = 0; i < HeightMap.Num(); i++)
	{
		HeightMap[i] = 32768;
			
		// Calculate X and Y of index
		const int32 x = i % SizeX;
		const int32 y = i / SizeX;

		HeightMap[i] += WorleyNoise2D(x, y) * WorleyAmplitude;

		for (uint8 Octave = 0; Octave <= Octaves; Octave++)
		{
			float Octave_Frequency = Frequency;
			float Octave_Amplitude = PerlinAmplitude;
			
			if (Octave > 0)
			{
				Octave_Frequency *= (Octave * Lacunarity);
				Octave_Amplitude /= (Octave * Persistence);
			}
			
			//UE_LOG(LogTemp, Display, TEXT("Octave: %d, Frequency: %f, Amplitude: %f"), Octave, Octave_Frequency, Octave_Amplitude);
			
			FVector2D Coordinates = FVector2D(x, y) * Octave_Frequency;
			float PerlinValue = FMath::PerlinNoise2D(Coordinates + PerlinOffset) * Octave_Amplitude;
			
			//UE_LOG(LogTemp, Display, TEXT("Height Value: %d, Perlin Value: %f"), HeightMap[i], PerlinValue);
			HeightMap[i] += static_cast<uint16>(PerlinValue);
		}
	}

	HeightDataPerLayers.Add(FGuid(), MoveTemp(HeightMap));
	MaterialLayerDataPerLayers.Add(FGuid(), MoveTemp(MaterialImportLayers));

	ALandscape* Landscape = GetWorld()->SpawnActor<ALandscape>();
	Landscape->LandscapeMaterial = Material;
	Landscape->Import(FGuid::NewGuid(), 0, 0, SizeX	-1, SizeY -1 , SectionsPerComponent, QuadsPerComponent, HeightDataPerLayers, nullptr, MaterialLayerDataPerLayers, ELandscapeImportAlphamapType::Additive);

	return Landscape;
}

void ATerrain_Generator::GenerateCheckpoints(int32 SizeX, int32 SizeY)
{
	MaxX = (SizeX - 1) * 128.f;
	MaxY = (SizeY - 1) * 128.f;
	
	for (int8 i = 0; i < CheckpointCount; i++)
	{
		SpawnCheckpoint();
	}
	
	/*// Corners of map
	DrawDebugSphere(GetWorld(), FVector(0, 0, 0), 20, 20, FColor::Green, true);
	DrawDebugSphere(GetWorld(), FVector(0, MaxY, 0), 20, 20, FColor::Red, true);
	DrawDebugSphere(GetWorld(), FVector(MaxX, 0, 0), 20, 20, FColor::Blue, true);
	DrawDebugSphere(GetWorld(), FVector(MaxX, MaxY, 0), 20, 20, FColor::Orange, true);*/
}

// Called every frame
void ATerrain_Generator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

