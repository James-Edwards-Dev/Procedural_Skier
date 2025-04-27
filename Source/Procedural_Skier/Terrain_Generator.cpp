// Fill out your copyright notice in the Description page of Project Settings.


#include "Terrain_Generator.h"

#include "Landscape.h"
#include "LandscapeProxy.h"

// Sets default values
ATerrain_Generator::ATerrain_Generator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATerrain_Generator::BeginPlay()
{
	// Calculate QuadsPerComponent, SizeX and SizeY
	int32 QuadsPerComponent = SectionSize  *  SectionSize;
	int32 SizeX = ComponentCountX * QuadsPerComponent + 1;
	int32 SizeY = ComponentCountY * QuadsPerComponent + 1;
	
	ALandscape* Landscape = CreateLandscape(QuadsPerComponent, SizeX, SizeY);
	//Landscape->LandscapeMaterial = Material;
	
	GenerateCheckpoints(SizeX, SizeY);
}

ALandscape* ATerrain_Generator::CreateLandscape(int32 QuadsPerComponent, int32 SizeX, int32 SizeY)
{
	TArray<FLandscapeImportLayerInfo>MaterialImportLayers;
	MaterialImportLayers.Reserve(0);

	TMap<FGuid, TArray<uint16>> HeightDataPerLayers;
	TMap<FGuid, TArray<FLandscapeImportLayerInfo>> MaterialLayerDataPerLayers;

	TArray<uint16> HeightMap;
	HeightMap.SetNum(SizeX * SizeY);
	for (int32 i = 0; i < HeightMap.Num(); i++)
	{
		HeightMap[i] = 32768;
			
		// Calculate X and Y of index
		const int32 x = i % SizeX;
		const int32 y = i / SizeX;

		for (uint8 Octave = 0; Octave <= Octaves; Octave++)
		{
			float Octave_Frequency = Frequency;
			float Octave_Amplitude = Amplitude;
			
			if (Octave > 0)
			{
				Octave_Frequency *= (Octave * Lacunarity);
				Octave_Amplitude /= (Octave * Persistence);
			}
			
			UE_LOG(LogTemp, Display, TEXT("Octave: %d, Frequency: %f, Amplitude: %f"), Octave, Octave_Frequency, Octave_Amplitude);
			
			FVector2D Coordinates = FVector2D(x, y) * Octave_Frequency;
			float PerlinValue = FMath::PerlinNoise2D(Coordinates) * Octave_Amplitude;
			
			UE_LOG(LogTemp, Display, TEXT("Height Value: %d, Perlin Value: %f"), HeightMap[i], PerlinValue);
			HeightMap[i] += static_cast<uint16>(PerlinValue);
		}
	}

	HeightDataPerLayers.Add(FGuid(), MoveTemp(HeightMap));
	MaterialLayerDataPerLayers.Add(FGuid(), MoveTemp(MaterialImportLayers));

	ALandscape* Landscape = GetWorld()->SpawnActor<ALandscape>();

	Landscape->Import(FGuid::NewGuid(), 0, 0, SizeX	-1, SizeY -1 , SectionsPerComponent, QuadsPerComponent, HeightDataPerLayers, nullptr, MaterialLayerDataPerLayers, ELandscapeImportAlphamapType::Additive);

	return Landscape;
}

void ATerrain_Generator::GenerateCheckpoints(int32 SizeX, int32 SizeY)
{
	int32 MaxX = (SizeX - 1) * 128.f;
	int32 MaxY = (SizeY - 1) * 128.f;
	
	DrawDebugSphere(GetWorld(), FVector(0, 0, 0), 20, 20, FColor::Green, true);
	DrawDebugSphere(GetWorld(), FVector(0, MaxY, 0), 20, 20, FColor::Red, true);
	DrawDebugSphere(GetWorld(), FVector(MaxX, 0, 0), 20, 20, FColor::Blue, true);
	DrawDebugSphere(GetWorld(), FVector(MaxX, MaxY, 0), 20, 20, FColor::Orange, true);
}

// Called every frame
void ATerrain_Generator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

