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

void UDamageTextComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!DamageTextWidgetComponent || !GetWorld()) return;

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;

	FVector CameraLocation;
	FRotator CameraRotation;
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// 카메라와 데미지 텍스트 사이 거리 계산
	float Distance = FVector::Dist(CameraLocation, DamageTextWidgetComponent->GetComponentLocation());

	// 크기 조절 (1000cm 거리에서 1배 크기 유지)
	float BaseDistance = 1000.0f;
	float ScaleFactor = Distance / BaseDistance;

	// 최소 및 최대 크기 제한
	ScaleFactor = FMath::Clamp(ScaleFactor, 0.5f, 2.0f);

	// 크기 적용
	DamageTextWidgetComponent->SetWorldScale3D(FVector(ScaleFactor));

	// 항상 카메라를 바라보도록 설정
	FRotator LookAtRotation = (CameraLocation - DamageTextWidgetComponent->GetComponentLocation()).Rotation();
	DamageTextWidgetComponent->SetWorldRotation(LookAtRotation);
}


void UDamageTextComponent::ShowDamageText(float Damage, FVector HitLocation, FLinearColor TextColor)
{
	if (!DamageTextWidgetComponent || !DamageTextWidgetComponent->GetWidget())
	{
		return;
	}

	// 위치 설정 (HitLocation을 기준으로 약간의 높이를 추가)
	DamageTextWidgetComponent->SetWorldLocation(HitLocation);  // Z 값으로 텍스트 높이 조정

	// DamageText 찾기
	if (UTextBlock* DamageText = Cast<UTextBlock>(DamageTextWidgetComponent->GetWidget()->GetWidgetFromName("DamageText")))
	{
		if (Damage <= 0.0f)
		{
			HideDamageText();
			return;
		}
		DamageText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Damage)));

		// RGBA 색상 변경 (TextColor를 사용)
		DamageText->SetColorAndOpacity(FSlateColor(TextColor));
	}

	if (DamageTextWidgetComponent)
	{
		DamageTextWidgetComponent->SetVisibility(true);
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
