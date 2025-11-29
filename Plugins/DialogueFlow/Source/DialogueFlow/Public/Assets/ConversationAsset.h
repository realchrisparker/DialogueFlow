// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationAsset.h
// Description: Primary asset that stores all dialogue flow nodes for a 
//              conversation. This asset is created from the Content Browser
//              and edited inside the Dialogue Flow Editor.
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ConversationAsset.generated.h"


/**
 * UConversationAsset
 *
 * Represents a dialogue conversation containing a collection of dialogue nodes.
 * This asset is created in the Content Browser through a menu entry
 * and opened by the custom Dialogue Flow Editor.
 */
UCLASS(BlueprintType, Category = "Dialogue Flow", meta = (DisplayName = "Conversation Asset"))
class DIALOGUEFLOW_API UConversationAsset: public UObject
{
    GENERATED_BODY()

public:

    /*
     * Functions
    */

    /** Constructor */
    UConversationAsset();

    /*
     * Properties
    */
    
    UPROPERTY(EditAnywhere, Category = "Dialogue Flow", meta = (DisplayName = "Conversation Name"))
    FString Name;

    UPROPERTY(EditAnywhere, Category = "Dialogue Flow", meta = (DisplayName = "Conversation Description"))
    FString Description;

    /**
     * All nodes that make up this conversation.
     * These nodes are instanced subobjects so they can be edited inline,
     * serialized with the asset, and support undo/redo operations.
     */
    UPROPERTY(EditAnywhere, Instanced, Category = "Dialogue Flow", meta = (DisplayName = "Nodes"))
    TArray<class UDialogueFlowBaseNode*> Nodes;

    /**
     * The editor graph representing this conversation in the Dialogue Flow Editor.
     *
     * This graph stores node positions, links, and visual layout.
     * It is *not* exposed to designers and is used purely by the editor.
     */
    UPROPERTY()
    TObjectPtr<UEdGraph> EditorGraph;
};
