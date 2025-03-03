// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Consumable/Grenade.h"
#include "PlayerProjectile.h"
#include "PlayerCharacter.h"

void UGrenade::ApplyConsumableEffect(APlayerCharacter* InPlayerCharacter)
{
	if (InPlayerCharacter == nullptr || InPlayerCharacter->GetMesh() == nullptr) return;

	FVector SocketLocation = InPlayerCharacter->GetMesh()->GetSocketLocation(FireSocketName);
	FRotator SocketRotation = InPlayerCharacter->GetMesh()->GetSocketRotation(FireSocketName);
	APlayerProjectile* ProjectileInstance = GetWorld()->SpawnActor<APlayerProjectile>(ProjectileClass, SocketLocation, SocketRotation);

	if (ProjectileInstance)
	{
		ProjectileInstance->InitProjectile(VelocityOfProjectile);
	}
}
