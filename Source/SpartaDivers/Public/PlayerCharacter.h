// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CharacterBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class UGunBase;

UCLASS()
class SPARTADIVERS_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UGunBase* EquippedGun;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;
	UAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere)
	UAnimMontage* FireMontage;
	UPROPERTY(EditAnywhere)
	UAnimMontage* ReloadMontage;

	bool bIsReloading = false;
	FTimerHandle ReloadTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& value);
	
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	
	UFUNCTION()
	void Look(const FInputActionValue& value);
	
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);

	UFUNCTION()
	void Fire(const FInputActionValue& value);
	
	UFUNCTION()
	void Reload(const FInputActionValue& value);

	UFUNCTION()
	void FinishReload();

	UFUNCTION()
	UGunBase* GetEquippedGun();
};
