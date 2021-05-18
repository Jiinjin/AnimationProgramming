# AnimationProgramming

## Hardware needed

I personally use Visual Studio 2019 & Cmake 3.16.3 to setup the project

## Setup the project
First of, clone the project wherever you want with the following command :

`git clone https://github.com/Jiinjin/AnimationProgramming.git`

Then open Cmake, and put the project path on the top input text (1), and the same right under but with `/build` at the end (2).

Click the configure button (3).

![Cmake](/Screens/CmakePaths.png)

Confirm by clicking yes to create the new build directory.

![Cmake](/Screens/CmakeConfirm.png)

Then you need to configure the project for Win32, click on the text input (1), and chose Win32 in the list (2).

Click "Finish" (3).

![Cmake](/Screens/CmakeWin32.png)

When CMake finished the configure, click on "Generate" next to the "Configure" button.

When generated, just click on "Open project" and Visual Studio should open the project.

On visual, in the solution explorer, right click on "libMaths" (1) and click the "Build" button (2)

![Cmake](/Screens/VisualBuildLibMath.png)

Right click on "AnimProgExe" (1) and click the "Set as StartUp Project" button (2)

![Cmake](/Screens/VisualSetupAsProject.png)

Then hit CTRL + F5 to build the project or click the "Local Windows Debugger" in the tool bar.

Enjoy !

