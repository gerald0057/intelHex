/*
 * ihex_write.c: 
 *
 */

#include <assert.h>
#include <string.h>
#include <ihex.h>

#define MIN(a,b) (a) > (b) ? (b) : (a)

int ihex_hex_write(FILE *fp, uint8_t *data, uint8_t size, uint16_t addr, ihex_frame_record_t type)
{
    uint32_t chksum;

    assert(fp);

    chksum = IHEX_FRAME_CHKSUM_BASE;
    chksum -= size;
    chksum -= (addr >> 0U) & 0xFF;
    chksum -= (addr >> 8U) & 0xFF;
    chksum -= type & 0xFF;

    fprintf(fp, IHEX_FRAME_START_CODE "%02X%04X%02X", size, addr, type);
    for (int i = 0 ; i < size; ++i)
    {
        chksum -= data[i];
        fprintf(fp, "%02X", data[i]);
    }
    fprintf(fp, "%02X", chksum & 0xFF);
    fprintf(fp, IHEX_NEW_LINE);

    return size;
}

int ihex_hex_write_address(FILE *fp, uint16_t addr)
{
    uint8_t addr_u8[2];

    addr_u8[0] = (addr >> 8U) & 0xFF;
    addr_u8[1] = (addr >> 0U) & 0xFF;

    return ihex_hex_write(fp, addr_u8, sizeof(addr_u8), 0, IHEX_RECORD_EXT_LINEAR_ADDR);
}

int ihex_hex_write_eof(FILE *fp)
{
    return ihex_hex_write(fp, NULL, 0, 0, IHEX_RECORD_EOF);
}

int ihex_bin2hex(FILE *fp_bin, FILE *fp_hex, uint32_t offset)
{
    uint32_t rd2end;
    uint16_t address;
    uint8_t rdsz;
    uint8_t buffer[IHEX_HEX_FRAME_BYTES_MAX];

    assert(fp_bin);
    assert(fp_hex);

    address = offset & 0xFFFF;
    offset = offset >> 16U;

    if (offset)
    {
        goto __START;
    }

    while (1)
    {
        rd2end = 0x10000 - address;
        if ((rdsz = fread(buffer, 1, MIN(rd2end, sizeof(buffer)), fp_bin)) <= 0)
        {
            break;
        }

        ihex_hex_write(fp_hex, buffer, rdsz, address, IHEX_RECORD_DATA);
        address += rdsz;

        if (rd2end <= sizeof(buffer))
        {
            address = 0U;
__START:
            ihex_hex_write_address(fp_hex, offset++);
        }
    }

    return ihex_hex_write_eof(fp_hex);
}

int main(void)
{
    FILE *fp_bin = fopen("rtt.bin", "rb");
    FILE *fp_hex = fopen("rtt.hex", "w");

    ihex_bin2hex(fp_bin, fp_hex, 0x08020011);

    return 0;
}
