#include "Modules/ModuleManager.h"

class FDialogueFlowModule : public IModuleInterface
{
public:
    virtual void StartupModule() override {}
    virtual void ShutdownModule() override {}
};

IMPLEMENT_MODULE(FDialogueFlowModule, DialogueFlow);
