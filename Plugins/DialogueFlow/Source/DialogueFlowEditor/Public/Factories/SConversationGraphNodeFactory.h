#pragma once

#include "EdGraphUtilities.h"

class FConversationGraphNodeFactory : public FGraphPanelNodeFactory
{
public:
    virtual TSharedPtr<SGraphNode> CreateNode(UEdGraphNode* Node) const override;
};
