// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Engine/OverlapResult.h"

APlayerProjectile::APlayerProjectile()
{
	ExplosionRadius = 300.0f;
	ExplosionDelay = 2.0f;

	Collision->OnComponentBeginOverlap.AddDynamic(this, &APlayerProjectile::OnProjectileOverlap);
}

void APlayerProjectile::OnProjectileOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	{
		if (!PlayerCharacter)
		{
			if (OtherActor)
			{
				GetWorld()->GetTimerManager().ClearTimer(PlayerProjectileTimerHandle);
				Explode();
			}
		}
	}
}

void APlayerProjectile::Explode()
{
	TArray<AActor*> IgnoreActors;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(ExplosionRadius),
		QueryParams
	);

	if (bHit)
	{
		for (const FOverlapResult& Result : OverlapResults)
		{
			AActor* HitActor = Result.GetActor();
			if (HitActor && HitActor->ActorHasTag("Player"))
			{
				IgnoreActors.Add(HitActor);
			}
		}
	}

	UGameplayStatics::ApplyRadialDamage(
		this,
		ExplosionDamage * FMath::FRandRange(0.9f, 1.1f),
		GetActorLocation(),
		ExplosionRadius,
		UDamageType::StaticClass(),
		IgnoreActors
	);
	Super::Explode();
}


void APlayerProjectile::SetVelocity(FVector InDirection, float Speed)
{
	Super::SetVelocity(InDirection, Speed);

	GetWorld()->GetTimerManager().SetTimer(
		PlayerProjectileTimerHandle,
		this,
		&APlayerProjectile::Explode,
		ExplosionDelay);
}

void APlayerProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}
