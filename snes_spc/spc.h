/* SNES SPC-700 APU emulator C interface (also usable from C++) */

/* snes_spc 0.9.0 */
#ifndef SPC_H
#define SPC_H

#include <stddef.h>

#ifdef __cplusplus
	extern "C" {
#endif

/* Error string return. NULL if success, otherwise error message. */
typedef const char* spc_err_t;

typedef void snes_spc_t;

/* Creates new SPC emulator. NULL if out of memory. */
snes_spc_t* spc_new( void );

/* Frees SPC emulator */
void spc_delete( snes_spc_t* );

/* Sample pairs generated per second */
enum { spc_sample_rate = 32000 };


/**** Emulator use ****/

/* Sets IPL ROM data. Library does not include ROM data. Most SPC music files
don't need ROM, but a full emulator must provide this. */
enum { spc_rom_size = 0x40 };
void spc_init_rom( snes_spc_t*, unsigned char const rom [spc_rom_size] );

/* Sets destination for output samples */
typedef short spc_sample_t;
void spc_set_output( snes_spc_t*, spc_sample_t* out, int out_size );

/* Number of samples written to output since last set */
int spc_sample_count( snes_spc_t const* );

/* Resets SPC to power-on state. This resets your output buffer, so you must
call spc_set_output() after this. */
void spc_reset( snes_spc_t* );

/* Emulates pressing reset switch on SNES. This resets your output buffer, so
you must call spc_set_output() after this. */
void spc_soft_reset( snes_spc_t* );

/* 1024000 SPC clocks per second, sample pair every 32 clocks */
typedef int spc_time_t;
enum { spc_clock_rate = 1024000 };
enum { spc_clocks_per_sample = 32 };

/* Reads/writes port at specified time */
enum { spc_port_count = 4 };
int  spc_read_port ( snes_spc_t*, spc_time_t, int port );
void spc_write_port( snes_spc_t*, spc_time_t, int port, int data );

/* Runs SPC to end_time and starts a new time frame at 0 */
void spc_end_frame( snes_spc_t*, spc_time_t end_time );


/**** Sound control ****/

/*Mutes voices corresponding to non-zero bits in mask. Reduces emulation accuracy. */
enum { spc_voice_count = 8 };
void spc_mute_voices( snes_spc_t*, int mask );

/* If true, prevents channels and global volumes from being phase-negated.
Only supported by fast DSP; has no effect on accurate DSP. */
void spc_disable_surround( snes_spc_t*, int disable );

/* Sets tempo, where spc_tempo_unit = normal, spc_tempo_unit / 2 = half speed, etc. */
enum { spc_tempo_unit = 0x100 };
void spc_set_tempo( snes_spc_t*, int );


/**** SPC music playback *****/

/* Loads SPC data into emulator. Returns NULL on success, otherwise error string. */
spc_err_t spc_load_spc( snes_spc_t*, void const* spc_in, long size );

/* Clears echo region. Useful after loading an SPC as many have garbage in echo. */
void spc_clear_echo( snes_spc_t* );

/* Plays for count samples and write samples to out. Discards samples if out
is NULL. Count must be a multiple of 2 since output is stereo. */
spc_err_t spc_play( snes_spc_t*, int count, short* out );

/* Skips count samples. Several times faster than spc_play(). */
spc_err_t spc_skip( snes_spc_t*, int count );


/**** State save/load (only available with accurate DSP) ****/

/* Saves/loads exact emulator state */
enum { spc_state_size = 67 * 1024L }; /* maximum space needed when saving */
typedef void (*spc_copy_func_t)( unsigned char** io, void* state, size_t );
void spc_copy_state( snes_spc_t*, unsigned char** io, spc_copy_func_t );

/* Writes minimal SPC file header to spc_out */
void spc_init_header( void* spc_out );

/* Saves emulator state as SPC file data. Writes spc_file_size bytes to spc_out.
Does not set up SPC header; use spc_init_header() for that. */
enum { spc_file_size = 0x10200 }; /* spc_out must have this many bytes allocated */
void spc_save_spc( snes_spc_t*, void* spc_out );

/* Returns non-zero if new key-on events occurred since last check. Useful for 
trimming silence while saving an SPC. */
int spc_check_kon( snes_spc_t* );


/**** spc_filter_t ****/

typedef void spc_filter_t;

/* Creates new filter. NULL if out of memory. */
spc_filter_t* spc_filter_new( void );

/* Frees filter */
void spc_filter_delete( spc_filter_t* );

/* Filters count samples of stereo sound in place. Count must be a multiple of 2. */
void spc_filter_run( spc_filter_t*, spc_sample_t* io, int count );

/* Clears filter to silence */
void spc_filter_clear( spc_filter_t* );

/* Sets gain (volume), where spc_filter_gain_unit is normal. Gains greater than
spc_filter_gain_unit are fine, since output is clamped to 16-bit sample range. */
enum { spc_filter_gain_unit = 0x100 };
void spc_filter_set_gain( spc_filter_t*, int gain );

/* Sets amount of bass (logarithmic scale) */
enum { spc_filter_bass_none =  0 };
enum { spc_filter_bass_norm =  8 }; /* normal amount */
enum { spc_filter_bass_max  = 31 };
void spc_filter_set_bass( spc_filter_t*, int bass );


#ifdef __cplusplus
	}
#endif

#endif
