// Copyright Epic Games, Inc. All Rights Reserved.

#include "MissionStartTrigger.h"
#include "MissionManager.h"
#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"

AMissionStartTrigger::AMissionStartTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetGenerateOverlapEvents(true);

	// UI 위젯 설정 
	/*InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidget->SetupAttachment(RootComponent);
	InteractionWidget->SetVisibility(false);*/

	// 오버랩 이벤트 바인딩
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMissionStartTrigger::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AMissionStartTrigger::OnOverlapEnd);

	bIsActive = false;
}

void AMissionStartTrigger::BeginPlay()
{
	Super::BeginPlay();

	MissionManager = Cast<AMissionManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AMissionManager::StaticClass()));
}

void AMissionStartTrigger::ActivateTrigger()
{
	bIsActive = true;

	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void AMissionStartTrigger::DeactivateTrigger()
{
	bIsActive = false;

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void AMissionStartTrigger::OnInteracted()
{
	if (bIsActive && MissionManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("Interacted!!"));
		GetWorld()->GetTimerManager().SetTimer(
			MissionStartTimerHandle,
			MissionManager,
			&AMissionManager::StartMission,
			0.5f,
			false);

		OnInteracted_BP();
	}
}

void AMissionStartTrigger::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (bIsActive && InteractionWidget)
	{
	}
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{

		Player->CurrentMissionTrigger = this;
		//InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMissionStartTrigger::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (InteractionWidget)
	{
	}
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player && Player->CurrentMissionTrigger == this)
	{
		Player->CurrentMissionTrigger = nullptr;
		//InteractionWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}