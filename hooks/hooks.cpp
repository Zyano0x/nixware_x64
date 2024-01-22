#include "hooks.h"

#include "../features/menu/menu.h"
#include "../features/visuals/visuals.h"

#include "handles/wndproc.cpp"
#include "handles/d3d9/present.cpp"
#include "handles/d3d9/reset.cpp"
#include "handles/html_panel/load_url.cpp"
#include "handles/html_panel/paint.cpp"
#include "handles/panel/paint_traverse.cpp"

void hooks::initialize()
{
    if (MH_Initialize() != MH_OK)
        throw;

    if (MH_CreateHook((PVOID)memory::pattern_scanner(xorstr("gameoverlayrenderer64.dll"), xorstr("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 54 41 56 41 57 48 81 EC ? ? ? ? 4C 8B A4 24 ? ? ? ?")), &handles::d3d9_present, (LPVOID*)&handles::originals::d3d9_present) != MH_OK)
        throw;

    if (MH_CreateHook((PVOID)memory::pattern_scanner(xorstr("gameoverlayrenderer64.dll"), xorstr("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 50 48 8B F2 48 8B F9 48 8B D1")), &handles::d3d9_reset, (LPVOID*)&handles::originals::d3d9_reset) != MH_OK)
        throw;

    if (MH_CreateHook((PVOID)memory::relative_to_absolute((uintptr_t)memory::pattern_scanner(xorstr("menusystem.dll"), xorstr("E8 ? ? ? ? EB 17 48 8B 0D ? ? ? ?")), 1, 5), &handles::html_panel_load_url, (LPVOID*)&handles::originals::html_panel_load_url) != MH_OK)
        throw;

    if (MH_CreateHook((PVOID)memory::pattern_scanner(xorstr("menusystem.dll"), xorstr("40 57 48 83 EC 40 48 8B F9")), &handles::html_panel_paint, (LPVOID*)&handles::originals::html_panel_paint) != MH_OK)
        throw;

	if (MH_CreateHook((PVOID)memory::get_virtual((PVOID**)interfaces::panel, 41), &handles::panel_paint_traverse, (LPVOID*)&handles::originals::panel_paint_traverse) != MH_OK)
		throw;

    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
        throw;
 
    handles::originals::wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(FindWindowW(L"Valve001", 0), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(handles::wndproc)));
}

void hooks::unhook()
{
    if (MH_DisableHook(MH_ALL_HOOKS) != MH_OK)
        throw;
}