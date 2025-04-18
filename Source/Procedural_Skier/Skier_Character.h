// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Skier_Character.generated.h"

UCLASS()
class PROCEDURAL_SKIER_API ASkier_Character : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASkier_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void AddForwardForce();
	void AddBreakForce(float FrameDeltaTime);

	UPROPERTY(BlueprintReadOnly, Category = Player_State)
	bool Pushing = false;
	
private:
	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* Capsule;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* SkeletalMesh;

	float YawInput = 90;
	float PitchInput = 0;
	float TurnInput = 0;

	bool BreakingInput = false;
	
	bool GroundCheck(FVector PlayerLocation);
	
	void Look(float InputValue);
	void Camera_Turn(float InputValue);
	void TurnPlayer(float InputValue);
	
	void StartMovement();
	void EndMovement();

	void StartBreak();
	void EndBreak();

protected:
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* MoveForwardAction;
	
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* BreakAction;


	UPROPERTY(EditAnywhere, Category = Movement)
	float Push_Force = 200.0f;
	UPROPERTY(EditAnywhere, Category = Movement)
	float MaxPushSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float MaxTurnSpeed = 400.0f;
	
	UPROPERTY(EditAnywhere, Category = Movement)
	float MinTurnVelocity = 100.0f;

	UPROPERTY(EditAnywhere, Category = Movement)
	UCurveFloat* TurnSpeedCurve;

	UPROPERTY(EditAnywhere, Category = Movement)
	float LeanForce = 10.0f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float GroundCheckDistance = 54.25f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float BreakForce = 100.0f;

	// Physics Material For Default & Leaning
	UPROPERTY(EditAnywhere, Category = Physics_Material)
	UPhysicalMaterial* DefaultMaterial;
	UPROPERTY(EditAnywhere, Category = Physics_Material)
	UPhysicalMaterial* LeanMaterial;

	UPROPERTY(EditAnywhere, Category = Animation)
	float Rotation_Speed = 2.0f;

	UPROPERTY(EditAnywhere, Category = Animation)
	float End_Rotation_Velocity = 25.0f;

	UPROPERTY(BlueprintReadOnly, Category = Player_State)
	bool MovingForward = false;
	UPROPERTY(BlueprintReadOnly, Category = Player_State)
	bool Grounded = false;
	UPROPERTY(BlueprintReadOnly, Category = Player_State)
	bool ForwardInput = false;
	UPROPERTY(BlueprintReadOnly, Category = Player_State)
	bool Breaking = false;
};
