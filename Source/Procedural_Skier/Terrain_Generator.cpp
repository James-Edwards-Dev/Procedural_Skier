// Fill out your copyright notice in the Description page of Project Settings.


#include "Terrain_Generator.h"

// Sets default values
ATerrain_Generator::ATerrain_Generator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATerrain_Generator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATerrain_Generator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

