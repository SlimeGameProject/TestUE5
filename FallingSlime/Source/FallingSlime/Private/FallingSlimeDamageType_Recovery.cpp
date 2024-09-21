// Fill out your copyright notice in the Description page of Project Settings.

#include "FallingSlimeDamageType_Recovery.h"

#include "Character/FallingSlimeHealthComponent.h"

float UFallingSlimeDamageType_Recovery::TakeRecovery(AActor* RecoveriedActor, float RecoveryAmount, AController* InvestigatedBy, AActor* RecoveryCauser)
{
	if (!RecoveriedActor)
	{
		return 0.f;
	}

	float ActualRecoveryAmount = 0.f;

	if (UFallingSlimeHealthComponent* HealthComp = RecoveriedActor->FindComponentByClass<UFallingSlimeHealthComponent>())
	{
		const float OldHealthPoint = HealthComp->GetHealthPoint();

		HealthComp->SetHealthPoint(OldHealthPoint + RecoveryAmount);

		ActualRecoveryAmount = HealthComp->GetHealthPoint() - OldHealthPoint;
	}

	return ActualRecoveryAmount;
}
