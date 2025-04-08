// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Skier.generated.h"

UCLASS()
class PROCEDURAL_SKIER_API ASkier : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASkier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	void Look(float InputValue);
	void Turn(float InputValue);
	void StartMovement();
	void EndMovement();

protected:
	
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* MoveForwardAction;

	UPROPERTY(EditAnywhere, Category = Movement)
	float Push_Force = 100.0f;
};
