// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Slash/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Characters/SlashCharacter.h"
#include "NiagaraComponent.h" 

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	EmbersEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	EmbersEffect->SetupAttachment(GetRootComponent());
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant); // TimeConstant = period = 2*pi/K
}

float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant); // TimeConstant = period = 2*pi/K;
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	if (SlashCharacter)
	{
		SlashCharacter->SetOverlappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	if (SlashCharacter)
	{
		SlashCharacter->SetOverlappingItem(nullptr);
	}
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;

	if (ItemState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
		// ItemRotation(); // This was a part of an earlier challenge
	}
}

//void AItem::ItemRotation(float DeltaTime)
//{
//	// on every frame change rotationg for a smooth rotating actor
//	FRotator NewRotation = FRotator((RotationSpeedX * DeltaTime),
//		(RotationSpeedY * DeltaTime),
//		(RotationSpeedZ * DeltaTime));
//
//	FQuat QuatRotation = FQuat(NewRotation);
//
//	AddActorLocalRotation(QuatRotation);
//
//	if (GEngine)
//	{
//		FString MessagePitch = FString::Printf(TEXT("Actor pitch: %f"), GetActorRotation().Pitch);
//		GEngine->AddOnScreenDebugMessage(3, 60.f, FColor::Cyan, MessagePitch);
//
//		FString MessageRoll = FString::Printf(TEXT("Actor roll: %f"), GetActorRotation().Roll);
//		GEngine->AddOnScreenDebugMessage(2, 60.f, FColor::Cyan, MessageRoll);
//
//		FString MessageYaw = FString::Printf(TEXT("Actor pitch: %f"), GetActorRotation().Yaw);
//		GEngine->AddOnScreenDebugMessage(4, 60.f, FColor::Cyan, MessageYaw);
//	}
//}