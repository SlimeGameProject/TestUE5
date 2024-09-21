// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType, DisplayName = "SlimeCharacterCustomMovementMode")
enum class EFallingSlimeSlimeCharacterCustomMovementMode : uint8
{
    None = 0			UMETA(DisplayName = "None"),
    Sticking = 1		UMETA(DisplayName = "Sticking"),
    StoneLanding = 2	UMETA(DisplayName = "StoneLanding"),
    StoneFalling = 3	UMETA(DisplayName = "StoneFalling"),
};
