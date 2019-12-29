# GarageDoorKeypad


## FAQ
1. To flash SW from CLI:
```sh
~/Tools/STM32/STM32CubeProgrammer/bin/STM32_Programmer_CLI -c port=SWD reset=Hwrst -d ~/Projects/GarageDoorKeypad/.pio/build/disco_f051r8/firmware.elf
```

2. Erase all memory:
```sh
 ~/Tools/STM32/STM32CubeProgrammer/bin/STM32_Programmer_CLI -c port=SWD -e all
```

3. Add libs to STM32CubeIDE project
```
1. Go to: Project -> Properties -> C/C++ General -> Paths and Symbols -> Includes.
2. Add library paths.
3. To add AT24CXX library to keypad project:
    - Click `Add` button on the right side.
    - Choose `File System...` button.
    - Add library path: `/home/luk6xff/Projects/GarageDoorKeypad/libs/AT24CXX/platform/stm32cube`.
4. Go to: Project -> Properties -> C/C++ General -> Paths and Symbols -> Source Location.
5. Add library folder paths.
6. To add AT24CXX library to keypad project:
    - Click `Link Folder` button on the right side.
    - Mark `Link to folder in the filesystem`.
    - Choose `Browse...` button.
    - Add library path: `/home/luk6xff/Projects/GarageDoorKeypad/libs/AT24CXX/`.
    - Click `OK` button.
    - Click Right mouse button on the `/keypad/AT24CXX` folder and choose `Edit Filter` button.
    - Add exclusiion pattern: `platform/mbed/` and click `OK`.
```