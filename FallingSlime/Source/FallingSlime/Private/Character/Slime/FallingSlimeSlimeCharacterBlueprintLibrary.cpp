// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Slime/FallingSlimeSlimeCharacterBlueprintLibrary.h"

// Include headers in Engine module.
#include "Components/CapsuleComponent.h"

// Include headers in Niagara module.
#include "NiagaraFunctionLibrary.h"

FFallingSlimeSlimeCharacterMetamorphosisResult UFallingSlimeSlimeCharacterBlueprintLibrary::TryMetamorphose(AFallingSlimeSlimeCharacter* OriginalSlimeCharacter, const UFallingSlimeSlimeCharacterMetamorphosisData* MetamorphosisData)
{
	if (!OriginalSlimeCharacter)
	{
		return { false };
	}

	if (!MetamorphosisData)
	{
		return { false };
	}

	FFallingSlimeSlimeCharacterMetamorphosisResult Result{ false };

	if (UWorld* World = OriginalSlimeCharacter->GetWorld())
	{
		// 変身後のスライムの生成処理

		UCapsuleComponent* OriginalCapsuleComp = OriginalSlimeCharacter->GetCapsuleComponent();
		// 衝突して新しいスライムのスポーン位置がずれてしまうため、コリジョンを無効化する
		ECollisionEnabled::Type OriginalCollisionEnabled = OriginalCapsuleComp->GetCollisionEnabled();
		OriginalCapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		TSubclassOf<AFallingSlimeSlimeCharacter> SlimeCharacterClassToMetamorphose = MetamorphosisData->GetSlimeCharacterClassToMetamorphose();
		FActorSpawnParameters ActorSpawnParams;
		// 動作が不安定なため省略。プロパティのコピーが必要な場合、AFallingSlimeCharacter::MetamorphosedFrom 関数内で実装可能
		// ActorSpawnParams.Template = OriginalSlimeCharacter;
		FTransform ActorSpawnTransform = OriginalSlimeCharacter->GetActorTransform();
		
		AFallingSlimeSlimeCharacter* MetamorphosedSlimeCharacter = World->SpawnActor<AFallingSlimeSlimeCharacter>(SlimeCharacterClassToMetamorphose, ActorSpawnTransform, ActorSpawnParams);
		MetamorphosedSlimeCharacter->GetCapsuleComponent()->SetCollisionEnabled(OriginalCollisionEnabled);


		// AController が所有する Pawn の更新

		AController* OriginalController = OriginalSlimeCharacter->Controller;
		const FRotator OriginalControlRotation = OriginalSlimeCharacter->GetControlRotation();

		OriginalController->Possess(MetamorphosedSlimeCharacter);
		// 所有時にアクタの正面に ControlRotation を合わせてしまうため、修正する
		OriginalController->SetControlRotation(OriginalControlRotation);


		// 変身後の処理

		MetamorphosedSlimeCharacter->MetamorphosedFrom(OriginalSlimeCharacter);

		OriginalSlimeCharacter->Destroy();

		Result.bHasMetamorphosed = true;
		Result.MetamorphosedSlimeCharacter = MetamorphosedSlimeCharacter;


		// 変身時のエフェクトなどの再生処理

		if (UNiagaraSystem* NiagaraSystem_Metamorphosed = MetamorphosisData->GetNiagaraSystem_Metamorphosed())
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				/* WorldContextObject = */ MetamorphosedSlimeCharacter,
				/* SystemTemplate = */ NiagaraSystem_Metamorphosed,
				MetamorphosedSlimeCharacter->GetActorLocation(),
				MetamorphosedSlimeCharacter->GetActorRotation());
		}
	}

	return Result;
}
