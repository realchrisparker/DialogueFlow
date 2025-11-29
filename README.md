# Dialogue Flow – UE 5.6 Dialogue Editor Plugin
*A node-based conversation authoring tool for Unreal Engine*

Dialogue Flow is a **fully custom Unreal Engine 5.6 plugin** that provides a rich **graph-based Dialogue Editor**, a dedicated **Dialogue Asset**, and a **runtime DialogueFlowComponent** for executing branching conversations in-game.

Built on UE’s native graph and Slate systems (e.g., `UEdGraph`, `UEdGraphNode`, `SGraphNode`, `SGraphPin`, `SGraphEditor`), Dialogue Flow delivers a familiar yet highly specialized toolset for designing conversations directly inside the Editor.

---

## ✨ Features

### 🎛 Graph-Based Dialogue Editor
- Custom Dialogue Graph built on top of Unreal’s **EdGraph** system  
- Specialized node types:
  - **Start Node** – Beginning of a conversation  
  - **End Node** – Terminates a conversation  
  - **Dialogue Node** – Speaker text + multiple selectable choices  
  - **Event Node** – Fires gameplay events  
  - **Condition Node** – Branches based on runtime logic  
- Dynamic **multiple-choice output pins** that can be added/removed in the editor  
- Custom SGraphNode visuals inspired by Animation State Machine nodes

### 🧰 Custom Editor Toolkit
- Built using `FAssetEditorToolkit` and `FTabManager`
- Registers its own Editor Tab for the Dialogue Graph
- Supports:
  - Graph selection callbacks  
  - Details panel integration  
  - Read-only and PIE overlays  
  - Editor command bindings (cut/copy/paste/undo/redo)

### 🎨 Custom Slate UI
- Custom widgets derived from:
  - `SGraphNode`
  - `SGraphPin`
  - `SNodeTitle`
  - `SGraphEditor`
- Custom styling for node bodies, pins, title bars, and icons  
- Dynamic property updates that visually refresh the graph node

### 🧩 Dialogue Asset Type
- New asset: **UConversationAsset**
- Stores:
  - Name  
  - Description  
  - Editor Graph reference  
- Serialized via standard asset saving
- Opens in a full custom asset editor

### 🎮 Runtime DialogueFlowComponent
- Attach to any Actor to run dialogue assets
- Provides Blueprint-compatible events:
  - **OnDialogueNodeChanged**
  - **OnDialogueEventTriggered**
  - **OnDialogueEnded**
- Can drive UI widgets, voice acting, cinematics, or gameplay events

---

## 🧠 Technical Architecture

### Core UE Systems Used
- **Asset Editing:**  
  - `FAssetEditorToolkit`, `FTabManager`, `SDockTab`
- **Graph System:**  
  - `UEdGraph`, `UEdGraphNode`, `UEdGraphPin`, `UEdGraphSchema`
- **Slate Widgets:**  
  - `SGraphEditor`, `SGraphNode`, `SGraphPin`
- **Commands:**  
  - `FGenericCommands` (cut/copy/paste/undo/redo)
- **Property Notify:**  
  - `FNotifyHook`

---

## 📂 Source Tree Overview
```
DialogueFlow/
│
├── Public/
│   └── Editor/
│
├── Private/
│   └── Editor/
│
├── Nodes/
│   ├── Base
│   ├── Start
│   ├── End
│   ├── Dialogue
│   ├── Event
│   └── Condition
│
├── Runtime/
│   ├── DialogueFlowComponent.h/.cpp
│   └── Data
│
└── Assets/
    └── ConversationAsset.h/.cpp
```


---

## 📘 Installation

1. Place the plugin folder in:
2. Open Unreal Engine → **Edit → Plugins**
3. Enable **Dialogue Flow**
4. Restart the Editor

---

## 🚀 Using Dialogue Flow

### Creating a Dialogue
1. Right-click in the Content Browser  
2. Choose **Dialogue → Conversation Asset**
3. Open it to launch the custom Dialogue Editor

### Editing the Conversation
- Add Start, Dialogue, Event, Condition, and End nodes  
- Drag connections to define flow  
- Edit text, speaker, and settings in the Details panel  
- Add or remove choice pins dynamically  
- Save like any other UE asset

### Running a Dialogue at Runtime
```cpp
UDialogueFlowComponent* Flow = FindComponentByClass<UDialogueFlowComponent>();

if (Flow)
{
 Flow->StartConversation(MyConversationAsset);
}
```


Copyright © 2025 God's Studio
All Rights Reserved.
This plugin and its source code may not be copied, redistributed, or published without written permission.
