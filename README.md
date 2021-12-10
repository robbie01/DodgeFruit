# DodgeFruit SDL

This isn't really a README as much as it is the current state of the project.

- Emscripten works. This required vast restructuring to achieve, but it paid off, mostly.
  - Use `mkdir buildem; cd buildem; emcmake cmake -G Ninja ..; ninja`
    - You don't *have* to use Ninja, I just think it's cool.
- OpenBSD works. Compile as normal.
- Cross-compiling to Windows works. Use `mkdir buildwin; cd buildwin; cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=/usr/share/mingw/toolchain-mingw64.cmake ..; ninja`
  - I currently have no way of testing whether this compiles with MinGW under Windows, or if the output even runs outside of WINE at all.
- A streaming texture is now used instead of GetWindowSurface to maximize compatibility with Emscripten.
  - When using GetWindowSurface, the colors weren't consistent between native and Emscripten. This makes sense, as SDL surfaces weren't made for portability.
  - For some reason, changing screens is now a slow operation, even though the gameplay itself is still decent.
    - Even weirder, it's faster under MinGW/WINE than Linux native. This world is mad.

