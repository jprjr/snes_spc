// snes_spc 0.9.0. http://www.slack.net/~ant/

#include "spc.h"

#include "SNES_SPC.h"
#include "SPC_Filter.h"

/* Copyright (C) 2004-2007 Shay Green. This module is free software; you
can redistribute it and/or modify it under the terms of the GNU Lesser
General Public License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version. This
module is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
details. You should have received a copy of the GNU Lesser General Public
License along with this module; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA */

#include "blargg_source.h"

snes_spc_t* spc_new( void )
{
	// be sure constants match
	assert( spc_sample_rate         == (int) SNES_SPC::sample_rate );
	assert( spc_rom_size            == (int) SNES_SPC::rom_size );
	assert( spc_clock_rate          == (int) SNES_SPC::clock_rate );
	assert( spc_clocks_per_sample   == (int) SNES_SPC::clocks_per_sample );
	assert( spc_port_count          == (int) SNES_SPC::port_count );
	assert( spc_voice_count         == (int) SNES_SPC::voice_count );
	assert( spc_tempo_unit          == (int) SNES_SPC::tempo_unit );
	assert( spc_file_size           == (int) SNES_SPC::spc_file_size );
	#if !SPC_NO_COPY_STATE_FUNCS
	assert( spc_state_size          == (int) SNES_SPC::state_size );
	#endif
	
	SNES_SPC* s = new SNES_SPC;
	if ( s && s->init() )
	{
		delete s;
		s = 0;
	}
	return s;
}

void spc_delete          ( snes_spc_t* s )                                { delete reinterpret_cast<SNES_SPC*>(s); }
void spc_init_rom        ( snes_spc_t* s, unsigned char const r [64] )    { reinterpret_cast<SNES_SPC*>(s)->init_rom( r ); }
void spc_set_output      ( snes_spc_t* s, spc_sample_t* p, int n )        { reinterpret_cast<SNES_SPC*>(s)->set_output( p, n ); }
int  spc_sample_count    ( snes_spc_t const* s )                          { return reinterpret_cast<const SNES_SPC*>(s)->sample_count(); }
void spc_reset           ( snes_spc_t* s )                                { reinterpret_cast<SNES_SPC*>(s)->reset(); }
void spc_soft_reset      ( snes_spc_t* s )                                { reinterpret_cast<SNES_SPC*>(s)->soft_reset(); }
int  spc_read_port       ( snes_spc_t* s, spc_time_t t, int p )           { return reinterpret_cast<SNES_SPC*>(s)->read_port( t, p ); }
void spc_write_port      ( snes_spc_t* s, spc_time_t t, int p, int d )    { reinterpret_cast<SNES_SPC*>(s)->write_port( t, p, d ); }
void spc_end_frame       ( snes_spc_t* s, spc_time_t t )                  { reinterpret_cast<SNES_SPC*>(s)->end_frame( t ); }  
void spc_mute_voices     ( snes_spc_t* s, int mask )                      { reinterpret_cast<SNES_SPC*>(s)->mute_voices( mask ); }
void spc_disable_surround( snes_spc_t* s, int disable )                   { reinterpret_cast<SNES_SPC*>(s)->disable_surround( disable ); }
void spc_set_tempo       ( snes_spc_t* s, int tempo )                     { reinterpret_cast<SNES_SPC*>(s)->set_tempo( tempo ); }
spc_err_t spc_load_spc   ( snes_spc_t* s, void const* p, long n )         { return reinterpret_cast<SNES_SPC*>(s)->load_spc( p, n ); }
void spc_clear_echo      ( snes_spc_t* s )                                { reinterpret_cast<SNES_SPC*>(s)->clear_echo(); }
spc_err_t spc_play       ( snes_spc_t* s, int count, short* out )         { return reinterpret_cast<SNES_SPC*>(s)->play( count, out ); }
spc_err_t spc_skip       ( snes_spc_t* s, int count )                     { return reinterpret_cast<SNES_SPC*>(s)->skip( count ); }
#if !SPC_NO_COPY_STATE_FUNCS
void spc_copy_state      ( snes_spc_t* s, unsigned char** p, spc_copy_func_t f ) { reinterpret_cast<SNES_SPC*>(s)->copy_state( p, f ); }
void spc_init_header     ( void* spc_out )                              { SNES_SPC::init_header( spc_out ); }
void spc_save_spc        ( snes_spc_t* s, void* spc_out )                 { reinterpret_cast<SNES_SPC*>(s)->save_spc( spc_out ); }
int  spc_check_kon       ( snes_spc_t* s )                                { return reinterpret_cast<SNES_SPC*>(s)->check_kon(); }
#endif

spc_filter_t* spc_filter_new( void )                              { return new SPC_Filter; }
void spc_filter_delete( spc_filter_t* f )                         { delete reinterpret_cast<SPC_Filter*>(f); }
void spc_filter_run( spc_filter_t* f, spc_sample_t* p, int s )    { reinterpret_cast<SPC_Filter*>(f)->run( p, s ); }
void spc_filter_clear( spc_filter_t* f )                          { reinterpret_cast<SPC_Filter*>(f)->clear(); }
void spc_filter_set_gain( spc_filter_t* f, int gain )             { reinterpret_cast<SPC_Filter*>(f)->set_gain( gain ); }
void spc_filter_set_bass( spc_filter_t* f, int bass )             { reinterpret_cast<SPC_Filter*>(f)->set_bass( bass ); }
