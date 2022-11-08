// Fill out your copyright notice in the Description page of Project Settings.


#include "Casing.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
ACasing::ACasing() : 
	ShellDistroyTime(2.f)
{
	PrimaryActorTick.bCanEverTick = false;

	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CasingMesh"));
	SetRootComponent(CasingMesh);
	CasingMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	CasingMesh->SetSimulatePhysics(true);
	CasingMesh->SetEnableGravity(true);
	CasingMesh->SetNotifyRigidBodyCollision(true);
	ShellEjectionImpulse = 10.f;
}

void ACasing::BeginPlay()
{
	Super::BeginPlay();
	
	CasingMesh->OnComponentHit.AddDynamic(this, &ACasing::OnHit);

	float RandomRotation = FMath::RandRange(-30.f, 30.f);
	FRotator Rotation = FRotator(0.f, RandomRotation, 0.f);
	FQuat QuatRotation = FQuat(Rotation);
	AddActorLocalRotation(QuatRotation);
	CasingMesh->AddImpulse(GetActorForwardVector() * ShellEjectionImpulse);
}

void ACasing::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpluse, const FHitResult & Hit)
{
	if (ShellSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShellSound, GetActorLocation());
	}

	GetWorld()->GetTimerManager().SetTimer(ShellDistroyHandle, this, &ACasing::DestroyCasing, ShellDistroyTime);
	CasingMesh->SetNotifyRigidBodyCollision(false);
}

void ACasing::DestroyCasing()
{
	Destroy();
}