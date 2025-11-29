#pragma once

#include "CoreMinimal.h"
#include "FDialogueChoice.generated.h"

/**
 * Represents a single selectable dialogue option for the player.
 * Each choice corresponds to an output pin on the editor graph node.
 */
USTRUCT(BlueprintType)
struct FDialogueChoice
{
    GENERATED_BODY()

public:

    /**
     * Short clickable prompt shown to the player.
     * Not the full spoken line, just the summary shown in the UI.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Choice")
    FText ChoiceTitle;

    /**
     * Full localized text spoken after the player selects this option.
     * Optional—some games do silent protagonists.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Choice", meta = (MultiLine = "true"))
    FText ChoiceFullText;

    /**
     * Icon displayed before the choice title (e.g. LifePath, Attribute icon).
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Choice")
    TObjectPtr<UTexture2D> PrefixIcon = nullptr;

    /**
     * Icon displayed after the choice title (e.g. Money cost, Item requirement).
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Choice")
    TObjectPtr<UTexture2D> SuffixIcon = nullptr;

    /**
     * Index of the output pin this choice corresponds to.
     * Editor-managed: do NOT modify manually.
     */
    UPROPERTY(VisibleAnywhere, Category = "Choice")
    int32 LinkedOutputPinIndex = INDEX_NONE;
};
