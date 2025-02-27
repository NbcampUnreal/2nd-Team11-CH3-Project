// Copyright Epic Games, Inc. All Rights Reserved.

#include "EnemyProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

AEnemyProjectile::AEnemyProjectile()
{
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
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		GetWorld()->GetTimerManager().ClearTimer(ExplosionTimerHandle);
		UGameplayStatics::ApplyRadialDamage(
			this,
			ExplosionDamage,
			GetActorLocation(),
			ExplosionRadius,
			UDamageType::StaticClass(),
			TArray<AActor*>()
		);
		UE_LOG(LogTemp, Warning, TEXT("Player Get Damaged %f"), ExplosionDamage);
		Explode();
	}
}

void AEnemyProjectile::Explode()
{
	Super::Explode();
}

void AEnemyProjectile::InitProjectile(float InitVelocity)
{
	Super::InitProjectile(InitVelocity);

	GetWorld()->GetTimerManager().SetTimer(
		ExplosionTimerHandle,
		this,
		&AEnemyProjectile::Explode,
		ExplosionDelay);
}
