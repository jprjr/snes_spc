.PHONY: all clean

CFLAGS = -I. -fPIC -Wall -Wextra -Werror
CXXFLAGS = -I. -fPIC -fno-exceptions -fno-rtti -Wall -Wextra -Werror

STATIC_PREFIX=lib
DYNLIB_PREFIX=lib
STATIC_EXT=.a
DYNLIB_EXT=.so

SPC_A = snes_spc/$(STATIC_PREFIX)spc$(STATIC_EXT)
SPC_SO = snes_spc/$(DYNLIB_PREFIX)spc$(DYNLIB_EXT)

all: demo/play_spc demo/benchmark demo/trim_spc demo/save_state $(SPC_A) $(SPC_SO)

SPC_CPP_SRCS = \
  snes_spc/SPC_Filter.cpp \
  snes_spc/SNES_SPC.cpp \
  snes_spc/SNES_SPC_misc.cpp \
  snes_spc/SNES_SPC_state.cpp \
  snes_spc/SPC_DSP.cpp \
  snes_spc/spc.cpp \
  snes_spc/dsp.cpp

SPC_CPP_OBJS = \
  snes_spc/SPC_Filter.o \
  snes_spc/SNES_SPC.o \
  snes_spc/SNES_SPC_misc.o \
  snes_spc/SNES_SPC_state.o \
  snes_spc/SPC_DSP.o \
  snes_spc/spc.o \
  snes_spc/dsp.o

UTIL_OBJS = demo/demo_util.o
WAVE_OBJS = demo/wave_writer.o
BENCHMARK_OBJS = demo/benchmark.o
TRIM_OBJS = demo/trim_spc.o
SAVE_OBJS = demo/save_state.o
PLAY_OBJS = demo/play_spc.o

demo/play_spc: $(PLAY_OBJS) $(UTIL_OBJS) $(WAVE_OBJS) $(SPC_A)
	$(CC) -o $@ $^

demo/benchmark: $(BENCHMARK_OBJS) $(UTIL_OBJS) $(SPC_A)
	$(CC) -o $@ $^

demo/trim_spc: $(TRIM_OBJS) $(UTIL_OBJS) $(SPC_A)
	$(CC) -o $@ $^

demo/save_state: $(SAVE_OBJS) $(UTIL_OBJS) $(WAVE_OBJS) $(SPC_A)
	$(CC) -o $@ $^

amal/spc.cpp: $(SPC_CPP_SRCS)
	mkdir -p amal
	perl aux/amalgate.pl "c:license.txt" $(SPC_CPP_SRCS) > $@

$(SPC_A): $(SPC_CPP_OBJS)
	$(AR) rcs $@ $^

$(SPC_SO): $(SPC_CPP_OBJS)
	$(CXX) -shared -o $@ $^

clean:
	rm -f demo/play_spc demo/benchmark demo/trim_spc demo/save_state $(UTIL_OBJS) $(WAVE_OBJS) $(PLAY_OBJS) $(BENCHMARK_OBJS) $(TRIM_OBJS) $(SAVE_OBJS) $(SPC_A) $(SPC_SO) amal/spc.cpp amal/*.o
