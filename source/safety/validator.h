#pragma once

#include "common.h"

// see: https://www.3dbrew.org/wiki/FIRM#Firmware_Section_Headers
typedef struct {
    u32 offset;
    u32 address;
    u32 size;
    u32 type;
    u8  hash[0x20];
} __attribute__((packed)) FirmSectionHeader;

// see: https://www.3dbrew.org/wiki/FIRM#FIRM_Header
typedef struct {
    u8  magic[4];
    u8  priority[4];
    u32 entry_arm11;
    u32 entry_arm9;
    u8  reserved1[0x30];
    FirmSectionHeader sections[4];
    u8  signature[0x100];
} __attribute__((packed, aligned(16))) FirmHeader;

u32 ValidateFirm(void* firm, u8* firm_sha, u32 firm_size, char* output);
u32 ValidateSector(void* sector);
u32 CheckFirmSigHax(void* firm);
u32 CheckFirmPayload(void* firm, char* result);
