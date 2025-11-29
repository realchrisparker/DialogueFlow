#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include <Nodes/DialogueFlowBaseNode.h>
#include "ConversationGraphNode.generated.h"

/**
 * Graph node representing a dialogue flow node in the editor
 */
UCLASS()
class DIALOGUEFLOWEDITOR_API UConversationGraphNode : public UEdGraphNode
{
    GENERATED_BODY()

public:

    /** Runtime node reference stored inside the ConversationAsset */
    UPROPERTY()
    UDialogueFlowBaseNode* DialogueNode;

    // Pin name constants
    static const FName PinInput;
    static const FName PinOutput;

    virtual void AllocateDefaultPins() override;
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual void PrepareForCopying() override;

    /** Returns the UObject that should appear in the details panel when this graph node is selected. */
    virtual UObject* GetPropertyObject() const
    {
        return nullptr;
    }

};
