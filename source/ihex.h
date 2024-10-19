/*
 * ihex.h: 
 *
 */

#ifndef __IHEX_H
#define __IHEX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

/* -------------------------------------------------------------------------- */
/*                              Macro Definition                              */
/* -------------------------------------------------------------------------- */

#define IHEX_NEW_LINE               "\r\n"
#define IHEX_FRAME_START_CODE       ":"
#define IHEX_HEX_FRAME_BYTES_MAX    (16)

#define IHEX_FRAME_CHKSUM_BASE      (0x100)

/* -------------------------------------------------------------------------- */
/*                           Public Type Definition                           */
/* -------------------------------------------------------------------------- */

typedef enum ihex_frame_record_type
{
    IHEX_RECORD_DATA                = 0x00,
    IHEX_RECORD_EOF                 = 0X01,
    IHEX_RECORD_EXT_SEG_ADDR        = 0X02,
    IHEX_RECORD_START_SEG_ADDR      = 0X03,
    IHEX_RECORD_EXT_LINEAR_ADDR     = 0X04,
    IHEX_RECORD_START_LINEAR_ADDR   = 0X05,
}
ihex_frame_record_t;

typedef struct ihex_frame
{
    uint16_t    address;            /*  */
    uint8_t     record_type;        /*  */
    uint8_t     payload[0];         /*  */
}
ihex_frame_t;

typedef struct ihex
{
    FILE *fp;
    uint32_t ready  :  1;
    uint32_t addr   : 16;
    uint32_t        : 15;
    uint8_t *frame;
}
ihex_t;

/* -------------------------------------------------------------------------- */
/*                         Public Function Declaration                        */
/* -------------------------------------------------------------------------- */

ihex_t *ihex_create(const char *o_filepath);

int ihex_delete(ihex_t *ihex);

int ihex_write_frame(ihex_t *ihex, uint16_t addr, ihex_frame_record_t type, uint8_t *data, uint8_t size);

#ifdef __cplusplus
}
#endif

#endif /* __IHEX_H */