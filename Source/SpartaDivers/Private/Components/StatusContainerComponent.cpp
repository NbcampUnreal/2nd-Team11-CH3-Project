// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatusContainerComponent.h"

// Sets default values for this component's properties
UStatusContainerComponent::UStatusContainerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UStatusContainerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStatusContainerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UStatusContainerComponent::GetCurHealth() const
{
	return StatusStruct.CurHealth;
}

void UStatusContainerComponent::SetCurHealth(float NewCurHealth)
{
	StatusStruct.CurHealth = NewCurHealth;

	if (StatusStruct.CurHealth < 0.f)
	{
		StatusStruct.CurHealth = 0.f;
	}

	if (GetCurHealth() > GetMaxHealth())
	{
		StatusStruct.CurHealth = GetMaxHealth();
	}
}

float UStatusContainerComponent::GetMaxHealth() const
{
	return StatusStruct.MaxHealth;
}

void UStatusContainerComponent::SetMaxHealth(float NewMaxHealth)
{
	StatusStruct.MaxHealth = NewMaxHealth;

	if (StatusStruct.MaxHealth < 0.f)
	{
		StatusStruct.CurHealth = 0.f;
	}

	if (GetCurHealth() > GetMaxHealth())
	{
		StatusStruct.CurHealth = GetMaxHealth();
	}
}

float UStatusContainerComponent::GetAttackPower() const
{
	return StatusStruct.AttackPower;
}

void UStatusContainerComponent::SetAttackPower(float NewAttackPower)
{
	StatusStruct.AttackPower = NewAttackPower;
}

float UStatusContainerComponent::GetDepensePower() const
{
	return StatusStruct.DefensePower;
}

void UStatusContainerComponent::SetDepensePower(float NewDepensePower)
{
	StatusStruct.DefensePower = NewDepensePower;
}
