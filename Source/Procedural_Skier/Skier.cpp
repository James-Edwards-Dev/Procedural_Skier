// Fill out your copyright notice in the Description page of Project Settings.


#include "Skier.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
ASkier::ASkier()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring_Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player_Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ASkier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASkier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAxis("Look", this, &ASkier::Look);
	PlayerInputComponent->BindAxis("Turn", this, &ASkier::Turn);

	if(APlayerController* PlayerController = Cast<APlayerController>(Controller)){
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);

			if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
			{
				Input->BindAction(MoveForwardAction, ETriggerEvent::Started, this, &ASkier::StartMovement);
				Input->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ASkier::EndMovement);
			}
		}
	}
}

void ASkier::Look(float InputValue)
{
	AddControllerYawInput(InputValue);
}
void ASkier::Turn(float InputValue)
{
	AddControllerPitchInput(InputValue);
}

void ASkier::StartMovement()
{
	GEngine->AddOnScreenDebugMessage(1, 2, FColor::Green, "Start Movement");
}

void ASkier::EndMovement()
{
	GEngine->AddOnScreenDebugMessage(1, 2, FColor::Red, "End Movement");
}

