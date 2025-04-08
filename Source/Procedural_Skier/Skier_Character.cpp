// Fill out your copyright notice in the Description page of Project Settings.


#include "Skier_Character.h"

#include "NavigationSystemTypes.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ASkier_Character::ASkier_Character()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	Capsule->SetupAttachment(RootComponent);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring_Arm");
	SpringArm->SetupAttachment(Capsule);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ASkier_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkier_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASkier_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Look", this, &ASkier_Character::Look);
	PlayerInputComponent->BindAxis("Turn", this, &ASkier_Character::Turn);
}

void ASkier_Character::Look(const float InputValue)
{
	AddControllerYawInput(InputValue);
}

void ASkier_Character::Turn(const float InputValue)
{
	AddControllerPitchInput(InputValue);
}

