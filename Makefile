# Alternative GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug)
  ray_tracer_config = debug
  scene_generator_config = debug
  utils_config = debug
  stb_config = debug

else ifeq ($(config),release)
  ray_tracer_config = release
  scene_generator_config = release
  utils_config = release
  stb_config = release

else ifeq ($(config),dist)
  ray_tracer_config = dist
  scene_generator_config = dist
  utils_config = dist
  stb_config = dist

else
  $(error "invalid configuration $(config)")
endif

PROJECTS := ray_tracer scene_generator utils stb

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

ray_tracer: utils stb
ifneq (,$(ray_tracer_config))
	@echo "==== Building ray_tracer ($(ray_tracer_config)) ===="
	@${MAKE} --no-print-directory -C ray_tracer -f Makefile config=$(ray_tracer_config)
endif

scene_generator: utils
ifneq (,$(scene_generator_config))
	@echo "==== Building scene_generator ($(scene_generator_config)) ===="
	@${MAKE} --no-print-directory -C scene_generator -f Makefile config=$(scene_generator_config)
endif

utils:
ifneq (,$(utils_config))
	@echo "==== Building utils ($(utils_config)) ===="
	@${MAKE} --no-print-directory -C utils -f Makefile config=$(utils_config)
endif

stb:
ifneq (,$(stb_config))
	@echo "==== Building stb ($(stb_config)) ===="
	@${MAKE} --no-print-directory -C vendor/stb -f Makefile config=$(stb_config)
endif

clean:
	@${MAKE} --no-print-directory -C ray_tracer -f Makefile clean
	@${MAKE} --no-print-directory -C scene_generator -f Makefile clean
	@${MAKE} --no-print-directory -C utils -f Makefile clean
	@${MAKE} --no-print-directory -C vendor/stb -f Makefile clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo "  dist"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   ray_tracer"
	@echo "   scene_generator"
	@echo "   utils"
	@echo "   stb"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"