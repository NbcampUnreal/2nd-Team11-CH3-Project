// Copyright Epic Games, Inc. All Rights Reserved.

#include "Components/StatusContainerComponent.h"

UStatusContainerComponent::UStatusContainerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


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
		StatusStruct.MaxHealth = 0.f;
	}

	if (GetCurHealth() > GetMaxHealth())
	{
		StatusStruct.CurHealth = GetMaxHealth();
	}
}

float UStatusContainerComponent::GetCurArmor() const
{
	return StatusStruct.CurArmor;
}

void UStatusContainerComponent::SetCurArmor(float NewCurArmor)
{
	StatusStruct.CurArmor= NewCurArmor;

	if (StatusStruct.CurArmor< 0.f)
	{
		StatusStruct.CurArmor= 0.f;
	}

	if (GetCurArmor() > GetMaxArmor())
	{
		StatusStruct.CurArmor= GetMaxArmor();
	}
}

float UStatusContainerComponent::GetMaxArmor() const
{
	return StatusStruct.MaxArmor;
}

void UStatusContainerComponent::SetMaxArmor(float NewMaxArmor)
{
	StatusStruct.MaxArmor= NewMaxArmor;

	if (StatusStruct.MaxArmor< 0.f)
	{
		StatusStruct.CurArmor= 0.f;
	}

	if (GetCurArmor() > GetMaxArmor())
	{
		StatusStruct.CurArmor= GetMaxArmor();
	}
}