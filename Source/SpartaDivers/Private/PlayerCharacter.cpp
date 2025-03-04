// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerCharacter.h"
#include "MyGameState.h"
#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StatusContainerComponent.h"
#include "Components/InventoryComponent.h"
#include "Item/GunBase.h"
#include "Item/Weapons/AssaultRifle.h"
#include "Animation/AnimMontage.h"
#include "Blueprint/UserWidget.h"
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

	this->Tags.Add(TEXT("Player"));
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
			if (PlayerController->InventoryAction)
			{
				EnhancedInput->BindAction(
					PlayerController->InventoryAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::OpenIventory
				);
			}
			if (PlayerController->SwapAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SwapAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::SwapGun
				);
			}
			if (PlayerController->ButtonOneAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ButtonOneAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::UseOne
				);
			}
			if (PlayerController->ButtonTwoAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ButtonTwoAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::UseTwo
				);
			}
			if (PlayerController->ButtonThreeAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ButtonThreeAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::UseThree
				);
			}
			if (PlayerController->ButtonFourAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ButtonFourAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::UseFour
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
			if (PlayerController->CrouchAction)
			{
				EnhancedInput->BindAction(
					PlayerController->CrouchAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::StartCrouch
				);
			}
			if (PlayerController->CrouchAction)
			{
				EnhancedInput->BindAction(
					PlayerController->CrouchAction,
					ETriggerEvent::Completed,
					this,
					&APlayerCharacter::StopCrouch
				);
			}
			if (PlayerController->RollingAction)
			{
				EnhancedInput->BindAction(
					PlayerController->RollingAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::Rolling
				);
			}
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& value)
{
	if (bIsRolling)
		return;
	if (!Controller) 
		return;

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
	if (value.Get<bool>() && !bIsRolling)
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
	if (GetCharacterMovement() && bIsReloading == false && bIsCrouch == false && !bIsRolling)
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
	if (EquippedGun && bIsReloading == false && EquippedGun->CurAmmo > 0 && !bIsCrouch && !bIsRolling)
	{
		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
		EquippedGun->Fire();
		GetMesh()->GetAnimInstance()->Montage_Play(FireMontage);
	}

	AMyGameState* MyGameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
	if (MyGameState)
	{
		MyGameState->UpdateHUD();
	}
}

void APlayerCharacter::Reload(const FInputActionValue& value)
{
	if (EquippedGun && bIsReloading && bIsRolling == false)
	{
		bIsReloading = true;
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &APlayerCharacter::FinishReload, EquippedGun->ReloadTime, false);

		GetMesh()->GetAnimInstance()->Montage_Play(ReloadMontage);
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
void APlayerCharacter::OpenIventory(const FInputActionValue& value)
{
	if (!bIsOpenInventory)
	{
		bIsOpenInventory = true;
	}
	else
	{
		bIsOpenInventory = false;
	}
}
void APlayerCharacter::SwapGun(const FInputActionValue& value)
{
	UGunBase* temp = EquippedGun;
	EquippedGun = SubGun;
	SubGun = EquippedGun;
}
void APlayerCharacter::UseOne(const FInputActionValue& value)
{

}
void APlayerCharacter::UseTwo(const FInputActionValue& value)
{

}
void APlayerCharacter::UseThree(const FInputActionValue& value)
{

}
void APlayerCharacter::UseFour(const FInputActionValue& value)
{

}
void APlayerCharacter::StartCrouch(const FInputActionValue& value)
{
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	bIsCrouch = true;

	Crouch();
}
void APlayerCharacter::StopCrouch(const FInputActionValue& value)
{
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	bIsCrouch = false;

	UnCrouch();
}

float APlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (StatusContainerComponent->GetCurHealth() <= 0)
	{
		bIsDead = true;
	}

	return DamageAmount;
}

void APlayerCharacter::Interact(const FInputActionValue& value)
{
	if (CurrentMissionTrigger)
	{
		CurrentMissionTrigger->OnInteracted();
	}
}

void APlayerCharacter::Rolling (const FInputActionValue& value)
{
	if (!(GetMesh()->GetAnimInstance()->Montage_IsPlaying(RollingMontage)))
	{
		GetMesh()->GetAnimInstance()->Montage_Play(RollingMontage);
		Crouch();
		bIsRolling = true;
	}

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this,&APlayerCharacter::StopRolling);

	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, RollingMontage);
}

UGunBase* APlayerCharacter::GetEquippedGun()
{
	if (EquippedGun)
	{
		return EquippedGun;
	}
	return nullptr;
}

void APlayerCharacter::StopRolling(UAnimMontage* Montage, bool isEnded)
{
	UnCrouch();
	bIsRolling = false;
}
