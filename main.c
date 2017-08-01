#include <vitasdk.h>
#include <taihen.h>

#define HOOKS_NUM       2

static SceUID g_hooks[HOOKS_NUM];
static tai_hook_ref_t ref[HOOKS_NUM];
static uint8_t cur_hook = 0;

// Generic hooking function
void hookFunction(uint32_t nid, const void* func){
	g_hooks[cur_hook] = taiHookFunctionImport(&ref[cur_hook], TAI_MAIN_MODULE, TAI_ANY_LIBRARY, nid, func);
	cur_hook++;
}

int scePowerSetUsingWireless_patched(int enable) {
	return TAI_CONTINUE(int, ref[0], 1);
}

int scePowerSetConfigurationMode_patched(int mode) {
	return 0;
}


void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(SceSize argc, const void *args) {
		
	hookFunction(0x4D695C1F, scePowerSetUsingWireless_patched);
	hookFunction(0x3CE187B6, scePowerSetConfigurationMode_patched);
	
	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args) {
	
	// Freeing hooks
	int i;
	for (i = 0; i < HOOKS_NUM; i++){
		taiHookRelease(g_hooks[i], ref[i]);
	}

	return SCE_KERNEL_STOP_SUCCESS;
	
}