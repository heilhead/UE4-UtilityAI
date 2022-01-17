# Utility AI

A Utility-based AI implementation for Unreal Engine. Inspired by Infinite Axis 
Utility System architecture (see [this the excellent presentation](https://www.gdcvault.com/play/1021848/Building-a-Better-Centaur-AI) by Dave Mark
and Mike Lewis).

If you're unfamiliar with the concept of a utility-based AI, I suggest having
a look at [curvature](https://github.com/apoch/curvature) which explains
the concept very well.

## Project Status

The project is **very early in development** and hasn't been production-tested.
It's also subject to constant refactoring and improvements.

## Implementation Details

One of the main goals of the project is to leverage as much of existing AI functionality of the Unreal Engine, such as the perception system, pawn actions, AI blackboards etc.

Overview of the system:

- `UUtilityAIComponent` - the brain component. Needs to be added to the agents' `AAIController`.
- `UUtilityAITargetSourceComponent` - the perception component. Also needs to be added to the controller. This will use perception system to feed target list to the brain component. It's possible to not use the perception system, and instead implement `IUtilityAITargetSource` interface to provide arbitrary target list from any other source. Targets can be either actors or locations (e.g. potential cover spots).
- `UUtilityAI` and `UUtilityAIDecisionMaker` - the main data assets containing the actions that your AI will perform. They're separate since in future it will be possible to dynamically add and remove decision makers to a running AI. For example, when AI enters a vehicle it may receive an additional skill set relevant to that context.
- `UAITest` and `UAITest_BlueprintBase` - base classes for decision scoring. For each decision there will be a number of these tests performed to determine how optimal the decision would be in current circumstances. Currently **tests are not instanced** and cannot have any state.
- `UUtilityAITask` - the task to execute if the decision wins. The task API is very unstable and will likely change in future. It is intended to be used with blueprints, since you'll most likely want to either use gameplay abilities, or pawn actions. Tasks are instanced and can have state. Upon execution they receive the context object, including currently selected target.
- `UAIInfluenceSubsystem` (a world subsystem) - this isn't actually a part of the utility AI. An octree-based spatial query system to store arbitrary AI influence data. The API is currently very minimal and will likely change. See second half of [this talk](https://www.gdcvault.com/play/1021848/Building-a-Better-Centaur-AI) for some examples of how such system can be used.

Unfortunately, there's not enough code comments and other documentation. This will be addressed in future as the API gets more stable.

## TODO

- Settle on the `UUtilityAITask` API.
- Improve blueprint integration, especially for data providers and tests.
- Support some configuration via config file.
- Provide a way for tests to have state (similar to BT node memory).
- Make `UUtilityAI` instanced so it's possible to add/remove decision makers at runtime.
- Code comments, docs and sample project.
- Support multiplayer environment.

## License

MIT
