## DirectX and Win32 API method injection and detouring on Windows platform ##

The hard time development project in my past company. All confidential parts are removed and rewritten.

## Project - overlay presenter ##

The project contains some namespaces for following purposes

- detour: Method detours for DirectX, kernel32 and user32 API references

- hook: The detoured method implementation

- injector: Exported API for process injection, and assembly image operation

## Project - content renderer ##

So far the project is a test/demo window for overlay presenter

## Notes for source code ##

Search `//TODO` or `//DEBUG` reminds me what should I do next

### Microsoft - Detours ###

The library and method injection is referenced from Microsoft research program [Detours](https://github.com/Microsoft/Detours), there has useful process related knowledge on Windows system.

Default place of **detours.h** file for this project is at *<repo root>*/detours/include/

Build static library file **detours.lib**, copy x64 arch to *<repo root>*/detours/lib.X64/ and x86 arch to *<repo root>*/detours/lib.X86/

Look into detour namespace and source files at overlay presenter project for more details.
