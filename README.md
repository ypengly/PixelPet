# PixelPet (Phase 1)
Build (Windows 10/11, Visual Studio 2022 or MinGW, CMake 3.24+):

    cmake -S . -B build
    cmake --build build --config Release
    build/Release/PixelPet.exe

Phase 1 scope: transparent borderless always-on-top window, procedural placeholder pet
(`Pet/PetRenderer` is the only file to swap when real sprite sheets arrive), idle/walk,
blink, breathing, drag & drop. Right-click quits for now.
