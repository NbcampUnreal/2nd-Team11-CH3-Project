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
class URocketLauncher;
class UInventoryComponent;
class AMissionStartTrigger;
class UConsumableBase;

UCLASS()
class SPARTADIVERS_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(VisibleAnywhere)
	UConsumableBase* FirstConsumable;
	UPROPERTY(VisibleAnywhere)
	UConsumableBase* SecondConsumable;
	UPROPERTY(VisibleAnywhere)
	UConsumableBase* ThirdConsumable;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UGunBase* EquippedGun;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UGunBase* SubGun;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere)
	UAnimMontage* FireMontage;
	UPROPERTY(EditAnywhere)
	UAnimMontage* ReloadMontage;

	bool bIsReloading = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sate")
	bool bIsOpenInventory = false;
	FTimerHandle ReloadTimerHandle;
	bool bIsSprinting = false;
	FTimerHandle FireTimerHandle;



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
	void StopSprint();
	UFUNCTION()
	void Fire(const FInputActionValue& value);
	UFUNCTION()
	void Reload(const FInputActionValue& value);
	UFUNCTION()
	void OpenIventory(const FInputActionValue& value);
	UFUNCTION()
	void SwapGun(const FInputActionValue& value);
	UFUNCTION()
	void UseOne(const FInputActionValue& value);
	UFUNCTION()
	void UseTwo(const FInputActionValue& value);
	UFUNCTION()
	void UseThree(const FInputActionValue& value);
	UFUNCTION()
	void UseFour(const FInputActionValue& value);
	UFUNCTION()
	void FinishReload();

	float TakeDamage(
		float DamageAmount,
		FDamageEvent const& DamageEvent, 
		AController* EventInstigator, 
		AActor* DamageCauser) override;

	UFUNCTION()
	void Interact(const FInputActionValue& value);

	UFUNCTION()
	UGunBase* GetEquippedGun();

	UFUNCTION()
	UGunBase* GetSubGun();

	void SetEquippedGun(UGunBase* InGun);
	void SetSubGun(UGunBase* InGun);

	UStatusContainerComponent* GetStatusContainerComponent() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AMissionStartTrigger* CurrentMissionTrigger;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UInventoryComponent* InventoryComponent;

	void OnDeath() override;
	FTimerHandle GameOverTimerHandle;
};
