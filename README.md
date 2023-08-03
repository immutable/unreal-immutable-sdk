# Immutable SDK for Unreal 

## User Guide

### Installation

To install the plugin download it into your project's `Plugins` folder, e.g.: `MyGame/Plugins/unreal-immutable-sdk`.

### Setup

#### Blueprint

See the included sample widgets in `All`->`Plugins`->`Immutable Content` in the Unreal Editor content browser for examples of how to use the plugin in Blueprint.  If you don't see a `Plugins` folder under `All` you may need to enable `Show Plugin Content` in the content browser's `Settings` menu.

#### C++

See `ImmutablePassport.h` for the C++ Passport API.  Follow the same sequence of steps as the Blueprint widget example to initialise and connect Passport and retrieve account data.


## Plugin Maintainers' Guide

### Prerequisites

- Windows 10 or 11
- git
- Epic Games Launcher
- Unreal Engine installed from the Epic Games Launcher; you will need every version that the plugin supports (currently 4.27, 5.0, 5.1, 5.2)
- Visual Studio 2022 with C++ support

The Jetbrains Rider IDE is recommended for working with Unreal Engine C++ code.  If not Rider, Visual Studio is fine and even better with the Resharper plugin.

### Getting Started

See user docs to install the plugin, with one caveat:  If you want to open the project and plugin in an IDE you'll need to create your Unreal project as a C++ project.  If you've already accidentally created your Unreal project as BluePrint-only you can convert it to a BluePrint-and-C++ project by adding any C++ class from the `Add New C++ Class` option in the Unreal Editor's `File` menu.

### Debugging the Embedded Browser

#### Embedded Browser Logs

See `Saved/Logs/cef3.log` in your Unreal project.

#### JS Console Output

In Unreal Engine 5.1+ the project is set up so that `console.log()` will pass all the way through to the Unreal console.  This isn't available in UE5.0 and earlier but you can still use Chromium's remote debugging (see below).

#### Remote Debugging

To set up Chromium remote debugging, edit project build/launch configurations in your IDE to add the following program command line argument to the appropriate configurations: `cefdebug=9222`

Next, launch the game and then open a separate Chrome/Chromium browser and navigate to `chrome://inspect` where your app should be listed, sometimes intermittently or after a delay, under the `Remote Target #localhost` section.  Click `inspect` to launch the remote debugging session.

#### Displaying the Browser

**TODO**

