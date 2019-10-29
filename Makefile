.PHONY: all clean

CFLAGS = -I. -Wall -Wextra
CXXFLAGS = -I. -Wall -Wextra

all: demo/play_spc demo/benchmark demo/trim_spc demo/save_state

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
	$(CXX) -o $@ $^

demo/benchmark: $(BENCHMARK_OBJS)
	$(CXX) -o $@ $^

demo/trim_spc: $(TRIM_OBJS)
	$(CXX) -o $@ $^

demo/save_state: $(SAVE_OBJS)
	$(CXX) -o $@ $^

clean:
	rm -f demo/play_spc demo/benchmark demo/trim_spc demo/save_state $(PLAY_OBJS) $(BENCHMARK_OBJS) $(TRIM_OBJS) $(SAVE_OBJS)
