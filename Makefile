.PHONY: all clean

CFLAGS = -I. -fPIC -Wall -Wextra -Werror
CXXFLAGS = -I. -fPIC -fno-exceptions -fno-rtti -Wall -Wextra -Werror

all: demo/play_spc demo/benchmark demo/trim_spc demo/save_state snes_spc/spc.a snes_spc/spc.so

SPC_OBJS = \
  snes_spc/spc.o \
  snes_spc/dsp.o \
  snes_spc/SPC_Filter.o \
  snes_spc/SNES_SPC.o \
  snes_spc/SNES_SPC_misc.o \
  snes_spc/SNES_SPC_state.o \
  snes_spc/SPC_DSP.o

BENCHMARK_OBJS = \
  demo/benchmark.o \
  demo/demo_util.o \
  $(SPC_OBJS)

TRIM_OBJS = \
  demo/trim_spc.o \
  demo/demo_util.o \
  $(SPC_OBJS)

SAVE_OBJS = \
  demo/save_state.o \
  demo/wave_writer.o \
  demo/demo_util.o \
  $(SPC_OBJS)

PLAY_OBJS = \
  demo/play_spc.o \
  demo/wave_writer.o \
  demo/demo_util.o \
  $(SPC_OBJS)

demo/play_spc: $(PLAY_OBJS)
	$(CC) -o $@ $^

demo/benchmark: $(BENCHMARK_OBJS)
	$(CC) -o $@ $^

demo/trim_spc: $(TRIM_OBJS)
	$(CC) -o $@ $^

demo/save_state: $(SAVE_OBJS)
	$(CC) -o $@ $^

snes_spc/spc.a: $(SPC_OBJS)
	$(AR) rcs $@ $^

snes_spc/spc.so: $(SPC_OBJS)
	$(CC) -shared -o $@ $^

clean:
	rm -f demo/play_spc demo/benchmark demo/trim_spc demo/save_state $(PLAY_OBJS) $(BENCHMARK_OBJS) $(TRIM_OBJS) $(SAVE_OBJS) snes_spc/spc.a snes_spc/spc.so
