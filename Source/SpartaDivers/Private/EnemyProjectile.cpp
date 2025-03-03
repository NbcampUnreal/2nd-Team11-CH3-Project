// Copyright Epic Games, Inc. All Rights Reserved.

#include "EnemyProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "PlayerCharacter.h"
#include "Engine/OverlapResult.h" 

AEnemyProjectile::AEnemyProjectile()
{
	ExplosionRadius = 200.0f;
	ExplosionDelay = 2.0f;
	ExplosionDamage = 10.0f;

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyProjectile::OnProjectileOverlap);
}

void AEnemyProjectile::OnProjectileOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (Cast<APlayerCharacter>(OtherActor))
	{


		GetWorld()->GetTimerManager().ClearTimer(EnemyProjectileTimerHandle);
		Explode();
	}
}

void AEnemyProjectile::Explode()
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
			if (HitActor && HitActor->ActorHasTag("Enemy"))
			{
				IgnoreActors.Add(HitActor);
			}
		}
	}

	UGameplayStatics::ApplyRadialDamage(
		this,
		ExplosionDamage,
		GetActorLocation(),
		ExplosionRadius,
		UDamageType::StaticClass(),
		IgnoreActors
	);
	Super::Explode();
}


void AEnemyProjectile::InitProjectile(float InitVelocity)
{
	Super::InitProjectile(InitVelocity);

	GetWorld()->GetTimerManager().SetTimer(
		EnemyProjectileTimerHandle,
		this,
		&AEnemyProjectile::Explode,
		ExplosionDelay);
}
