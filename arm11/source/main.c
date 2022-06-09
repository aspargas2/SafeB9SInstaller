/*
 *   This file is part of GodMode9
 *   Copyright (C) 2019 Wolfvak
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "common/common.h"
#include "common/types.h"
#include "common/arm.h"

#include "arm/gic.h"

#include "hw/gpulcd.h"
#include "hw/i2c.h"
#include "hw/mcu.h"

#include "system/sys.h"
#include "system/event.h"

void __attribute__((noreturn)) MainLoop(void)
{
	// initialize state stuff
	getEventIRQ()->reset();
	getEventMCU()->reset();

	// configure interrupts
	gicSetInterruptConfig(VBLANK_INTERRUPT, BIT(0), GIC_PRIO0, NULL);
	gicSetInterruptConfig(MCU_INTERRUPT, BIT(0), GIC_PRIO0, NULL);

	// enable interrupts
	gicEnableInterrupt(MCU_INTERRUPT);
	gicEnableInterrupt(VBLANK_INTERRUPT);

	// perform gpu init after initializing mcu but before
	// enabling the pxi system and the vblank handler
	GFX_init(GFX_BGR8);
	
	// broke stupid man's PXI
	while (*((u8*) 0x10163000) != 231);
	*((u8*) 0x10163001) = 231;

	// wait for the arm9 to tell us to jump to firm
	while (*((u8*) 0x10163000) != 37)
		ARM_WFI();

	gicDisableInterrupt(VBLANK_INTERRUPT);
	gicDisableInterrupt(MCU_INTERRUPT);

	SYS_CoreZeroShutdown();
	SYS_CoreShutdown();
}
