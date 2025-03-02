// Copyright Epic Games, Inc. All Rights Reserved.

#include "SDEnemyBase.h"
#include "SDAIController.h"
#include "MissionManager.h"
#include "DamageTextComponent.h"
#include "Engine/DamageEvents.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
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

	EnemyType = FName(TEXT("DefaultEnemy"));
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

FName ASDEnemyBase::GetEnemyType() const
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
			UE_LOG(LogTemp, Warning, TEXT("Point Damage Hit Location: %s"), *HitLocation.ToString());
		}
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		const FRadialDamageEvent* RadialDamageEvent = static_cast<const FRadialDamageEvent*>(&DamageEvent);
		if (RadialDamageEvent && RadialDamageEvent->ComponentHits.Num() > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("RadialDamageEventCalled!!"));
			HitLocation = RadialDamageEvent->ComponentHits[0].ImpactPoint;
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
				ADropItem* DropItemInstance = GetWorld()->SpawnActor<ADropItem>(DropItem);
				if (DropItemInstance)
				{
					DropItemInstance->OwningItemClass = dropItemInfo.dropItemClass;
					DropItemInstance->SetActorLocation(GetActorLocation());
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