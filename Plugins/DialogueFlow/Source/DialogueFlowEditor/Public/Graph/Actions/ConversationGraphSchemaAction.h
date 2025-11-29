#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "ConversationGraphSchemaAction.generated.h"

class UConversationGraphNode;

USTRUCT()
struct DIALOGUEFLOWEDITOR_API FConversationGraphSchemaAction_NewNode : public FEdGraphSchemaAction
{
    GENERATED_BODY()

public:

    /** The editor graph node class to spawn */
    UPROPERTY()
    TSubclassOf<UEdGraphNode> GraphNodeClass;

    FConversationGraphSchemaAction_NewNode()
        : FEdGraphSchemaAction()
        , GraphNodeClass(nullptr)
    {
    }

    FConversationGraphSchemaAction_NewNode(
        const FText& InNodeCategory,
        const FText& InMenuDesc,
        const FText& InToolTip,
        const int32 InGrouping,
        TSubclassOf<UEdGraphNode> InGraphNodeClass
    )
        : FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping)
        , GraphNodeClass(InGraphNodeClass)
    {
    }

    virtual UEdGraphNode* PerformAction(
        UEdGraph* ParentGraph,
        UEdGraphPin* FromPin,
        const FVector2D Location,
        bool bSelectNewNode = true
    ) override;
};

