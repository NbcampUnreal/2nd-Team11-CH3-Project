// Copyright Epic Games, Inc. All Rights Reserved.

#include "SDEnemyBase.h"
#include "SDAIController.h"
#include "MyGameInstance.h"
#include "MyGameState.h"
#include "SDLogManager.h"
#include "MissionManager.h"
#include "PlayerCharacter.h"
#include "MyPlayerController.h"
#include "Item/GunBase.h"
#include "DamageTextComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StatusContainerComponent.h"
#include "Item/DropItem.h"
#include "Item/ItemBase.h"

ASDEnemyBase::ASDEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ASDAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	DamageTextComp = CreateDefaultSubobject<UDamageTextComponent>(TEXT("DamageTextComp"));
	DamageTextComp->SetupAttachment(RootComponent);
	DamageTextComp->SetWidgetSpace(EWidgetSpace::World);

	HeadHitbox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadHitbox"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> HeadStaticMeshAsset(TEXT("/Engine/EditorMeshes/EditorSphere.EditorSphere"));
	if (HeadStaticMeshAsset.Succeeded())
	{
		HeadHitbox->SetStaticMesh(HeadStaticMeshAsset.Object);
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> HeadMaterialAsset(TEXT("/Game/_Art/Materials/MasterMaterial/MI_Invisible.MI_Invisible"));
	if (HeadMaterialAsset.Succeeded())
	{
		HeadHitbox->SetMaterial(0, HeadMaterialAsset.Object);
	}
	HeadHitbox->SetupAttachment(GetMesh(), TEXT("head"));

	EnemyType = FName(TEXT("DefaultEnemy"));
	KillScore = 10.0f;
	MoveSpeed = 300.f;
	StatusContainerComponent->SetMaxHealth(100);
	StatusContainerComponent->SetCurHealth(StatusContainerComponent->GetMaxHealth());
}

float ASDEnemyBase::GetHealth() const
{
	return StatusContainerComponent->GetCurHealth();
}

float ASDEnemyBase::GetMaxHealth() const
{
	return StatusContainerComponent->GetMaxHealth();
}

void ASDEnemyBase::AddHealth(float Amount)
{
	StatusContainerComponent->SetCurHealth(StatusContainerComponent->GetCurHealth() + Amount);
}

FName ASDEnemyBase::GetEnemyType()
{
	return EnemyType;
}

float ASDEnemyBase::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("DamageEvent TypeID: %d"), DamageEvent.GetTypeID());
	FVector HitLocation = GetActorLocation();

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		if (PointDamageEvent)
		{
			HitLocation = PointDamageEvent->HitInfo.ImpactPoint;
		}
	}

	if (DamageTextComp)
	{
		DamageTextComp->ShowDamageText(ActualDamage, HitLocation);
	}
	return ActualDamage;
}

void ASDEnemyBase::Attack(int32 SkillIndex)
{
}

void ASDEnemyBase::OnDeath()
{
	OnDropItem();

	AddToLogManager();

	if (AMyGameState* MyGameState = GetWorld()->GetGameState<AMyGameState>())
	{
		if (MyGameState)
		{
			MyGameState->AddScore(KillScore);
		}
	}
  
	Super::OnDeath();

	AMissionManager* MissionManager = Cast<AMissionManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AMissionManager::StaticClass()));
	if (MissionManager && MissionManager->CurrentMissionData.MissionType == EMissionType::Eliminate)
	{
		MissionManager->KilledEnemyCount++;
		UE_LOG(LogTemp, Warning, TEXT("KilledEnemyCount : %d"), MissionManager->KilledEnemyCount);
		MissionManager->CheckMissionCompletion();
	}

	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->UnPossess();
	}
}

void ASDEnemyBase::AddToLogManager()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn()))
		{
			FString VictimName = GetEnemyType().ToString();
			FString WeaponName = TEXT("Unknown Weapon");
			if (PlayerCharacter && PlayerCharacter->GetEquippedGun())
			{
				WeaponName = PlayerCharacter->GetEquippedGun()->GetItemName().ToString();
			}
			bool bHeadshot = PlayerCharacter->GetEquippedGun() && PlayerCharacter->GetEquippedGun()->bHitHead;

			if (USDLogManager* LogManager = USDLogManager::Get())
			{
				UE_LOG(LogTemp, Warning, TEXT("AddToLogManager Called"));

				LogManager->AddKillLog(VictimName, WeaponName, bHeadshot);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to Get LogManagers!"));

			}
		}
	}
}


void ASDEnemyBase::OnDropItem()
{
	float CumDropWeight = 0.f;

	if (DropItemInfos.IsEmpty()) return;

	for (FDropItemInfo dropItemInfo : DropItemInfos)
	{
		CumDropWeight += dropItemInfo.dropWeight;
	}

	float RandomWeight = FMath::FRandRange(0.f, CumDropWeight);
	for (FDropItemInfo dropItemInfo : DropItemInfos)
	{
		RandomWeight -= dropItemInfo.dropWeight;
		if (RandomWeight < 0.f)
		{
			if (dropItemInfo.dropItemClass && DropItem)
			{
				UE_LOG(LogTemp, Warning, TEXT("Spawn"));
				if (ADropItem* DropItemInstance = GetWorld()->SpawnActor<ADropItem>(DropItem))
				{
					DropItemInstance->OwningItemClass = dropItemInfo.dropItemClass;

					FVector Start = GetActorLocation();
					FVector End = Start - FVector::ZAxisVector * 1000.f;

					FHitResult HitResult;
					FCollisionQueryParams QueryParams;
					QueryParams.AddIgnoredActor(this); // Player characters ignore conflict

					bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldStatic, QueryParams);

					if (bHit)
					{
						DropItemInstance->SetActorLocation(HitResult.Location);
					}
				}
			}

			break;
		}
	}
}

void ASDEnemyBase::ApplyAttackEffect(int32 EffectIndex)
{
}

void ASDEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	this->Tags.Add(TEXT("Enemy"));
}

void ASDEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}