// Copyright Epic Games, Inc. All Rights Reserved.

#include "DamageTextComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"

UDamageTextComponent::UDamageTextComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDamageTextComponent::BeginPlay()
{
	Super::BeginPlay();

	// WidgetComponent를 생성하여 UI를 월드 공간에 표시
	if (!DamageTextWidgetComponent)
	{
		DamageTextWidgetComponent = NewObject<UWidgetComponent>(this);
		DamageTextWidgetComponent->RegisterComponent();
		DamageTextWidgetComponent->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageTextWidgetComponent->SetWidgetClass(DamageTextWidgetClass);
		DamageTextWidgetComponent->SetDrawSize(FVector2D(200.f, 100.f));  // 텍스트의 크기를 조정
	}
}

void UDamageTextComponent::ShowDamageText(float Damage, FVector HitLocation)
{
	if (!DamageTextWidgetComponent) return;

	// 위치 설정 (HitLocation을 기준으로 약간의 높이를 추가)
	DamageTextWidgetComponent->SetWorldLocation(HitLocation + FVector(-100, 0, 80));  // Z 값으로 텍스트 높이 조정

	// 위젯을 표시
	DamageTextWidgetComponent->SetVisibility(true);

	// DamageText 찾기
	if (UTextBlock* DamageText = Cast<UTextBlock>(DamageTextWidgetComponent->GetWidget()->GetWidgetFromName("DamageText")))
	{
		if (Damage <= 0.0f)
		{
			HideDamageText();
		}
		DamageText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Damage)));
	}

	// 일정 시간 후 위젯을 숨기기
	GetWorld()->GetTimerManager().SetTimer(
		DamageFadeTimerHandle,
		this, &UDamageTextComponent::HideDamageText,
		1.0f,  // 1초 후 위젯 숨기기
		false);
}

void UDamageTextComponent::HideDamageText()
{
	if (DamageTextWidgetComponent)
	{
		DamageTextWidgetComponent->SetVisibility(false);
	}
}