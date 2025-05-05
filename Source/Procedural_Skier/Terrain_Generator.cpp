// Fill out your copyright notice in the Description page of Project Settings.


#include "Terrain_Generator.h"

#include "Landscape.h"
#include "LandscapeProxy.h"
#include "LandscapeSplinesComponent.h"
#include "LandscapeSplineSegment.h"
#include "LandscapeSplineControlPoint.h"

// Sets default values
ATerrain_Generator::ATerrain_Generator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

ALandscape* CreateLandscape(UWorld* World, int32 SectionSize, int32 ComponentCountX, int32 ComponentCountY, int32 SectionsPerComponent, float NoiseScale, float HeightMultiplier)
{
	TArray<FLandscapeImportLayerInfo>MaterialImportLayers;
	MaterialImportLayers.Reserve(0);

	TMap<FGuid, TArray<uint16>> HeightDataPerLayers;
	TMap<FGuid, TArray<FLandscapeImportLayerInfo>> MaterialLayerDataPerLayers;

	// Calculate QuadsPerComponent, SizeX and SizeY
	int32 QuadsPerComponent = SectionSize  *  SectionSize;
	int32 SizeX = ComponentCountX * QuadsPerComponent + 1;
	int32 SizeY = ComponentCountY * QuadsPerComponent + 1;

	TArray<uint16> HeightMap;
	HeightMap.SetNum(SizeX * SizeY);
	for (int32 i = 0; i < HeightMap.Num(); i++)
	{
		// Calculate X and Y of index
		const int32 x = i % SizeX;
		const int32 y = i / SizeX;
		
		FVector2D Coordinates = FVector2D(x, y) * NoiseScale;
		float PerlinValue = FMath::PerlinNoise2D(Coordinates) * HeightMultiplier;
		
		HeightMap[i] = 32768 + static_cast<uint16>(PerlinValue);
	}

	HeightDataPerLayers.Add(FGuid(), MoveTemp(HeightMap));
	MaterialLayerDataPerLayers.Add(FGuid(), MoveTemp(MaterialImportLayers));

	ALandscape* Landscape = World->SpawnActor<ALandscape>();

	Landscape->Import(FGuid::NewGuid(), 0, 0, SizeX	-1, SizeY -1 , SectionsPerComponent, QuadsPerComponent, HeightDataPerLayers, nullptr, MaterialLayerDataPerLayers, ELandscapeImportAlphamapType::Additive);

	// Add Landscape Spline Component
	ULandscapeSplinesComponent* SplineComponent = NewObject<ULandscapeSplinesComponent>(Landscape);
	SplineComponent->RegisterComponent();
	Landscape->AddInstanceComponent(SplineComponent);
	Landscape->SetSplinesComponent(SplineComponent);
	
	return Landscape;
}

void AddSpline(ALandscape* Landscape, TArray<FVector>& SplinePoints)
{
	if (!Landscape || !Landscape->GetSplinesComponent())
	{
		return;
	}

	ULandscapeSplinesComponent* SplineComponent = Landscape->GetSplinesComponent();
	ULandscapeSplineControlPoint* PreviousControlPoint = nullptr;

	for (const FVector& Point : SplinePoints)
	{
		ULandscapeSplineControlPoint* ControlPoint = NewObject<ULandscapeSplineControlPoint>(SplineComponent);
		ControlPoint->Location = Point;
		SplineComponent->GetControlPoints().Add(ControlPoint);

		if (PreviousControlPoint)
		{
			ULandscapeSplineSegment* SplineSegment = NewObject<ULandscapeSplineSegment>(SplineComponent);
			SplineSegment->Connections[0].ControlPoint = PreviousControlPoint;
			SplineSegment->Connections[0].TangentLen = 0;
			
			SplineSegment->Connections[1].ControlPoint = ControlPoint;
			SplineSegment->Connections[1].TangentLen = 0;
			
			SplineComponent->GetSegments().Add(SplineSegment);
		}

		PreviousControlPoint = ControlPoint;
	}
	// Update Landscape Function Should Go Here
}

// Called when the game starts or when spawned
void ATerrain_Generator::BeginPlay()
{
	ALandscape* Landscape = CreateLandscape(GetWorld() ,SectionSize, ComponentCountX, ComponentCountY, SectionsPerComponent, NoiseScale, HeightMultiplier);

	if (Landscape)
	{
		TArray<FVector> SplinePoints = {
			FVector(0, 0, 0),
			FVector(500, 200, 120),
			FVector(1000, 500, 90),
		};
		AddSpline(Landscape, SplinePoints);
	}
}

// Called every frame
void ATerrain_Generator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

