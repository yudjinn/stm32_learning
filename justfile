build_dir := "build"
build_type := "Debug"
app := "led_toggle_addr"
elf := build_dir / app + ".elf"

default: build

# Configure CMake (Ninja) and refresh compile_commands.json for clangd/helix
configure:
    cmake -S . -B {{build_dir}} -G Ninja -DCMAKE_BUILD_TYPE={{build_type}} -DAPP={{app}}
    ln -sf {{build_dir}}/compile_commands.json compile_commands.json

# Alias for regenerating compile_commands.json only
compdb: configure

build: configure
    cmake --build {{build_dir}}

clean:
    rm -rf {{build_dir}} compile_commands.json

rebuild: clean build

# Flash over ST-LINK with OpenOCD
flash: build
    openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program {{elf}} verify reset exit"

# Flash over ST-LINK with stlink-tools (alternative)
flash-stlink: build
    st-flash --reset write {{build_dir}}/{{app}}.bin 0x08000000

# Start an OpenOCD GDB server on :3333
debug-server:
    openocd -f interface/stlink.cfg -f target/stm32f4x.cfg

# Connect GDB to the running debug-server
gdb: build
    arm-none-eabi-gdb {{elf}} -ex "target extended-remote :3333" -ex "monitor reset halt" -ex "load"

size: build
    arm-none-eabi-size {{elf}}
