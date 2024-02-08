<div align="center">
  <p align="center">
    <a  href="https://docs.x.immutable.com/docs">
      <img src="https://cdn.dribbble.com/users/1299339/screenshots/7133657/media/837237d447d36581ebd59ec36d30daea.gif" width="280"/>
    </a>
  </p>
</div>

---

# Immutable SDK for Unreal Engine

# Documentation

* [Immutable X](https://docs.immutable.com/docs/x/sdks/unreal)
* [Immutable zkEVM](https://docs.immutable.com/docs/zkEVM/sdks/unreal)

## Plugin Maintainers' Guide

### Prerequisites

- git
- Unreal Engine installed from the Epic Games Launcher or built from the source code. Make sure to check the list of currently supported Unreal Engine versions.
- Microsoft Visual Studio. It is crucial to have a compatible version of Visual Studio installed. Please refer to the official documentation of Unreal Engine.

### Getting Started

See [documentation](#documentation) on how to install the plugin, with one caveat:  If you want to open the project and plugin in an IDE you'll need to create your Unreal project as a C++ project.  If you've already accidentally created your Unreal project as BluePrint-only you can convert it to a BluePrint-and-C++ project by adding any C++ class from the `Add New C++ Class` option in the Unreal Editor's `File` menu.

### Debugging the Embedded Browser

#### Embedded Browser Logs

See `Saved/Logs/cef3.log` in your Unreal project.

#### JS Console Output

In Unreal Engine 5.1+ the project is set up so that `console.log()` will pass all the way through to the Unreal console.  This isn't available in UE5.0 and earlier but you can still use Chromium's remote debugging (see below).

#### Remote Debugging

To set up Chromium remote debugging, edit project build/launch configurations in your IDE to add the following program command-line argument to the appropriate configurations: `cefdebug=9222`

Next, launch the game and then open a separate Chrome/Chromium browser and navigate to `chrome://inspect` where your app should be listed, sometimes intermittently or after a delay, under the `Remote Target #localhost` section.  Click `inspect` to launch the remote debugging session.

## Release Unreal Immutable SDK Plugin

1. Release the plugin using GitHub releases. See https://github.com/immutable/unreal-immutable-sdk/releases

## Release Blui Plugin for UE4

For UE 4.26 and 4.27 we are using Blui as the in built browser does not work.

1. Download Blui release from https://github.com/immutable/BLUI-Unreal/tree/Imtbl4.2
2. Download CEF Browser from https://github.com/getnamo/BLUI-Unreal/releases/tag/v4.6.0 (`BluBrowserCEF....7z`)
3. Place the Win folder from the `BluBrowserCEF...7z` into the BLUI plugin `Thirdparty/cef`
4. Zip the Blui plugin folder and upload to the GitHub release.

## How to Add Game Bridge Asset to Packaged Resources

1. Open Unreal Editor in the lowest supported version (i.e. 4.26) for this Plugin.
2. Open Content Drawer
3. Navigate to Plugins -> Immutable Content -> Packages Resources
4. Delete the existing imported `index.uasset`
5. Right click and import the `index.js` file
   ![Import Asset](Docs/ImportAsset.png)

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

Immutable Unreal SDK repository is distributed under the terms of the [Apache License (Version 2.0)](LICENSE).
