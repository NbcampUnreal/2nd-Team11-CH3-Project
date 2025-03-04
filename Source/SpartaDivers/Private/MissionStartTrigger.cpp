// Copyright Epic Games, Inc. All Rights Reserved.

#include "MissionStartTrigger.h"
#include "MissionManager.h"
#include "PlayerCharacter.h"
#include "MyPlayerController.h"
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
	Destroy();
}

void AMissionStartTrigger::OnInteracted()
{
	UE_LOG(LogTemp, Error, TEXT("OnInteracted is called!"));
	if (bIsActive && MissionManager)
	{
		GetWorld()->GetTimerManager().SetTimer(
			MissionStartTimerHandle,
			MissionManager,
			&AMissionManager::StartMission,
			0.5f,
			false);


		if (MissionManager->CurrentMissionData.MissionType == EMissionType::Eliminate)
		{
			if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
			{
				if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn()))
				{
					FVector CaptureLocation = FVector(54110, -52200, -0);
					bool bSuccess = PlayerCharacter->SetActorLocation(CaptureLocation, false); 
				}
			}
		}

		OnInteracted_BP();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnInteracted Not callef!"));
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