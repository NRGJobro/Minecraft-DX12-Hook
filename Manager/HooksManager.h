#include "../dllmain.h"
class HookManager {
public:
    template <typename T>
    static void createHook(uintptr_t address, LPVOID detourFunction, T oFunc) { // Makes and enables the hook
        if (MH_CreateHook((LPVOID)address, (LPVOID)detourFunction, reinterpret_cast<LPVOID*>(oFunc)) == MH_OK)
            MH_EnableHook((void*)address);
    }
};