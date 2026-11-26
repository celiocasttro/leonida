#include <stdio.h>
#include <unistd.h>
#include <sysutil/sysutil.h>
#include <io/pad.h>

static volatile int exit_requested = 0;

static void on_system_event(u64 status, u64 param, void *userdata)
{
    (void)param;
    (void)userdata;

    if (status == SYSUTIL_EXIT_GAME) {
        exit_requested = 1;
    }
}

int main(void)
{
    padInfo pad_info;
    padData pad_data;

    puts("Leonida PS3 Homebrew Proof of Concept");
    puts("Native Cell PPU executable started.");
    puts("PSL1GHT / PS3DEV toolchain build.");
    puts("Use the PS button > Quit Game to exit.");
    fflush(stdout);

    if (sysUtilRegisterCallback(SYSUTIL_EVENT_SLOT0, on_system_event, NULL) != SYSUTIL_OK) {
        puts("Warning: could not register the system callback.");
        fflush(stdout);
    }

    if (ioPadInit(7) != PAD_OK) {
        puts("Warning: controller input initialization failed.");
        fflush(stdout);
    }

    while (!exit_requested) {
        sysUtilCheckCallback();

        if (ioPadGetInfo(&pad_info) == PAD_OK && pad_info.status[0]) {
            /* Poll the controller to verify that the native pad API is available. */
            (void)ioPadGetData(0, &pad_data);
        }

        usleep(10000);
    }

    ioPadEnd();
    sysUtilUnregisterCallback(SYSUTIL_EVENT_SLOT0);
    puts("Leonida PS3 PoC closed cleanly.");
    fflush(stdout);
    return 0;
}
