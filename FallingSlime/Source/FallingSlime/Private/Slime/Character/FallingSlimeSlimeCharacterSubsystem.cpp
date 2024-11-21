// Fill out your copyright notice in the Description page of Project Settings.

#include "Slime/Character/FallingSlimeSlimeCharacterSubsystem.h"

// Include headers in Engine module.
#include "Components/CapsuleComponent.h"

// Include headers in Niagara module.
#include "NiagaraFunctionLibrary.h"

AFallingSlimeSlimeCharacter* UFallingSlimeSlimeCharacterMetamorphosisSubsystem::MetamorphoseSlimeCharacter(AFallingSlimeSlimeCharacter* OriginalSlimeCharacter, const UFallingSlimeSlimeCharacterMetamorphosisData* MetamorphosisData)
{
	if (!OriginalSlimeCharacter)
	{
		return nullptr;
	}

	if (!MetamorphosisData)
	{
		return nullptr;
	}

	AFallingSlimeSlimeCharacter* MetamorphosedSlimeCharacter = nullptr;

	if (UWorld* World = OriginalSlimeCharacter->GetWorld())
	{
		UCapsuleComponent* OriginalCapsuleComp = OriginalSlimeCharacter->GetCapsuleComponent();
		// 衝突して新しいスライムのスポーン位置がずれてしまうため、コリジョンを無効化する
		ECollisionEnabled::Type OriginalCollisionEnabled = OriginalCapsuleComp->GetCollisionEnabled();
		OriginalCapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		TSubclassOf<AFallingSlimeSlimeCharacter> SlimeCharacterClassToMetamorphose = MetamorphosisData->GetSlimeCharacterClassToMetamorphose();
		FActorSpawnParameters ActorSpawnParams;
		FTransform ActorSpawnTransform = OriginalSlimeCharacter->GetActorTransform();

		MetamorphosedSlimeCharacter = World->SpawnActor<AFallingSlimeSlimeCharacter>(SlimeCharacterClassToMetamorphose, ActorSpawnTransform, ActorSpawnParams);
		MetamorphosedSlimeCharacter->GetCapsuleComponent()->SetCollisionEnabled(OriginalCollisionEnabled);

		AController* OriginalController = OriginalSlimeCharacter->Controller;
		const FRotator OriginalControlRotation = OriginalSlimeCharacter->GetControlRotation();

		OriginalController->Possess(MetamorphosedSlimeCharacter);
		// 所有時にアクタの正面に ControlRotation を合わせてしまうため、修正する
		OriginalController->SetControlRotation(OriginalControlRotation);

		MetamorphosedSlimeCharacter->MetamorphosedFrom(OriginalSlimeCharacter);

		OriginalSlimeCharacter->Destroy();

		if (UNiagaraSystem* NiagaraSystem_Metamorphosed = MetamorphosisData->GetNiagaraSystem_Metamorphosed())
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				/* WorldContextObject = */ MetamorphosedSlimeCharacter,
				/* SystemTemplate = */ NiagaraSystem_Metamorphosed,
				MetamorphosedSlimeCharacter->GetActorLocation(),
				MetamorphosedSlimeCharacter->GetActorRotation());
		}
	}

	return MetamorphosedSlimeCharacter;
}
