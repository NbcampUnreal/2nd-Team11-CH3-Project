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
#include "Item/Weapons/SniperRifle.h"
#include "Item/Weapons/RocketLauncher.h"
#include "Blueprint/UserWidget.h"
#include "MissionStartTrigger.h"
#include "UI/MyHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Item/ConsumableBase.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

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

	StatusContainerComponent->SetMaxHealth(200);
	StatusContainerComponent->SetCurHealth(StatusContainerComponent->GetMaxHealth());

	StatusContainerComponent->SetMaxArmor(100);
	StatusContainerComponent->SetCurArmor(StatusContainerComponent->GetMaxArmor());
	RestoreArmorAmount = 5.0f;

	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void APlayerCharacter::SetConsumable(UConsumableBase* InItem, int32 InSlotNum)
{
	switch (InSlotNum)
	{
	case 0:
		FirstConsumable = InItem;
		break;
	case 1:
		SecondConsumable = InItem;
		break;
	case 2:
		ThirdConsumable = InItem;
		break;
	default:
		break;
	}
}

void APlayerCharacter::GetGunItem(FName GunName)
{
	if (GunName == FName("AssaultRifle"))
	{
		UGunBase* NewGun = NewObject<UGunBase>(this, AssaultRifle);
		NewGun->InitializeItem(AssaultRifle->GetDefaultObject<UGunBase>());
		InventoryComponent->AddItem(NewGun);
	}

	if (GunName == FName("SniperRifle"))
	{
		UGunBase* NewGun = NewObject<UGunBase>(this, SniperRifle);
		NewGun->InitializeItem(SniperRifle->GetDefaultObject<UGunBase>());
		InventoryComponent->AddItem(NewGun);
	}

	if (GunName == FName("Shotgun"))
	{
		UGunBase* NewGun = NewObject<UGunBase>(this, Shotgun);
		NewGun->InitializeItem(Shotgun->GetDefaultObject<UGunBase>());
		InventoryComponent->AddItem(NewGun);
	}

	if (GunName == FName("RocketLauncher"))
	{
		UGunBase* NewGun = NewObject<UGunBase>(this, RocketLauncher);
		NewGun->InitializeItem(RocketLauncher->GetDefaultObject<UGunBase>());
		InventoryComponent->AddItem(NewGun);
	}

	UE_LOG(LogTemp, Warning, TEXT("WRONG GUN NAME"));
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AssaultRifle)
	{
		UGunBase* NewGun = NewObject<UGunBase>(this, AssaultRifle);
		NewGun->InitializeItem(AssaultRifle->GetDefaultObject<UGunBase>());
		EquippedGun = NewGun;
	}

	this->Tags.Add(TEXT("Player"));
	GetWorld()->GetTimerManager().SetTimer(
		ArmorRestoreTimer,
		this,
		&APlayerCharacter::RestoreArmor,
		5.0f,
		true
		);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSprinting)
	{
		// SprintSpeed로 서서히 증가
		float TargetSpeed = SprintSpeed;
		float InterpSpeed = FMath::FInterpTo(GetCharacterMovement()->MaxWalkSpeed, TargetSpeed, DeltaTime, 8.0f); // 8.0f는 보간 속도
		GetCharacterMovement()->MaxWalkSpeed = InterpSpeed;
	}
	else
	{
		// MoveSpeed로 서서히 감소
		float TargetSpeed = MoveSpeed;
		float InterpSpeed = FMath::FInterpTo(GetCharacterMovement()->MaxWalkSpeed, TargetSpeed, DeltaTime, 8.0f); // 8.0f는 보간 속도
		GetCharacterMovement()->MaxWalkSpeed = InterpSpeed;
	}
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
		AMyGameState* MyGameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
		if (MyGameState)
		{
			MyGameState->UpdateCrossHair();
		}
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
		bIsSprinting = true; // 스프린트 상태 시작
	}
}

void APlayerCharacter::StopSprint()
{
	if (GetCharacterMovement())
	{
		bIsSprinting = false; // 스프린트 상태 종료
	}
}

void APlayerCharacter::Fire(const FInputActionValue& value)
{
	if (bIsSprinting) StopSprint();

	if (EquippedGun && bIsReloading == false && EquippedGun->CurAmmo > 0 && EquippedGun->bCanFire && !bIsCrouch && !bIsRolling)
	{
		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
		EquippedGun->Fire();
		GetMesh()->GetAnimInstance()->Montage_Play(FireMontage);
	}
}

void APlayerCharacter::Reload(const FInputActionValue& value)
{
	if (EquippedGun && bIsReloading && bIsRolling == false)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EquippedGun->GetReloadSound(), GetActorLocation());		

		bIsReloading = true;
		GetWorld()->GetTimerManager().SetTimer(
			ReloadTimerHandle,
			this,
			&APlayerCharacter::FinishReload,
			EquippedGun->ReloadTime,
			false);

		GetMesh()->GetAnimInstance()->Montage_Play(ReloadMontage);
	}
}

void APlayerCharacter::FinishReload()
{
	EquippedGun->Reload();

	AMyGameState* MyGameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
	if (MyGameState)
	{
		MyGameState->UpdateCrossHair();
	}

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
		if (SubGun)
		{
			UGunBase* TempGun = EquippedGun;
			EquippedGun = SubGun;
			SubGun = TempGun;
		}

		if (EquippedGun)
		{
			UE_LOG(LogTemp, Warning, TEXT("Equipped Gun: %s"), *EquippedGun->GetName());
		}
		if (SubGun)
		{
			UE_LOG(LogTemp, Warning, TEXT("Sub Gun: %s"), *SubGun->GetName());
		}

		AMyGameState* MyGameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
		if (MyGameState)
		{
			MyGameState->UpdateCrossHair();
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
UGunBase* APlayerCharacter::GetSubGun()
{
	if (SubGun)
	{
		return SubGun;
	}
	return nullptr;
}

void APlayerCharacter::SetEquippedGun(UGunBase* InGun)
{
	EquippedGun = InGun;
}

void APlayerCharacter::SetSubGun(UGunBase* InGun)
{
	SubGun = InGun;
}

UStatusContainerComponent* APlayerCharacter::GetStatusContainerComponent() const
{
	return StatusContainerComponent;
}

void APlayerCharacter::RestoreArmor()
{
	if (StatusContainerComponent->GetCurArmor() > 0)
	{
		StatusContainerComponent->SetCurArmor(StatusContainerComponent->GetCurArmor() + RestoreArmorAmount);
	}
}

void APlayerCharacter::OnDeath()
{
	Super::OnDeath();

	// Handling GameOver After a Delay
	AMyGameState* MyGameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
	if (MyGameState)
	{
		GetWorld()->GetTimerManager().SetTimer(
			GameOverTimerHandle,
			MyGameState,
			&AMyGameState::OnGameOver,
			2.0f,
			false);
	}
}
