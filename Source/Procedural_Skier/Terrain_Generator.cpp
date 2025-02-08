// Fill out your copyright notice in the Description page of Project Settings.


#include "Terrain_Generator.h"

#include "Landscape.h"
#include "LandscapeProxy.h"
#include "LandscapeSplinesComponent.h"
#include "LandscapeSplineControlPoint.h"
#include "LandscapeSubsystem.h"

// Sets default values
ATerrain_Generator::ATerrain_Generator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

ALandscape* CreateLandscape(UWorld* World, int32 SectionSize, int32 ComponentCountX, int32 ComponentCountY, int32 SectionsPerComponent)
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
		HeightMap[i] = 32768;
	}

	HeightDataPerLayers.Add(FGuid(), MoveTemp(HeightMap));
	MaterialLayerDataPerLayers.Add(FGuid(), MoveTemp(MaterialImportLayers));

	ALandscape* Landscape = World->SpawnActor<ALandscape>();

	Landscape->Import(FGuid::NewGuid(), 0, 0, SizeX	-1, SizeY -1 , SectionsPerComponent, QuadsPerComponent, HeightDataPerLayers, nullptr, MaterialLayerDataPerLayers, ELandscapeImportAlphamapType::Additive);

	return Landscape;
}

void AddControlPoint(ALandscape* Landscape, ULandscapeSplinesComponent* SplinesComponent, const FVector& LocalLocation)
{
	SplinesComponent->Modify();
	
	ULandscapeSplineControlPoint* NewControlPoint = NewObject<ULandscapeSplineControlPoint>(SplinesComponent, NAME_None, RF_Transient); // Create New Control Point
	TArray<TObjectPtr<ULandscapeSplineControlPoint>>& ControlPoints = SplinesComponent->GetControlPoints(); // Get Control Point

	int32 ControlPointCount = ControlPoints.Num();

	ControlPoints.Add(NewControlPoint);

	NewControlPoint->Location = LocalLocation;

	NewControlPoint->UpdateSplinePoints();
}

// Called when the game starts or when spawned
void ATerrain_Generator::BeginPlay()
{
	// Create Landscape
	UWorld* World = GetWorld();
	ALandscape* Landscape = CreateLandscape(World ,SectionSize, ComponentCountX, ComponentCountY, SectionsPerComponent);
	
	ULandscapeInfo* LandscapeInfo = Landscape->GetLandscapeInfo(); // Get Landscape Info
	
	World->GetSubsystem<ULandscapeSubsystem>()->ChangeGridSize(LandscapeInfo, SectionSize); // Update grid 

	TSet<ULandscapeComponent*> LandscapeComponents;
	LandscapeInfo->GetComponentsInRegion(0, 0, 1, 1, LandscapeComponents);
	
	ULandscapeComponent* LandscapeComponent = *LandscapeComponents.CreateIterator();
	ULandscapeSplinesComponent* LandscapeSplinesComponent = Landscape->GetSplinesComponent();

	if (LandscapeSplinesComponent == nullptr)
	{
		Landscape->CreateSplineComponent();
		LandscapeSplinesComponent = Landscape->GetSplinesComponent();
	}

	// Spline Points
	TArray<FVector> splinePoints;
	splinePoints.Add(FVector(0 ,0 ,0));
	splinePoints.Add(FVector(0, 1000, 0));
	splinePoints.Add(FVector(1000, 1000, 0));
	splinePoints.Add(FVector(1000, 0, 0));

	// Add Spline Points To Landscape
	for (int32 i = 0; i < splinePoints.Num(); i++)
	{
		AddControlPoint(Landscape, LandscapeSplinesComponent, splinePoints[i]);
	}
}

// Called every frame
void ATerrain_Generator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

