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

ALandscape* CreateLandscape(FTransform& LandscapeTransform, UWorld* World, int32 SectionSize, int32 ComponentCountX, int32 ComponentCountY, int32 SectionsPerComponent)
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
	Landscape->bCanHaveLayersContent = false;
	//Landscape->LandscapeMaterial = nullptr;

	//Landscape->SetActorTransform(LandscapeTransform);

	Landscape->Import(FGuid::NewGuid(), 0, 0, SizeX	-1, SizeY -1 , SectionsPerComponent, QuadsPerComponent, HeightDataPerLayers, nullptr, MaterialLayerDataPerLayers, ELandscapeImportAlphamapType::Additive);

	Landscape->StaticLightingLOD = FMath::DivideAndRoundUp(FMath::CeilLogTwo(SizeX * SizeY) / (2028 * 2028) + 1, (uint32)2);

	ULandscapeInfo* LandscapeInfo = Landscape->GetLandscapeInfo();
	LandscapeInfo->UpdateLayerInfoMap(Landscape);
	Landscape->RegisterAllComponents();

	FPropertyChangedEvent MaterialPropertyChangedEvent(FindFieldChecked< FProperty >(Landscape->GetClass(), FName("LandscapeMaterial")));
	Landscape->PostEditChangeProperty(MaterialPropertyChangedEvent);
	Landscape->PostEditChange();
	
	return Landscape;
}

void AddSegment(ULandscapeSplineControlPoint* Start, ULandscapeSplineControlPoint* End)
{
	if (Start == End) return;

	ULandscapeSplinesComponent* SplinesComponent = Start->GetOuterULandscapeSplinesComponent();
	SplinesComponent->Modify();
	Start->Modify();
	End->Modify();

	ULandscapeSplineSegment* NewSegment = NewObject<ULandscapeSplineSegment>(SplinesComponent, NAME_None, RF_Transactional);
	TArray<TObjectPtr<ULandscapeSplineSegment>>& Segments = SplinesComponent->GetSegments();
	Segments.Add(NewSegment);

	NewSegment->Connections[0].ControlPoint = Start;
	NewSegment->Connections[1].ControlPoint = End;

	NewSegment->Connections[0].SocketName = Start->GetBestConnectionTo(End->Location);
	NewSegment->Connections[1].SocketName = End->GetBestConnectionTo(Start->Location);

	FVector StartLocation; FRotator StartRotation;
	Start->GetConnectionLocationAndRotation(NewSegment->Connections[0].SocketName, StartLocation, StartRotation);
	FVector EndLocation; FRotator EndRotation;
	End->GetConnectionLocationAndRotation(NewSegment->Connections[0].SocketName, EndLocation, EndRotation);

	NewSegment->Connections[0].TangentLen = static_cast<float>((EndLocation - StartLocation).Size());
	NewSegment->Connections[1].TangentLen = NewSegment->Connections[0].TangentLen;

	NewSegment->AutoFlipTangents();

	ULandscapeSplineSegment* CopyFromSegment = nullptr;
	if (Start->ConnectedSegments.Num() > 0)
	{
		CopyFromSegment = Start->ConnectedSegments[0].Segment;
	}
	else if (End->ConnectedSegments.Num() > 0)
	{
		CopyFromSegment = End->ConnectedSegments[0].Segment;
	}
	else
	{
		
	}

	if (CopyFromSegment != nullptr)
	{
		NewSegment->LayerName = CopyFromSegment->LayerName;
		NewSegment->SplineMeshes = CopyFromSegment->SplineMeshes;
		NewSegment->LDMaxDrawDistance = CopyFromSegment->LDMaxDrawDistance;
		NewSegment->bRaiseTerrain = CopyFromSegment->bRaiseTerrain;
		NewSegment->bLowerTerrain = CopyFromSegment->bLowerTerrain;
		NewSegment->bPlaceSplineMeshesInStreamingLevels = CopyFromSegment->bPlaceSplineMeshesInStreamingLevels;
		NewSegment->BodyInstance = CopyFromSegment->BodyInstance;
		NewSegment->bCastShadow = CopyFromSegment->bCastShadow;
		NewSegment->TranslucencySortPriority = CopyFromSegment->TranslucencySortPriority;
		NewSegment->RuntimeVirtualTextures = CopyFromSegment->RuntimeVirtualTextures;
		NewSegment->VirtualTextureLodBias = CopyFromSegment->VirtualTextureLodBias;
		NewSegment->VirtualTextureCullMips = CopyFromSegment->VirtualTextureCullMips;
		NewSegment->VirtualTextureRenderPassType = CopyFromSegment->VirtualTextureRenderPassType;
		NewSegment->bRenderCustomDepth = CopyFromSegment->bRenderCustomDepth;
		NewSegment->CustomDepthStencilWriteMask = CopyFromSegment->CustomDepthStencilWriteMask;
		NewSegment->CustomDepthStencilValue = CopyFromSegment->CustomDepthStencilValue;
	}

	Start->ConnectedSegments.Add(FLandscapeSplineConnection(NewSegment, 0));
	End->ConnectedSegments.Add(FLandscapeSplineConnection(NewSegment, 1));
	
	NewSegment->UpdateSplinePoints();
}

void AddControlPoint(ALandscape* Landscape, ULandscapeSplinesComponent* SplinesComponent, const FVector& LocalLocation)
{
	DrawDebugSphere(Landscape->GetWorld(), LocalLocation, 50.0f, 12, FColor::Red, false, 10.0f);
	SplinesComponent->Modify();
	
	ULandscapeSplineControlPoint* NewControlPoint = NewObject<ULandscapeSplineControlPoint>(SplinesComponent, NAME_None, RF_Transactional); // Create New Control Point
	TArray<TObjectPtr<ULandscapeSplineControlPoint>>& ControlPoints = SplinesComponent->GetControlPoints(); // Get Control Point

	int32 ControlPointCount = ControlPoints.Num();

	ControlPoints.Add(NewControlPoint);

	NewControlPoint->Location = LocalLocation;

	if (ControlPointCount > 0)
	{
		ULandscapeSplineControlPoint* PreviousPoint = ControlPoints[ControlPointCount - 1];
		FVector NewSegmentDirection = (NewControlPoint->Location - PreviousPoint->Location) * (PreviousPoint->ConnectedSegments.Num() == 0 || PreviousPoint->ConnectedSegments[0].End ? 1.0f : -1.0f);
		NewControlPoint->Rotation = NewSegmentDirection.Rotation();

		NewControlPoint->Width = PreviousPoint->Width;
		NewControlPoint->LayerWidthRatio = PreviousPoint->LayerWidthRatio;
		NewControlPoint->SideFalloff = PreviousPoint->SideFalloff;
		NewControlPoint->LeftSideFalloffFactor = PreviousPoint->LeftSideFalloffFactor;
		NewControlPoint->RightSideFalloffFactor = PreviousPoint->RightSideFalloffFactor;
		NewControlPoint->LeftSideLayerFalloffFactor = PreviousPoint->LeftSideLayerFalloffFactor;
		NewControlPoint->RightSideLayerFalloffFactor = PreviousPoint->RightSideLayerFalloffFactor;
		NewControlPoint->EndFalloff = PreviousPoint->EndFalloff;

		AddSegment(PreviousPoint, NewControlPoint);
	}
	else
	{
		NewControlPoint->UpdateSplinePoints();
	}

	if (!SplinesComponent->IsRegistered())
	{
		SplinesComponent->RegisterComponent();
	}
	else
	{
		SplinesComponent->MarkRenderStateDirty();
	}

	if (Landscape && Landscape->GetLandscapeSplinesReservedLayer())
	{
		Landscape->RequestSplineLayerUpdate();
	}
}

// Called when the game starts or when spawned
void ATerrain_Generator::BeginPlay()
{
	// Create Landscape
	UWorld* World = GetWorld();
	ALandscape* Landscape = CreateLandscape(LandscapeTransform, World ,SectionSize, ComponentCountX, ComponentCountY, SectionsPerComponent);
	
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
	splinePoints.Add(FVector(1000 ,1000 ,0));
	splinePoints.Add(FVector(1000, 3000, 100));
	splinePoints.Add(FVector(3000, 3000, 200));
	splinePoints.Add(FVector(3000, 1000, 100));

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

