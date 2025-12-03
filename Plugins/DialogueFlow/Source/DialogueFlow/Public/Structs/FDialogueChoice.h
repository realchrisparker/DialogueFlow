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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Choice", meta = (DisplayName = "Title"))
    FText ChoiceTitle;

    /**
     * Full localized text spoken after the player selects this option.
     * Optional—some games do silent protagonists.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Choice", meta = (MultiLine = "true", DisplayName = "Full Text"))
    FText ChoiceFullText;

    /**
     * Icon displayed before the choice title (e.g. LifePath, Attribute icon).
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Choice", meta = (DisplayName = "Prefix Icon"))
    TObjectPtr<UTexture2D> PrefixIcon = nullptr;

    /**
     * Icon displayed after the choice title (e.g. Money cost, Item requirement).
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Choice", meta = (DisplayName = "Suffix Icon"))
    TObjectPtr<UTexture2D> SuffixIcon = nullptr;

    /**
     * Index of the output pin this choice corresponds to.
     * Editor-managed: do NOT modify manually.
     */
    UPROPERTY(VisibleAnywhere, Category = "Choice", meta = (DisplayName = "Linked Output Pin Index"))
    int32 LinkedOutputPinIndex = INDEX_NONE;

    /**
     * Globally unique identifier mapping this choice to a corresponding
     * editor output pin. This GUID persists across save/load, undo/redo,
     * dynamic pin reconstruction, and choice reordering.
     *
     * The editor module assigns this GUID when the choice is created
     * (or when opening older assets), and then ensures the editor pin’s
     * PersistentGuid matches this value.
     */
    UPROPERTY(VisibleAnywhere, Category = "Choice", meta = (DisplayName = "Pin GUID"))
    FGuid PinGuid;
};
