#ifndef MCODE_SCHEDULER_H
#define MCODE_SCHEDULER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This is a test scheduler, to be removed.
 */

typedef void (*mcode_cheduler_tick) (void);

void mcode_scheduler_init (void);
void mcode_scheduler_deinit (void);

void mcode_scheduler_start (void);
void mcode_scheduler_stop (void);

void mcode_scheduler_add (mcode_cheduler_tick tick);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCODE_SCHEDULER_H */
