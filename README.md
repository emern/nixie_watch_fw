# nixie_watch_fw

Nixie tube watch firmware written in C, designed to be run on the STM8L family of microprocessors by STMicroelectronics. Firmware has been platformized to support both a hardware breakout board or target hardware. Firmware makes use of the STMicroelectronics Standard Peripheral Library as well as the ST Visual Develop IDE using the Cosmic C compiler.

## How to Use

In order to download and compile/develop this firmware, you will need:

* ST Visual Develop (STVD)
* Cosmic C compiler
* ST Visual Programmer (STVP)
* ST-Link debugger
* Target hardware

### Installing the Toolchain

In order to install the necessary toolchain (STVD, Cosmic compiler, STVP) the following guides are strongly recommended:

* https://www.instructables.com/Setup-ST-Visual-Develop-With-the-Cosmic-STM8-Compi/
* https://maker.pro/custom/tutorial/getting-started-with-stm8-microcontrollers

After installing the toolchain, download and import this project by opening the ```project.stw``` which is located in<br/>
```<PROJECT_ROOT>/nixie_watch_fw/STM8L15x-16x-05x-AL31-L_StdPeriph_Lib/Project/STM8L15x-16x-05x-AL31-L_StdPeriph_Lib/STVD/Cosmic/```

### Compiling Firmware

This project supports two hardware targets, the nixie watch hardware and STM8L breakout board. To build the breakout board FW, set the build profile in STVD to "Debug", for the watch hardware, use the "Release" build profile. In the code, any breakout board specific code snippets will be surrounded by the ```#ifdef STM8_BASEBAND``` compile guards. 

Hardware files for the supported targets are avaliable here:

* [Breakout board](https://github.com/emern/stm8l_hw_r10)
* [Nixie watch hardware](https://github.com/emern/nixie_watch_hw)

### Flashing/Debugging

The compiled binaries can be flashed using an ST-Link programmer with the STVP utility. If using the STM8 Breakout board, it is recommended to connect external STSP switches to ground on GPIOE pins 0, 1, 2, 3. The STM8 Breakout board also has USB host support, if desired it can be connected to a host PC and monitored via a terminal program such as [PuTTY](https://www.putty.org/).

## License Information

This project makes use of the STMicroelectronics Standard Peripheral Library and as such the enclosed firmware and related documentation are not covered by a License Agreement, if you need such License for the Peripheral Library you can contact your local STMicroelectronics office.
