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

	// WidgetComponent�� �����Ͽ� UI�� ���� ������ ǥ��
	if (!DamageTextWidgetComponent)
	{
		DamageTextWidgetComponent = NewObject<UWidgetComponent>(this);
		DamageTextWidgetComponent->RegisterComponent();
		DamageTextWidgetComponent->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageTextWidgetComponent->SetWidgetClass(DamageTextWidgetClass);
		DamageTextWidgetComponent->SetDrawSize(FVector2D(200.f, 100.f));  // �ؽ�Ʈ�� ũ�⸦ ����
	}
}

void UDamageTextComponent::ShowDamageText(float Damage, FVector HitLocation)
{
	if (!DamageTextWidgetComponent) return;

	// ��ġ ���� (HitLocation�� �������� �ణ�� ���̸� �߰�)
	DamageTextWidgetComponent->SetWorldLocation(HitLocation + FVector(-100, 0, 80));  // Z ������ �ؽ�Ʈ ���� ����

	// ������ ǥ��
	DamageTextWidgetComponent->SetVisibility(true);

	// DamageText ã��
	if (UTextBlock* DamageText = Cast<UTextBlock>(DamageTextWidgetComponent->GetWidget()->GetWidgetFromName("DamageText")))
	{
		if (Damage <= 0.0f)
		{
			HideDamageText();
		}
		DamageText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Damage)));
	}

	// ���� �ð� �� ������ �����
	GetWorld()->GetTimerManager().SetTimer(
		DamageFadeTimerHandle,
		this, &UDamageTextComponent::HideDamageText,
		1.0f,  // 1�� �� ���� �����
		false);
}

void UDamageTextComponent::HideDamageText()
{
	if (DamageTextWidgetComponent)
	{
		DamageTextWidgetComponent->SetVisibility(false);
	}
}