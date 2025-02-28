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
#include "Item/Weapons/SniperRifle.h"
#include "Item/Weapons/RocketLauncher.h"
#include "Blueprint/UserWidget.h"
#include "MissionStartTrigger.h"
#include "UI/MyHUD.h"
#include "Item/ConsumableBase.h"

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

	StatusContainerComponent->SetMaxHealth(100);
	StatusContainerComponent->SetCurHealth(StatusContainerComponent->GetMaxHealth());

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

	// AssaultRifle 테스트 (기본 착용)
	EquippedGun = NewObject<UAssaultRifle>(this, UAssaultRifle::StaticClass());
	SubGun = NewObject<USniperRifle>(this, USniperRifle::StaticClass());

	//// 로켓런처 테스트
	//UClass* RocketLauncherBPClass = LoadClass<URocketLauncher>(this, TEXT("/Game/_Blueprint/Player/BP_RocketLauncher.BP_RocketLauncher_C"));
	//if (RocketLauncherBPClass)
	//{
	//	EquippedGun = NewObject<URocketLauncher>(this, RocketLauncherBPClass);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Failed to load BP_RocketLauncher!"));
	//}

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
	if (GetCharacterMovement() && bIsReloading == false)
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
	if (EquippedGun && bIsReloading == false && EquippedGun->CurAmmo > 0 && EquippedGun->bCanFire)
	{
		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
		EquippedGun->Fire();
		GetMesh()->GetAnimInstance()->Montage_Play(FireMontage);
	}
}

void APlayerCharacter::Reload(const FInputActionValue& value)
{
	if (EquippedGun && bIsReloading == false)
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
}
void APlayerCharacter::OpenIventory(const FInputActionValue& value)
{
	AMyHUD* MyHUD = Cast<AMyHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (MyHUD == nullptr) return;

	MyHUD->ToggleMainMenu();
}

void APlayerCharacter::SwapGun(const FInputActionValue& value)
{
	if (bIsReloading == false)
	{
		UGunBase* TempGun = EquippedGun;
		EquippedGun = SubGun;
		SubGun = TempGun;

		if (EquippedGun)
		{
			UE_LOG(LogTemp, Warning, TEXT("Equipped Gun: %s"), *EquippedGun->GetName());
		}
		if (SubGun)
		{
			UE_LOG(LogTemp, Warning, TEXT("Sub Gun: %s"), *SubGun->GetName());
		}
	}
}

void APlayerCharacter::UseOne(const FInputActionValue& value)
{
	if (FirstConsumable)
	{
		FirstConsumable->ApplyConsumableEffect(this);
		FirstConsumable = nullptr;
	}
}
void APlayerCharacter::UseTwo(const FInputActionValue& value)
{
	if (SecondConsumable)
	{
		SecondConsumable->ApplyConsumableEffect(this);
		SecondConsumable = nullptr;
	}
}
void APlayerCharacter::UseThree(const FInputActionValue& value)
{
	if (ThirdConsumable)
	{
		ThirdConsumable->ApplyConsumableEffect(this);
		ThirdConsumable = nullptr;
	}
}
void APlayerCharacter::UseFour(const FInputActionValue& value)
{

}

float APlayerCharacter::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser) 
{
	const float ActualDamage = Super::TakeDamage(
		DamageAmount,
		DamageEvent,
		EventInstigator,
		DamageCauser);

	return ActualDamage;
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
UGunBase* APlayerCharacter::GetSubGun()
{
	if (SubGun)
	{
		return SubGun;
	}
	return nullptr;
}
  
UStatusContainerComponent* APlayerCharacter::GetStatusContainerComponent() const
{
	return StatusContainerComponent;
}
