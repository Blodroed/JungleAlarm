# Welcome to JungleAlarm
*this is the project ikt104 project for:*
- Albert Salvesen-Orø
- Marcus Hagen
- Lars Angel Ohne
- Aristidis Akritidis


# Setting up mbed-os for use with Visual Studio Code
## Setting up mbed-os with Visual Studio Code using PlatformIO
1. Install the PlatformIO extension in Visual Studio Code. You can do this by going to the Extensions view (Ctrl+Shift+X), searching for "PlatformIO", and clicking Install.

2. Create a new PlatformIO project. Click on the PlatformIO icon in the Activity Bar, then click on "New Project". In the "Board" field, type the name of your mbed board (for example, "DISCO-L475VG-IOT01A"). In the "Framework" field, select "mbed". Then click "Finish".

3. PlatformIO will create a new project with a basic mbed-os setup. You can replace the code in `src/main.cpp` with your own code.

4. To build the project, click on the checkmark icon in the PlatformIO toolbar. To upload the code to your board, click on the right arrow icon.

5. You can also use the PlatformIO: Build and PlatformIO: Upload commands in the Command Palette (Ctrl+Shift+P).

6. To add additional libraries to your project, you can use the `lib_deps` option in the `platformio.ini` file. For example, to add the DFRobot_RGBLCD1602 library, you would add the following line:

```ini
lib_deps = DFRobot_RGBLCD1602
```

7. PlatformIO will automatically download and install the libraries when you build your project.
Remember to include the mbed.h header file in your sourcecodes

```cpp
#include "mbed.h"
```