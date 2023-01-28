# ctle

C++ Template Library Extension classes - Extension classes for Windows and Linux STL

## Dependencies & Package Management

This repository uses vcpkg as the C++ package manager. CMake and Ninja for building. GTest for test.

Packages are added to vcpkg.json. Use vcpkg-configuration.json for adding custom ports using filesystem/git based registries. vcpkg is using manifest mode so install would be called by cmake.

vcpkg is fetched using CMake

## How to build

After checking out you can run the following commands to list and use presets. If you are using VSCode you can use then directly through Ctrl+Shift+P ( CMake:Configure, CMake:Build, CMake:Run Test, CMake:Install )

```
cmake --list-presets=all
```

To run a configure preset

```
cmake --preset <configurePreset-name>
```

To run a build preset
```
cmake --build --preset <buildPreset-name>
```

To run a test preset
```
ctest --preset <testPreset-name>
```

## Linux using DevContainer setup

A devcontainer setup can be setup for x64-linux platform. 
See [Developing inside a Container](https://code.visualstudio.com/docs/devcontainers/containers) for how to setup devcontainer and use them in VSCode.

You can use the devcontainer setup from the following repo.

```
git clone https://github.com/mtungekar/devcontainer.git .devcontainer
```



Some usefull Ctrl+Shift+P commands are :

- Dev Containers: Rebuild and Reopen in Container

- Dev Containers: Open Folder in Container

- ReOpen Folder Locally (once VSCode is running in container)


## License

Copyright (c) 2022 Ulrik Lindahl
Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
