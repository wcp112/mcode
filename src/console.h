#ifndef MCODE_CONSOLE_H
#define MCODE_CONSOLE_H

#include <stdint.h>

void console_init (void);
void console_deinit (void);

void console_clear_screen (void);

void console_write_byte (uint8_t byte);
void console_write_string (const char *pString);
void console_write_string_P (const char *pString);

#endif /* MCODE_CONSOLE_H */
