// Fill out your copyright notice in the Description page of Project Settings.


#include "Skier_Character.h"

#include <string>
#include <ThirdParty/ShaderConductor/ShaderConductor/External/DirectXShaderCompiler/include/dxc/DXIL/DxilConstants.h>

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

class UEnhancedInputLocalPlayerSubsystem;
// Sets default values
ASkier_Character::ASkier_Character()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	Capsule->SetupAttachment(RootComponent);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMesh->SetupAttachment(Capsule);
	
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
	
	
	// Get player Velocity
	FVector player_velocity = Capsule->GetComponentVelocity();
	if (player_velocity.Size() > End_Rotation_Velocity){
		// Player Turning
		Capsule->AddForce(SkeletalMesh->GetForwardVector() * TurnSpeed * 100000.0f * TurnInput * DeltaTime);
		
		player_velocity.Z = 0;
		player_velocity.Normalize();
		
		FRotator target_rotation = player_velocity.Rotation() + FRotator(0, -90, 0); // Calculate Target Rotation
		
		FRotator player_rotation = FMath::Lerp(SkeletalMesh->GetComponentRotation(), target_rotation, DeltaTime * Rotation_Speed); // Lerp Rotation
		SkeletalMesh->SetWorldRotation(player_rotation); // Rotate Player Mesh

		// Rotate Camera
		GetController()->SetControlRotation(FRotator(PitchInput, player_rotation.Yaw + YawInput, 0));
	}
	else
	{
		// Rotate Camera
		GetController()->SetControlRotation(FRotator(PitchInput, SkeletalMesh->GetComponentRotation().Yaw + YawInput, 0));
	}
}

// Called to bind functionality to input
void ASkier_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Camera Bindings
	PlayerInputComponent->BindAxis("Look", this, &ASkier_Character::Look);
	PlayerInputComponent->BindAxis("Turn", this, &ASkier_Character::Camera_Turn);

	PlayerInputComponent->BindAxis("Turn_Player", this, &ASkier_Character::TurnPlayer);

	if(APlayerController* PlayerController = Cast<APlayerController>(Controller)){
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);

			if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
			{
				// Bind Move Forward Action
				Input->BindAction(MoveForwardAction, ETriggerEvent::Started, this, &ASkier_Character::StartMovement);
				Input->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ASkier_Character::EndMovement);
			}
		}
	}
}

void ASkier_Character::Look(const float InputValue)
{
	YawInput += InputValue;
}

void ASkier_Character::Camera_Turn(const float InputValue)
{
	PitchInput -= InputValue;
}

void ASkier_Character::TurnPlayer(float InputValue)
{
	TurnInput = InputValue;
}

void ASkier_Character::StartMovement()
{
	GEngine->AddOnScreenDebugMessage(1, 2, FColor::Green, "Start Movement");
	Capsule->AddImpulse(SkeletalMesh->GetRightVector() * Push_Force * 1000.0f);
}

void ASkier_Character::EndMovement()
{
	GEngine->AddOnScreenDebugMessage(1, 2, FColor::Red, "End Movement");
}
