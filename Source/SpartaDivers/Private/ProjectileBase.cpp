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
		GetActorLocation(), // ���� �߽�
		ExplosionRadius,    // �ݰ�
		12,                 // ���׸�Ʈ �� (�ձ۰� ǥ���� ����)
		FColor::Red,        // ����
		false,              // ���� ���� (true�� ��� ǥ��)
		2.0f                // ���� �ð� (2�� �� �����)
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
