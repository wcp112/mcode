#ifndef MCODE_HW_I80_H
#define MCODE_HW_I80_H

#include <stdint.h>

typedef void (*hw_i80_read_callback) (int length, const unsigned char *data);

void hw_i80_init (void);
void hw_i80_deinit (void);

void hw_i80_set_read_callback (hw_i80_read_callback aCallback);

void hw_i80_read (uint8_t cmd, uint8_t length);
void hw_i80_write (uint8_t cmd, uint8_t length, const uint8_t *data);
void hw_i80_write_P (uint8_t cmd, uint8_t length, const uint8_t *data);

void hw_i80_write_double (uint8_t cmd, uint8_t length, const uint8_t *data);
void hw_i80_write_double_P (uint8_t cmd, uint8_t length, const uint8_t *data);

void hw_i80_write_const_short (uint8_t cmd, uint16_t constValue, uint8_t length);
void hw_i80_write_const (uint8_t cmd, uint16_t constValue, uint16_t length);
void hw_i80_write_const_long (uint8_t cmd, uint16_t constValue, uint32_t length);

void hw_i80_reset (void);

#endif /* MC_CODE_I80_H */
