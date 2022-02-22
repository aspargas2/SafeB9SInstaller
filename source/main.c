#include "installer.h"
#include "ui.h"
#include "i2c.h"
#include "qff.h"
#include "vram.h"


void Reboot()
{
    i2cWriteRegister(I2C_DEV_MCU, 0x20, 1 << 2);
    while(true);
}

void Poweroff()
{
    i2cWriteRegister(I2C_DEV_MCU, 0x20, 1 << 0);
    while(true);
}


u8 *top_screen, *bottom_screen;

void main(/*int argc, char** argv*/)
{
    top_screen = (u8*) VRAM_TOP_LA;
    bottom_screen = (u8*) VRAM_BOT_A;

    ClearScreenF(true, true, COLOR_STD_BG);
    u32 ret = SafeB9SInstaller();
    ShowInstallerStatus(); // update installer status one last time
    fs_deinit();
    if (ret) { 
        ShowPrompt(false, "SigHaxed FIRM was not installed!\nCheck lower screen for info.\n \nIf you don't know what's going on,\ngo to the link on the bottom screen for assistance.");
        Poweroff();
    }
    ClearScreenF(true, true, COLOR_STD_BG);
    Reboot();
}
