#include <dlfcn.h>
#include <jni.h>
#include "gdvm.h"

#include <sys/mman.h>
#include <android/log.h>

static void* libDvmHandle;

static void* getLibDvm() {
	if (!libDvmHandle) {
		libDvmHandle = dlopen("libdvm.so", RTLD_LAZY);
	}
	return libDvmHandle;
}

JNIEXPORT void JNICALL Java_net_zhuoweizhang_boardwalk_DalvikTweaks_setDefaultStackSize
  (JNIEnv *env, jclass clazz, jint size) {
	struct DvmGlobals* gDvm = dlsym(getLibDvm(), "gDvm");
	if (gDvm == NULL) return;
	gDvm->stackSize = size;
}

JNIEXPORT void JNICALL Java_net_zhuoweizhang_boardwalk_DalvikTweaks_crashTheLogger
  (JNIEnv *env, jclass clazz) {
	void* print = dlsym(RTLD_DEFAULT, "__android_log_print");
	int* printInt = (int*) print;
	__android_log_print(ANDROID_LOG_INFO, "Boardwalk", "Logger address: %x first bytes: %x", (int) print, printInt[0]);
	mprotect((void*) (((uintptr_t) print / 4096) * 4096), 4096, PROT_NONE);
}
