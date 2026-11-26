#include <stdio.h>
#include <unistd.h>
#include <sys/process.h>
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
    PadInfo pad_info;
    PadData pad_data;

    printf("\nLeonida PS3 Homebrew Proof of Concept\n");
    printf("Native PPU executable started.\n");
    printf("PSL1GHT / PS3DEV toolchain build.\n");
    printf("Press the PS button and choose Quit Game to exit.\n");
    fflush(stdout);

    sysUtilRegisterCallback(SYSUTIL_EVENT_SLOT0, on_system_event, NULL);
    ioPadInit(7);

    while (!exit_requested) {
        sysUtilCheckCallback();

        if (ioPadGetInfo(&pad_info) == 0 && pad_info.status[0]) {
            if (ioPadGetData(0, &pad_data) == 0 &&
                (pad_data.BTN_START || pad_data.BTN_CIRCLE)) {
                /* Do not exit on face/start buttons: keep the test harmless. */
            }
        }

        usleep(10000);
    }

    ioPadEnd();
    sysUtilUnregisterCallback(SYSUTIL_EVENT_SLOT0);
    printf("Leonida PS3 PoC closed cleanly.\n");
    fflush(stdout);
    return 0;
}
