// Copyright Epic Games, Inc. All Rights Reserved.

#include "DamageTextComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"

UDamageTextComponent::UDamageTextComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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

void UDamageTextComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!DamageTextWidgetComponent || !GetWorld()) return;

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;

	FVector CameraLocation;
	FRotator CameraRotation;
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// ī�޶�� ������ �ؽ�Ʈ ���� �Ÿ� ���
	float Distance = FVector::Dist(CameraLocation, DamageTextWidgetComponent->GetComponentLocation());

	// ũ�� ���� (1000cm �Ÿ����� 1�� ũ�� ����)
	float BaseDistance = 1000.0f;
	float ScaleFactor = Distance / BaseDistance;

	// �ּ� �� �ִ� ũ�� ����
	ScaleFactor = FMath::Clamp(ScaleFactor, 0.5f, 2.0f);

	// ũ�� ����
	DamageTextWidgetComponent->SetWorldScale3D(FVector(ScaleFactor));

	// �׻� ī�޶� �ٶ󺸵��� ����
	FRotator LookAtRotation = (CameraLocation - DamageTextWidgetComponent->GetComponentLocation()).Rotation();
	DamageTextWidgetComponent->SetWorldRotation(LookAtRotation);
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