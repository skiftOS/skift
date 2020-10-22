#pragma once

#include <libutils/Vector.h>

#include "kernel/devices/PCIDevice.h"
#include "kernel/memory/MMIO.h"

/* Utility macros */
#define N_ELEMENTS(arr) (sizeof(arr) / sizeof((arr)[0]))

#define SND_KNOB_MASTER 0
#define SND_KNOB_VENDOR 1024

/* BARs! */
#define AC97_NAMBAR 0x10  /* Native Audio Mixer Base Address Register */
#define AC97_NABMBAR 0x14 /* Native Audio Bus Mastering Base Address Register */

/* Bus mastering IO port offsets */
#define AC97_PO_BDBAR 0x10 /* PCM out buffer descriptor BAR */
#define AC97_PO_CIV 0x14   /* PCM out current index value */
#define AC97_PO_LVI 0x15   /* PCM out last valid index */
#define AC97_PO_SR 0x16    /* PCM out status register */
#define AC97_PO_PICB 0x18  /* PCM out position in current buffer register */
#define AC97_PO_CR 0x1B    /* PCM out control register */

/* Bus mastering misc */
/* Buffer descriptor list constants */
#define AC97_BDL_LEN 32                               /* Buffer descriptor list length */
#define AC97_BDL_BUFFER_LEN 0x1000                    /* Length of buffer in BDL */
#define AC97_CL_GET_LENGTH(cl) ((cl)&0xFFFF)          /* Decode length from cl */
#define AC97_CL_SET_LENGTH(cl, v) ((cl) = (v)&0xFFFF) /* Encode length to cl */
#define AC97_CL_BUP ((uint32_t)1 << 30)               /* Buffer underrun policy in cl */
#define AC97_CL_IOC ((uint32_t)1 << 31)               /* Interrupt on completion flag in cl */

/* PCM out control register flags */
#define AC97_X_CR_RPBM (1 << 0)  /* Run/pause bus master */
#define AC97_X_CR_RR (1 << 1)    /* Reset registers */
#define AC97_X_CR_LVBIE (1 << 2) /* Last valid buffer interrupt enable */
#define AC97_X_CR_FEIE (1 << 3)  /* FIFO error interrupt enable */
#define AC97_X_CR_IOCE (1 << 4)  /* Interrupt on completion enable */

/* Status register flags */
#define AC97_X_SR_DCH (1 << 0)   /* DMA controller halted */
#define AC97_X_SR_CELV (1 << 1)  /* Current equals last valid */
#define AC97_X_SR_LVBCI (1 << 2) /* Last valid buffer completion interrupt */
#define AC97_X_SR_BCIS (1 << 3)  /* Buffer completion interrupt status */
#define AC97_X_SR_FIFOE (1 << 4) /* FIFO error */

/* Mixer IO port offsets */
#define AC97_RESET 0x0000
#define AC97_MASTER_VOLUME 0x0002
#define AC97_AUX_OUT_VOLUME 0x0004
#define AC97_MONO_VOLUME 0x0006
#define AC97_PC_BEEP 0x000A
#define AC97_PCM_OUT_VOLUME 0x0018
#define AC97_EXT_AUDIO_ID 0x0028
#define AC97_EXT_AUDIO_STC 0x002A
#define AC97_FRONT_SPLRATE 0x002C
#define AC97_LR_SPLRATE 0x0032
// #define PORT_NABM_POBDBAR 0x0010
// #define PORT_NABM_POLVI 0x0015
// #define PORT_NABM_POCONTROL 0x001B
// #define PORT_NABM_GLB_CTRL_STAT 0x0060

/* snd values */
// #define AC97_SND_NAME "Intel AC'97"
// #define AC97_PLAYBACK_SPEED 48000
// #define AC97_PLAYBACK_FORMAT SND_FORMAT_L16SLE

// #define AC97_KNOB_PCM_OUT (SND_KNOB_VENDOR + 0)

// typedef struct snd_knob
// {
//     char name[256];
//     uint32_t id;
// } snd_knob_t;

// static snd_knob_t _knobs[] = {
//     {"Master",
//      SND_KNOB_MASTER},
//     {"PCM Out",
//      SND_KNOB_VENDOR + 0}};

struct __packed AC97BufferDescriptor
{
    uint32_t pointer;
    uint32_t cl;
    // void *buffer;
    // uint32_t length;
    // int reserved
};

class AC97 : public PCIDevice
{
private:
    uint16_t nabmbar;
    uint16_t nambar;
    size_t irq;
    uint8_t lvi;  // currently set last valid index ?
    uint8_t bits; // how many bits of volume are supported ?
    // AC97BufferDescriptor *bdl; // buffer descriptor list
    // uint16_t *bufs[AC97_BDL_LEN];

    // buffer descriptors range
    RefPtr<MMIORange> buffer_descriptors_range{};
    // buffer descriptor list of size 32 (MAX SIZE FOR AC97)
    AC97BufferDescriptor *buffer_descriptors_list;
    // 32*4096 buffer array for playing sound using PCM 16 Bit out (DMA)
    Vector<RefPtr<MMIORange>> buffers;

    uint32_t mask;

    // char *name[];

    uint16_t playback_speed;
    uint16_t playback_format;
    // snd_knob_t *knobs;

    uintptr_t curr_file_buffer_ptr;
    uint8_t playing;

    void initialise_buffers();

public:
    AC97(DeviceAddress address);
    ~AC97();

    void handle_interrupt() override;

    // bool can_write(FsHandle &handle) override;

    // bool can_read(FsHandle &handle) override;

    ResultOr<size_t> read(FsHandle &handle, void *buffer, size_t size) override;

    ResultOr<size_t> write(FsHandle &handle, const void *buffer, size_t size) override;

    Result call(FsHandle &handle, IOCall request, void *args) override;
};
