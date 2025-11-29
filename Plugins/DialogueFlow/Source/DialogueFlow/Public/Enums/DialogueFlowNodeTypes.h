#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType, Category="Dialogue Flow", Meta=(DisplayName="Dialogue Flow Node Type"))
enum class EDialogueFlowNodeType : uint8
{
    Start,
    End,
    Dialogue,
    Event,
    Condition,
    Unknown
};
