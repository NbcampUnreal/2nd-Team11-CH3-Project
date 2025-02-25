// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerCharacter.h"
#include "MyGameState.h"
#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InventoryComponent.h"
#include "Item/GunBase.h"
#include "Item/Weapons/AssaultRifle.h"
#include "MissionStartTrigger.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	MoveSpeed = 600.0f;
	SprintSpeedMultiplier = 1.5f;
	SprintSpeed = MoveSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//EquippedGun = NewObject<UGunBase>(this, UGunBase::StaticClass());
	//if (EquippedGun)
	//{
	//	// ÃÑ±â ÃÊ±âÈ­
	//	EquippedGun->ItemName = FName(TEXT("PlayerGun"));
	//	EquippedGun->ItemDescription = FText::FromString(TEXT("A powerful gun"));
	//	EquippedGun->Damage = 50.0f;
	//	EquippedGun->FireRate = 1.5f;
	//	EquippedGun->MaxAmmo = 30;
	//	EquippedGun->Ammo = 30;
	//}

	EquippedGun = NewObject<UAssaultRifle>(this, UAssaultRifle::StaticClass());
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::Move
				);
			}
			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::StartJump
				);
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&APlayerCharacter::StopJump
				);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::Look
				);
			}
			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::StartSprint
				);
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&APlayerCharacter::StopSprint
				);
			}
			if (PlayerController->FireAction)
			{
				EnhancedInput->BindAction(
					PlayerController->FireAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::Fire
				);
			}
			if (PlayerController->ReloadAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ReloadAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::Reload
				);
			}
			if (PlayerController->InteractAction)
			{
				EnhancedInput->BindAction(
					PlayerController->InteractAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::Interact
				);
			}
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}
void APlayerCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

void APlayerCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void APlayerCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();
	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void APlayerCharacter::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void APlayerCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	}
}

void APlayerCharacter::Fire(const FInputActionValue& value)
{
	if (EquippedGun && bIsReloading == false && EquippedGun->CurAmmo > 0)
	{
		EquippedGun->Fire();
		AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(FireMontage);
	}

	AMyGameState* MyGameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
	if (MyGameState)
	{
		MyGameState->UpdateHUD();
	}
}

void APlayerCharacter::Reload(const FInputActionValue& value)
{
	if (EquippedGun && bIsReloading == false)
	{
		bIsReloading = true;
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &APlayerCharacter::FinishReload, EquippedGun->ReloadTime, false);

		AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(ReloadMontage);
	}
}

void APlayerCharacter::FinishReload()
{
	EquippedGun->Reload();

	bIsReloading = false;

	AMyGameState* MyGameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
	if (MyGameState)
	{
		MyGameState->UpdateHUD();
	}
}

void APlayerCharacter::Interact(const FInputActionValue& value)
{
	if (CurrentMissionTrigger)
	{
		CurrentMissionTrigger->OnInteracted();
	}
}

UGunBase* APlayerCharacter::GetEquippedGun()
{
	if (EquippedGun)
	{
		return EquippedGun;
	}
	return nullptr;
}
