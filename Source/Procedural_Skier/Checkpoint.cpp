// Fill out your copyright notice in the Description page of Project Settings.


#include "Checkpoint.h"

#include "SkiGameMode.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	
	Trigger = CreateDefaultSubobject<USphereComponent>("Trigger");
	Trigger->SetupAttachment(Mesh);

	OnActorBeginOverlap.AddDynamic(this, &ACheckpoint::OnOverlapBegin);
	
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle InvincibleTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(
	InvincibleTimerHandle,
	this,
	&ACheckpoint::DisableInvincible,
	0.5f,
	false);
	
}

// Called every frame
void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACheckpoint::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (!Invincible)
	{
		ASkiGameMode* Gamemode = Cast<ASkiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
        	Gamemode->AddScore(1);
        	
        	GenerateNewCheckpoint();
        	
        	this->Destroy();
	}
}

void ACheckpoint::GenerateNewCheckpoint()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Black, "Spawning Checkpoint");
	
	int32 Checkpoint_X = FMath::RandRange(200, MaxX);
	int32 Checkpoint_Y = FMath::RandRange(200, MaxY);

	// Use Raycast to get position of ground at position
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(
		Hit,
		FVector(Checkpoint_X, Checkpoint_Y, SpawnHeight),
		FVector(Checkpoint_X, Checkpoint_Y, 0),
		ECC_WorldStatic);
	

	// Randomize Flag Yaw
	FRotator CheckpointRotation = FRotator(0, FMath::RandRange(0.f, 360.f), 0);
	
	ACheckpoint* checkpoint = GetWorld()->SpawnActor<ACheckpoint>(Hit.Location, CheckpointRotation);

	checkpoint->Mesh->SetStaticMesh(Mesh->GetStaticMesh());
	checkpoint->SetActorScale3D(GetActorScale());
	checkpoint->Trigger->SetSphereRadius(Trigger->GetUnscaledSphereRadius());

	checkpoint->MaxX = MaxX;
	checkpoint->MaxY = MaxY;
	checkpoint->SpawnHeight = SpawnHeight;
}

void ACheckpoint::DisableInvincible()
{
	Invincible = false;
}
