A minimal, cross-platform Time Tracking App (C++/Qt)

On Ubuntu (GNOME):
![A screenshot of the main page of the app on Ubuntu (GNOME)](tta-main-gnome-screenshot.png)

## Features

### Roadmap
<u>VERSION 0.1</u> <br/>
🗹 Create projects, save and load them from disk <br/>
🗹 Edit project : rename, delete <br/>
🗹 Dynamic project loading (default directory for saves + file loading if saved elsewhere) <br/>
🗹 Start and stop session, added automatically to project stats <br/>
🗹 Display various project stats by processing sessions data (next: calendar, see below 0.X) <br/>
🗹 Use customizable colors to distinguish projects <br/>
🗹 Display "Edit" and "Stats" features inside a collapsable panel <br/> 
🗹 Manual add session (also for testing purpose) <br/>
☐ JSON support <br/>

<u>VERSION 0.X</u> <br/>
☐ Manual delete session (and maybe better, thus dedicated UI for Sessions handling? see below) <br/>
☐ Better UI for sessions handling (manual add, delete, etc.) <br/>
☐ Year active days and sessions count per day graph calendar (similar to GitHub/GitLab profiles) <br/>
☐ Stacked widgets to differentiate stats and edit (maybe needed after having the calendar)? <br/>
☐ Make project cards moveable (drag-and-drop) <br/>

<u>VERSION 1.0</u> <br/>
☐ Ensure basic tests <br/>
☐ Ensure complete sessions uniqueness and reliability (input bugs, save bugs..) <br/>
☐ Ensure project data reliability (input bugs, save bugs..) <br/>

<u>TODO: LATER</u> <br/>
☐ Rethink the window size policy (is set fixed for now for simplicity) <br/>
☐ Project categories? <br/>

## Building and compatibility

Build with CMake.

Developed on Linux (Ubuntu) with QtCreator and Windows with VS2022 (Qt plugin and CMake support needed).

Tested on Linux Ubuntu and Windows 10.

#### Note
Qt projects built with CMake might have build issues related to the moc pipepline (the include of to-be-generated Qt' ui files). 
If so, you'll find the ui_*.h files in your /out/build directory that is generated at every CMake launch. 
This issue is especially happening on Windows, if you're using Visual Studio you might need to either : 
***Reconfigure your project using CMake*** (and clean build) 
or 
***Clean the project (also possible to delete the build directory) and restart VS as it will trigger the necessay CMake rebuilding.***
