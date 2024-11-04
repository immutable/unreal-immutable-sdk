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

* [Immutable X](https://docs.immutable.com/sdks/unreal)
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
## BLUI Plugin

For Unreal Engine versions 4.26, 4.27, and 5.0, we use the BLUI plugin because Epic's WebBrowserWidget plugin is incompatible with this SDK due to its reliance on an older version of CEF.

Please follow the [installation instructions](https://github.com/immutable/immutable-BLUI?tab=readme-ov-file#installation) to install the Immutable BLUI plugin.

## Contributing
Thank you for your interest in contributing to our project! Here's a quick guide on how you can get started:

1. **Fork this Repository**: Fork the repository to your GitHub account by clicking the "Fork" button at the top right of the repository page.
2. **Create a Branch**: Once you've forked the repository, create a new branch in your forked repository where you'll be making your changes. Branch naming convention is enforced [according to patterns here](https://github.com/deepakputhraya/action-branch-name).
3. **Make Changes**: Make the necessary changes in your branch. Ensure that your changes are clear, well-documented, and aligned with the project's guidelines.
4. **Commit Changes**: Commit your changes with clear and descriptive messages following [commit message pattern here](https://github.com/conventional-changelog/commitlint?tab=readme-ov-file#what-is-commitlint). It follows [Conventional Commits specification](https://www.conventionalcommits.org/en/v1.0.0/#specification), which helps maintain a consistent and informative commit history. Read [here](https://www.conventionalcommits.org/en/v1.0.0/#why-use-conventional-commits) to learn more about the benefits of Conventional Commits.
5. **Create a Pull Request (PR)**: After you've made and committed your changes, create a PR against the original repository. Provide a clear description of the changes you've made in the PR.
6. **Example Contribution**: Refer to [this contribution](https://github.com/immutable/unity-immutable-sdk/pull/182) as an example.

## Getting Help

Immutable X is open to all to build on, with no approvals required. If you want to talk to us to learn more, or apply for developer grants, click below:

[Contact us](https://www.immutable.com/contact)

### Project Support

To get help from other developers, discuss ideas, and stay up-to-date on what's happening, become a part of our community on Discord.

[Join us on Discord](https://discord.gg/TkVumkJ9D6)

You can also join the conversation, connect with other projects, and ask questions in our Immutable X Discourse forum.

[Visit the blog](https://www.immutable.com/blog)

#### Still need help?

You can also apply for marketing support for your project. Or, if you need help with an issue related to what you're building with Immutable X, click below to submit an issue. Select _I have a question_ or _issue related to building on Immutable X_ as your issue type.

[Contact support](https://support.immutable.com/hc/en-us/requests/new)

## License

Immutable Unreal SDK repository is distributed under the terms of the [Apache License (Version 2.0)](LICENSE).
