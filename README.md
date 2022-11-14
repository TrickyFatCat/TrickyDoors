# TrickyDoors

A plugin with a collection of door actors.

## Dependencies

This plugin required this plugins to work:

1. [**TrickyInteractionSystem**](https://github.com/TrickyFatCat/TrickyInteractionSystem) v1.2 and higher;
2. [**TrickyAnimationComponents**](https://github.com/TrickyFatCat/TrickyAnimationComponents) v1.2 and higher;
3. [**TrickyKeyringSystem**](https://github.com/TrickyFatCat/TrickyKeyringSystem) v1.1 and higher;

Install them before installing this plugin.

## Installation

The plugin can be used in both C++ and Blueprint projects.

### Blueprint projects

**At the moment, the package is compatible only with Unreal Engine 4.**

1. Download package file;
2. Unzip the package to the Plugins folder in engine folder, e.g. `C:\Program Files\Epic Games\UE_4.27\Engine\Plugins`;
3. Restart the project;

In this case the plugin can be used for any blueprint project.

### C++ projects

1. Create the Plugins folder in the project directory;
2. Create the TrickyAnimationComponents folder in the Plugins folder;
3. Download the plugin source code into that folder;
4. Rebuild the project;

## Content

1. DoorBase class;
2. DoorAuto class;
3. DoorInteractive class;

### DoorBase

A base door class which contains the basic door logic.

#### Animation setup

The door uses the TimelineAnimationComponent from [**TrickyAnimationComponents**](https://github.com/TrickyFatCat/TrickyAnimationComponents).

Read the instructions how to setup it on its git page.

#### Variables

1. `InitialState` - the door state on begin play;
2. `IsReversible` - toggles if the door animation can be reversed;
3. `CalculateSwingDirection` - If true, the rotation offset can be recalculated using the `CalculateSwingDirection` function;

#### Functions

1. `Open` - opens the door;
2. `Close` - closes the door;
3. `SetIsLocked` - sets if the door is locked or not;
4. `SetIsEnabled` - sets if the door is enabled or not;
5. `StartAutoClosingTimer` - starts the timer after which the door will be closed automatically;
6. `StopAutoClosingTimer` - stops autoclosing timer;
7. `CalculateSwingDirection` - calculates new rotation offset relative to the given actor;

#### Delegates

1. `OnStateChanged` - called when current state was changed;
2. `OnReversed` - called when door animation was reversed;

#### Implementable Events

1. `OnDoorChangedState` - called when the current state was changed;
2. `OnDoorAnimationReversed` - called when the door animation was reversed;

### DoorAuto

A door which can be opened automatically by entering a trigger. 

#### Variables

1. `RequireKey` - toggles if the door requires a key to open it;
2. `KeyClass` - key class which the actor must have to open the door;
3. `IsClosingDelayed` - toggles if the door will close automatically after some time;
4. `ClosingDelayDuration` - delay after which the opened door will close automatically;

### DoorInteractive

A door which requires interaction to be opened.

#### Variables

1. `RequireKey` - toggles if the door requires a key to open it;
2. `KeyClass` - key class which the actor must have to open the door;
3. `IsClosingDelayed` - toggles if the door will close automatically after some time;
4. `ClosingDelayDuration` - delay after which the opened door will close automatically;
5. `InteractionData` - data used for interaction;
6. `InteractionMessages` - interaction messages for different states of the door;
7. `CantUnlockMessage` - the message in case the actor don't have a key to unlock the door;