<div align="center">
  <p align="center">
    <a  href="https://docs.x.immutable.com/docs">
      <img src="https://cdn.dribbble.com/users/1299339/screenshots/7133657/media/837237d447d36581ebd59ec36d30daea.gif" width="280"/>
    </a>
  </p>
</div>

---

# Immutable SDK for Unreal Engine

## User Guide

### Supported Platforms

- Windows 10 (64-bit)
- MacOS

### Supported Unreal Engine Versions

- Unreal Engine 5.0 and above

> [!NOTE]
> Support for Unreal Engine 4.27 work is currently underway.

### Installation

To install the plugin download it into your project's `Plugins` folder, e.g.: `MyGame/Plugins/unreal-immutable-sdk`.

### Setup

#### Blueprint

See the included sample widgets in `All`->`Plugins`->`Immutable Content` in the Unreal Editor content browser for examples of how to use the plugin in Blueprint.  If you don't see a `Plugins` folder under `All` you may need to enable `Show Plugin Content` in the content browser's `Settings` menu.

#### C++

See `ImmutablePassport.h` for the C++ Passport API.  Follow the same sequence of steps as the Blueprint widget example to initialise and connect Passport and retrieve account data.

### Getting Started

See user docs to install the plugin, with one caveat:  If you want to open the project and plugin in an IDE you'll need to create your Unreal project as a C++ project.  If you've already accidentally created your Unreal project as BluePrint-only you can convert it to a BluePrint-and-C++ project by adding any C++ class from the `Add New C++ Class` option in the Unreal Editor's `File` menu.

### Passport Login Flow

See this Blueprint showing how to login to passport ![Passport Login Blueprint](PassportLoginFlow.jpg)

Here are the steps to login to passport:

1. `Initialise Passport` to initialise passport instance.
2. Check if there are stored credentials from previous login.
3. If stored credentials found, `ConnectSilent` uses the saved credentials to login to Passport.
4. When no stored credentials found or step 3 fails, attempt to make a new Connection to login to Passport using `Connect`. This will open the gamer's default browser and take them through the auth flow.

Once the gamer is connected to Passport, the SDK will store your credentials (access, ID, and refresh tokens).

#### Note

We use the [Device Code Authorisation](https://auth0.com/docs/get-started/authentication-and-authorization-flow/device-authorization-flow#:~:text=Your%20Auth0%20Authorization%20Server%20redirects,authorized%20to%20access%20the%20API.) flow to authenticate and authorise gamers.

### Log out of Passport

See this Blueprint showing how to logout from passport ![Passport Logout Blueprint](PassportLogoutFlow.jpg)

## Supported Functionality

- GetAddress
- GetEmail
- CheckStoredCredentials
- Connect
- ConnectSilent

See the [ImmutablePassport.h](https://github.com/immutable/unreal-immutable-sdk/blob/dc39324db204f2ba30e9c9f0ca25c070987785cb/Source/Immutable/Public/Immutable/ImmutablePassport.h#L115C8-L115C8) header for the full API.

## Examples

- **Sample Blueprints** - see the [sample blueprints](https://github.com/immutable/unreal-immutable-sdk/tree/main/Content/BlueprintSampleContent) for examples of how to use the Immutable Unity SDK.

- **Sample Game** - see the [sample game](https://github.com/immutable/sample-unreal-game) for example of how to use the Immutable Unity SDK.

## Plugin Maintainers' Guide

### Prerequisites

- git
- Epic Games Launcher
- Unreal Engine installed from the Epic Games Launcher; you will need every version that the plugin supports (currently 4.27, 5.0, 5.1, 5.2)
- Visual Studio 2022 with C++ support
- Jetbrains Rider IDE (recommended for working with Unreal Engine C++ code)

If not Rider, Visual Studio is fine and even better with the Resharper plugin.

### Debugging the Embedded Browser

#### Embedded Browser Logs

See `Saved/Logs/cef3.log` in your Unreal project.

#### JS Console Output

In Unreal Engine 5.1+ the project is set up so that `console.log()` will pass all the way through to the Unreal console.  This isn't available in UE5.0 and earlier but you can still use Chromium's remote debugging (see below).

#### Remote Debugging

To set up Chromium remote debugging, edit project build/launch configurations in your IDE to add the following program command line argument to the appropriate configurations: `cefdebug=9222`

Next, launch the game and then open a separate Chrome/Chromium browser and navigate to `chrome://inspect` where your app should be listed, sometimes intermittently or after a delay, under the `Remote Target #localhost` section.  Click `inspect` to launch the remote debugging session.

## Changelog Management

The following headings should be used as appropriate.

- Added
- Changed
- Deprecated
- Removed
- Fixed

What follows is an example with all the change headings, for real world use only use headings when appropriate.
This goes at the top of the CHANGELOG.md above the most recent release.

```markdown
...

## [Unreleased]

### Added

for new features.

### Changed

for changes in existing functionality.

### Deprecated

for soon-to-be removed features.

### Removed

for now removed features.

### Fixed

for any bug fixes.

...
```

## Contributing

If you would like to contribute, please read the following:

- We use the [Conventional Commits specification](https://www.conventionalcommits.org/en/v1.0.0/#specification) when writing our commit messages. Why use Conventional Commits? Read [here](https://www.conventionalcommits.org/en/v1.0.0/#why-use-conventional-commits).

## Getting Help

Immutable X is open to all to build on, with no approvals required. If you want to talk to us to learn more, or apply for developer grants, click below:

[Contact us](https://www.immutable.com/contact)

### Project Support

To get help from other developers, discuss ideas, and stay up-to-date on what's happening, become a part of our community on Discord.

[Join us on Discord](https://discord.gg/TkVumkJ9D6)

You can also join the conversation, connect with other projects, and ask questions in our Immutable X Discourse forum.

[Visit the forum](https://forum.immutable.com/)

#### Still need help?

You can also apply for marketing support for your project. Or, if you need help with an issue related to what you're building with Immutable X, click below to submit an issue. Select _I have a question_ or _issue related to building on Immutable X_ as your issue type.

[Contact support](https://support.immutable.com/hc/en-us/requests/new)

## License
Immutable Unity SDK repository is distributed under the terms of the [Apache License (Version 2.0)](LICENSE).