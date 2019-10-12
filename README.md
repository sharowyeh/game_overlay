## DirectX and Win32 API method injection and detouring on Windows platform ##

The hard time project developed for past company. All confidential parts are removed, and the rest are rewritten.

## External references ##

### Microsoft - Detours ###

The overlay presenter project referenced Microsoft research program [Detours](https://github.com/Microsoft/Detours) for Windows library injection.

Default place include file **detours.h** at *<repo root>*/detours/include/

Build static library file **detours.lib**, copy x64 arch to *<repo root>*/detours/lib.X64/ and x86 arch to *<repo root>*/detours/lib.X86/

Look into detour namespace and source files at overlay presenter project for more reference details.
