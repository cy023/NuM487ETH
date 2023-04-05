# NuM487ETH

A GNU GCC Toolchain project template for Nuvoton M480 series MCU.

### Project Architecture
```
NuM487_GCC
|    Makefile
|    README.md
|    LICENSE
|    .gitignore
|
|────Core
|    |    main.c
|
|────Device_Startup
|    |    _syscall.c
|    |    gcc_arm.ld
|    |    startup_M480.S
|
|────Drivers
|    |────CMSIS
|    └────Library
|         |────Device
|         └────StdDriver
|
|────UnitTest
|         test.c
```

- `/Core`
    - `main.c`
    - User can add `user.c/user.h` in this folder.
- `/Device_Startup`
    - `_syscall.c` system call function. (provide by Nuvoton M480 BSP)
    - `gcc_arm_.ld` linker script. (provide by Nuvoton M480 BSP)
    - `startup_M480.S` startup file. (provide by Nuvoton M480 BSP)
- `/Drivers`
    - CMSIS (Common Microcontroller Software Interface Standard) , including ARM core related files. (provide by ARM)
    - Device, Register Layer. (provide by Nuvoton M480 BSP)
    - StdDriver, Hardware Abstract Layer. (provide by Nuvoton M480 BSP)
- `/UnitTest`
    - User can add test.c here. Every test.c will build separately (Every test.c should include its own main() function).

### How to use?
The following command can be used.

- `make`
    This command will build the sources file in `/Core`, `/Device_Startup`, `/Drivers`. The exact files can be set in Makefile variables `C_INCLUDES` and `C_INCLUDES`.
    - create `\build` folder, and the following output files will be put here.
    - compile the sources file separately.
    - link the depedency object files.
    - use the `arm-none-eabi-objcopy` to translate the ELF file to `.hex` or `.bin`.
- `make test`
    This command will build the sources file in `/UnitTest`, `/Device_Startup`, `/Drivers`. The exact files can be set in Makefile variables `C_INCLUDES` and `C_INCLUDES`.
- `make macro`
    This command will preprocess source files in `/Core`, `/UnitTest`, `/Device_Startup`, `/Drivers`, for debugging.
- `make size`
    This command will use the `arm-none-eabi-nm` tool to analyze the ELF file.
- `make clean`
    This command will clean all of the build files. (The `/build` folder)
