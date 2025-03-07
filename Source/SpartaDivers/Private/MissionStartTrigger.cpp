// Copyright Epic Games, Inc. All Rights Reserved.

#include "MissionStartTrigger.h"
#include "MissionManager.h"
#include "PlayerCharacter.h"
#include "MyPlayerController.h"
#include "Components/TextBlock.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"

AMissionStartTrigger::AMissionStartTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetGenerateOverlapEvents(true);

	// 오버랩 이벤트 바인딩
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMissionStartTrigger::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AMissionStartTrigger::OnOverlapEnd);

	bIsActive = false;
	bOverlapTrigger = false;
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
					FVector CaptureLocation = FVector(56753, -51952, -1350);
					PlayerCharacter->SetActorRotation(FRotator(0.f, -190.f, 0.f));
					bool bSuccess = PlayerCharacter->SetActorLocation(CaptureLocation, false); 
				}
			}
		}

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
	bOverlapTrigger = true;
	ShowInteractText();
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{

		Player->CurrentMissionTrigger = this;
	}
}

void AMissionStartTrigger::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	bOverlapTrigger = false;
	ShowInteractText();
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player && Player->CurrentMissionTrigger == this)
	{
		Player->CurrentMissionTrigger = nullptr;
	}
}

void AMissionStartTrigger::ShowInteractText()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			// HudWidget
			if (UUserWidget* HUDWidget = MyPlayerController->GetHUDWidget())
			{
				if (UTextBlock* InteractText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("InteractText"))))
				{
					if (bOverlapTrigger)
					{
						InteractText->SetVisibility(ESlateVisibility::Visible);
					}
					else
					{
						InteractText->SetVisibility(ESlateVisibility::Hidden);
					}
				}
			}
		}
	}
}