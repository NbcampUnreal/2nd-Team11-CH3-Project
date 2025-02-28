// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusContainerComponent.generated.h"

USTRUCT(BlueprintType)
struct FStatusStruct
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	float CurHealth;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPARTADIVERS_API UStatusContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusContainerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	FStatusStruct StatusStruct;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	

	FORCEINLINE float GetCurHealth() const;
	void SetCurHealth(float NewCurHealth);

	FORCEINLINE float GetMaxHealth() const;
	void SetMaxHealth(float NewMaxHealth);
};
