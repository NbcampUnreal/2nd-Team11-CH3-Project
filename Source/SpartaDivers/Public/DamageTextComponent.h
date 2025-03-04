// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPARTADIVERS_API UDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UDamageTextComponent();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|DamageText")
	TSubclassOf<UUserWidget> DamageTextWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|DamageText")
	UWidgetComponent* DamageTextWidgetComponent;

	FTimerHandle DamageFadeTimerHandle;

	void ShowDamageText(float Damage, FVector HitLocation);

private:
	void HideDamageText();
};
