// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(Collision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;

	ExplosionDelay = 0.0f;
	ExplosionRadius = 0.0f;
	ExplosionDamage = 0.0f;
}

void AProjectileBase::Explode()
{
	
	DrawDebugSphere(
		GetWorld(),
		GetActorLocation(), // 폭발 중심
		ExplosionRadius,    // 반경
		12,                 // 세그먼트 수 (둥글게 표현할 정도)
		FColor::Red,        // 색상
		false,              // 지속 여부 (true면 계속 표시)
		2.0f                // 지속 시간 (2초 후 사라짐)
	);
	Destroy();
}

void AProjectileBase::InitProjectile(float InitVelocity)
{
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->Velocity = InitVelocity * GetActorForwardVector();
	}
}

void AProjectileBase::SetVelocity(FVector InDirection, float Speed)
{
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->Velocity = InDirection * Speed;
	}
}

void AProjectileBase::SetExplosionDamage(float NewDamage)
{
	ExplosionDamage = NewDamage;
}

float AProjectileBase::GetExplosionDamage() const
{
	return ExplosionDamage;
}
