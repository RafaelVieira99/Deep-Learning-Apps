/*
 * system.h - SOPC Builder system and BSP software package information
 *
 * Machine generated for CPU 'cpu' in SOPC Builder design 'DE2_115_SD_CARD_NIOS'
 * SOPC Builder design path: ../../DE2_115_SD_CARD_NIOS.sopcinfo
 *
 * Generated: Tue Jan 18 23:23:17 GMT 2022
 */

/*
 * DO NOT MODIFY THIS FILE
 *
 * Changing this file will have subtle consequences
 * which will almost certainly lead to a nonfunctioning
 * system. If you do modify this file, be aware that your
 * changes will be overwritten and lost when this file
 * is generated again.
 *
 * DO NOT MODIFY THIS FILE
 */

/*
 * License Agreement
 *
 * Copyright (c) 2008
 * Altera Corporation, San Jose, California, USA.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This agreement shall be governed in all respects by the laws of the State
 * of California and by the laws of the United States of America.
 */

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

/* Include definitions from linker script generator */
#include "linker.h"


/*
 * CPU configuration
 *
 */

#define ALT_CPU_ARCHITECTURE "altera_nios2_gen2"
#define ALT_CPU_BIG_ENDIAN 0
#define ALT_CPU_BREAK_ADDR 0x02082820
#define ALT_CPU_CPU_ARCH_NIOS2_R1
#define ALT_CPU_CPU_FREQ 100000000u
#define ALT_CPU_CPU_ID_SIZE 1
#define ALT_CPU_CPU_ID_VALUE 0x00000000
#define ALT_CPU_CPU_IMPLEMENTATION "fast"
#define ALT_CPU_DATA_ADDR_WIDTH 0x1a
#define ALT_CPU_DCACHE_BYPASS_MASK 0x80000000
#define ALT_CPU_DCACHE_LINE_SIZE 32
#define ALT_CPU_DCACHE_LINE_SIZE_LOG2 5
#define ALT_CPU_DCACHE_SIZE 2048
#define ALT_CPU_EXCEPTION_ADDR 0x02040020
#define ALT_CPU_FLASH_ACCELERATOR_LINES 0
#define ALT_CPU_FLASH_ACCELERATOR_LINE_SIZE 0
#define ALT_CPU_FLUSHDA_SUPPORTED
#define ALT_CPU_FREQ 100000000
#define ALT_CPU_HARDWARE_DIVIDE_PRESENT 0
#define ALT_CPU_HARDWARE_MULTIPLY_PRESENT 1
#define ALT_CPU_HARDWARE_MULX_PRESENT 0
#define ALT_CPU_HAS_DEBUG_CORE 1
#define ALT_CPU_HAS_DEBUG_STUB
#define ALT_CPU_HAS_EXTRA_EXCEPTION_INFO
#define ALT_CPU_HAS_ILLEGAL_INSTRUCTION_EXCEPTION
#define ALT_CPU_HAS_JMPI_INSTRUCTION
#define ALT_CPU_ICACHE_LINE_SIZE 32
#define ALT_CPU_ICACHE_LINE_SIZE_LOG2 5
#define ALT_CPU_ICACHE_SIZE 4096
#define ALT_CPU_INITDA_SUPPORTED
#define ALT_CPU_INST_ADDR_WIDTH 0x1a
#define ALT_CPU_NAME "cpu"
#define ALT_CPU_NUM_OF_SHADOW_REG_SETS 0
#define ALT_CPU_OCI_VERSION 1
#define ALT_CPU_RESET_ADDR 0x02040000


/*
 * CPU configuration (with legacy prefix - don't use these anymore)
 *
 */

#define NIOS2_BIG_ENDIAN 0
#define NIOS2_BREAK_ADDR 0x02082820
#define NIOS2_CPU_ARCH_NIOS2_R1
#define NIOS2_CPU_FREQ 100000000u
#define NIOS2_CPU_ID_SIZE 1
#define NIOS2_CPU_ID_VALUE 0x00000000
#define NIOS2_CPU_IMPLEMENTATION "fast"
#define NIOS2_DATA_ADDR_WIDTH 0x1a
#define NIOS2_DCACHE_BYPASS_MASK 0x80000000
#define NIOS2_DCACHE_LINE_SIZE 32
#define NIOS2_DCACHE_LINE_SIZE_LOG2 5
#define NIOS2_DCACHE_SIZE 2048
#define NIOS2_EXCEPTION_ADDR 0x02040020
#define NIOS2_FLASH_ACCELERATOR_LINES 0
#define NIOS2_FLASH_ACCELERATOR_LINE_SIZE 0
#define NIOS2_FLUSHDA_SUPPORTED
#define NIOS2_HARDWARE_DIVIDE_PRESENT 0
#define NIOS2_HARDWARE_MULTIPLY_PRESENT 1
#define NIOS2_HARDWARE_MULX_PRESENT 0
#define NIOS2_HAS_DEBUG_CORE 1
#define NIOS2_HAS_DEBUG_STUB
#define NIOS2_HAS_EXTRA_EXCEPTION_INFO
#define NIOS2_HAS_ILLEGAL_INSTRUCTION_EXCEPTION
#define NIOS2_HAS_JMPI_INSTRUCTION
#define NIOS2_ICACHE_LINE_SIZE 32
#define NIOS2_ICACHE_LINE_SIZE_LOG2 5
#define NIOS2_ICACHE_SIZE 4096
#define NIOS2_INITDA_SUPPORTED
#define NIOS2_INST_ADDR_WIDTH 0x1a
#define NIOS2_NUM_OF_SHADOW_REG_SETS 0
#define NIOS2_OCI_VERSION 1
#define NIOS2_RESET_ADDR 0x02040000


/*
 * Define for each module class mastered by the CPU
 *
 */

#define __ALTERA_AVALON_JTAG_UART
#define __ALTERA_AVALON_ONCHIP_MEMORY2
#define __ALTERA_AVALON_PIO
#define __ALTERA_AVALON_TIMER
#define __ALTERA_GENERIC_TRISTATE_CONTROLLER
#define __ALTERA_NIOS2_GEN2
#define __ALTERA_UP_AVALON_VIDEO_CHARACTER_BUFFER_WITH_DMA
#define __ALTPLL


/*
 * System configuration
 *
 */

#define ALT_DEVICE_FAMILY "Cyclone IV E"
#define ALT_ENHANCED_INTERRUPT_API_PRESENT
#define ALT_IRQ_BASE NULL
#define ALT_LOG_PORT "/dev/null"
#define ALT_LOG_PORT_BASE 0x0
#define ALT_LOG_PORT_DEV null
#define ALT_LOG_PORT_TYPE ""
#define ALT_NUM_EXTERNAL_INTERRUPT_CONTROLLERS 0
#define ALT_NUM_INTERNAL_INTERRUPT_CONTROLLERS 1
#define ALT_NUM_INTERRUPT_CONTROLLERS 1
#define ALT_STDERR "/dev/jtag_uart"
#define ALT_STDERR_BASE 0x20830b0
#define ALT_STDERR_DEV jtag_uart
#define ALT_STDERR_IS_JTAG_UART
#define ALT_STDERR_PRESENT
#define ALT_STDERR_TYPE "altera_avalon_jtag_uart"
#define ALT_STDIN "/dev/jtag_uart"
#define ALT_STDIN_BASE 0x20830b0
#define ALT_STDIN_DEV jtag_uart
#define ALT_STDIN_IS_JTAG_UART
#define ALT_STDIN_PRESENT
#define ALT_STDIN_TYPE "altera_avalon_jtag_uart"
#define ALT_STDOUT "/dev/jtag_uart"
#define ALT_STDOUT_BASE 0x20830b0
#define ALT_STDOUT_DEV jtag_uart
#define ALT_STDOUT_IS_JTAG_UART
#define ALT_STDOUT_PRESENT
#define ALT_STDOUT_TYPE "altera_avalon_jtag_uart"
#define ALT_SYSTEM_NAME "DE2_115_SD_CARD_NIOS"


/*
 * activations_index configuration
 *
 */

#define ACTIVATIONS_INDEX_BASE 0x2083000
#define ACTIVATIONS_INDEX_BIT_CLEARING_EDGE_REGISTER 0
#define ACTIVATIONS_INDEX_BIT_MODIFYING_OUTPUT_REGISTER 0
#define ACTIVATIONS_INDEX_CAPTURE 0
#define ACTIVATIONS_INDEX_DATA_WIDTH 4
#define ACTIVATIONS_INDEX_DO_TEST_BENCH_WIRING 0
#define ACTIVATIONS_INDEX_DRIVEN_SIM_VALUE 0
#define ACTIVATIONS_INDEX_EDGE_TYPE "NONE"
#define ACTIVATIONS_INDEX_FREQ 100000000
#define ACTIVATIONS_INDEX_HAS_IN 0
#define ACTIVATIONS_INDEX_HAS_OUT 1
#define ACTIVATIONS_INDEX_HAS_TRI 0
#define ACTIVATIONS_INDEX_IRQ -1
#define ACTIVATIONS_INDEX_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ACTIVATIONS_INDEX_IRQ_TYPE "NONE"
#define ACTIVATIONS_INDEX_NAME "/dev/activations_index"
#define ACTIVATIONS_INDEX_RESET_VALUE 0
#define ACTIVATIONS_INDEX_SPAN 16
#define ACTIVATIONS_INDEX_TYPE "altera_avalon_pio"
#define ALT_MODULE_CLASS_activations_index altera_avalon_pio


/*
 * altpll configuration
 *
 */

#define ALTPLL_BASE 0x20830a0
#define ALTPLL_IRQ -1
#define ALTPLL_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ALTPLL_NAME "/dev/altpll"
#define ALTPLL_SPAN 16
#define ALTPLL_TYPE "altpll"
#define ALT_MODULE_CLASS_altpll altpll


/*
 * cfi_flash configuration
 *
 */

#define ALT_MODULE_CLASS_cfi_flash altera_generic_tristate_controller
#define CFI_FLASH_BASE 0x1800000
#define CFI_FLASH_HOLD_VALUE 60
#define CFI_FLASH_IRQ -1
#define CFI_FLASH_IRQ_INTERRUPT_CONTROLLER_ID -1
#define CFI_FLASH_NAME "/dev/cfi_flash"
#define CFI_FLASH_SETUP_VALUE 60
#define CFI_FLASH_SIZE 8388608u
#define CFI_FLASH_SPAN 8388608
#define CFI_FLASH_TIMING_UNITS "ns"
#define CFI_FLASH_TYPE "altera_generic_tristate_controller"
#define CFI_FLASH_WAIT_VALUE 160


/*
 * floatdata_output configuration
 *
 */

#define ALT_MODULE_CLASS_floatdata_output altera_avalon_pio
#define FLOATDATA_OUTPUT_BASE 0x2083050
#define FLOATDATA_OUTPUT_BIT_CLEARING_EDGE_REGISTER 0
#define FLOATDATA_OUTPUT_BIT_MODIFYING_OUTPUT_REGISTER 0
#define FLOATDATA_OUTPUT_CAPTURE 0
#define FLOATDATA_OUTPUT_DATA_WIDTH 32
#define FLOATDATA_OUTPUT_DO_TEST_BENCH_WIRING 0
#define FLOATDATA_OUTPUT_DRIVEN_SIM_VALUE 0
#define FLOATDATA_OUTPUT_EDGE_TYPE "NONE"
#define FLOATDATA_OUTPUT_FREQ 100000000
#define FLOATDATA_OUTPUT_HAS_IN 0
#define FLOATDATA_OUTPUT_HAS_OUT 1
#define FLOATDATA_OUTPUT_HAS_TRI 0
#define FLOATDATA_OUTPUT_IRQ -1
#define FLOATDATA_OUTPUT_IRQ_INTERRUPT_CONTROLLER_ID -1
#define FLOATDATA_OUTPUT_IRQ_TYPE "NONE"
#define FLOATDATA_OUTPUT_NAME "/dev/floatdata_output"
#define FLOATDATA_OUTPUT_RESET_VALUE 0
#define FLOATDATA_OUTPUT_SPAN 16
#define FLOATDATA_OUTPUT_TYPE "altera_avalon_pio"


/*
 * hal configuration
 *
 */

#define ALT_INCLUDE_INSTRUCTION_RELATED_EXCEPTION_API
#define ALT_MAX_FD 32
#define ALT_SYS_CLK none
#define ALT_TIMESTAMP_CLK TIMER


/*
 * jtag_uart configuration
 *
 */

#define ALT_MODULE_CLASS_jtag_uart altera_avalon_jtag_uart
#define JTAG_UART_BASE 0x20830b0
#define JTAG_UART_IRQ 0
#define JTAG_UART_IRQ_INTERRUPT_CONTROLLER_ID 0
#define JTAG_UART_NAME "/dev/jtag_uart"
#define JTAG_UART_READ_DEPTH 64
#define JTAG_UART_READ_THRESHOLD 8
#define JTAG_UART_SPAN 8
#define JTAG_UART_TYPE "altera_avalon_jtag_uart"
#define JTAG_UART_WRITE_DEPTH 64
#define JTAG_UART_WRITE_THRESHOLD 8


/*
 * key configuration
 *
 */

#define ALT_MODULE_CLASS_key altera_avalon_pio
#define KEY_BASE 0x2083060
#define KEY_BIT_CLEARING_EDGE_REGISTER 0
#define KEY_BIT_MODIFYING_OUTPUT_REGISTER 0
#define KEY_CAPTURE 1
#define KEY_DATA_WIDTH 4
#define KEY_DO_TEST_BENCH_WIRING 0
#define KEY_DRIVEN_SIM_VALUE 0
#define KEY_EDGE_TYPE "FALLING"
#define KEY_FREQ 100000000
#define KEY_HAS_IN 1
#define KEY_HAS_OUT 0
#define KEY_HAS_TRI 0
#define KEY_IRQ -1
#define KEY_IRQ_INTERRUPT_CONTROLLER_ID -1
#define KEY_IRQ_TYPE "NONE"
#define KEY_NAME "/dev/key"
#define KEY_RESET_VALUE 0
#define KEY_SPAN 16
#define KEY_TYPE "altera_avalon_pio"


/*
 * ledg configuration
 *
 */

#define ALT_MODULE_CLASS_ledg altera_avalon_pio
#define LEDG_BASE 0x2083080
#define LEDG_BIT_CLEARING_EDGE_REGISTER 0
#define LEDG_BIT_MODIFYING_OUTPUT_REGISTER 0
#define LEDG_CAPTURE 0
#define LEDG_DATA_WIDTH 9
#define LEDG_DO_TEST_BENCH_WIRING 0
#define LEDG_DRIVEN_SIM_VALUE 0
#define LEDG_EDGE_TYPE "NONE"
#define LEDG_FREQ 100000000
#define LEDG_HAS_IN 0
#define LEDG_HAS_OUT 1
#define LEDG_HAS_TRI 0
#define LEDG_IRQ -1
#define LEDG_IRQ_INTERRUPT_CONTROLLER_ID -1
#define LEDG_IRQ_TYPE "NONE"
#define LEDG_NAME "/dev/ledg"
#define LEDG_RESET_VALUE 0
#define LEDG_SPAN 16
#define LEDG_TYPE "altera_avalon_pio"


/*
 * ledr configuration
 *
 */

#define ALT_MODULE_CLASS_ledr altera_avalon_pio
#define LEDR_BASE 0x2083090
#define LEDR_BIT_CLEARING_EDGE_REGISTER 0
#define LEDR_BIT_MODIFYING_OUTPUT_REGISTER 0
#define LEDR_CAPTURE 0
#define LEDR_DATA_WIDTH 18
#define LEDR_DO_TEST_BENCH_WIRING 0
#define LEDR_DRIVEN_SIM_VALUE 0
#define LEDR_EDGE_TYPE "NONE"
#define LEDR_FREQ 100000000
#define LEDR_HAS_IN 0
#define LEDR_HAS_OUT 1
#define LEDR_HAS_TRI 0
#define LEDR_IRQ -1
#define LEDR_IRQ_INTERRUPT_CONTROLLER_ID -1
#define LEDR_IRQ_TYPE "NONE"
#define LEDR_NAME "/dev/ledr"
#define LEDR_RESET_VALUE 0
#define LEDR_SPAN 16
#define LEDR_TYPE "altera_avalon_pio"


/*
 * onchip_memory2 configuration
 *
 */

#define ALT_MODULE_CLASS_onchip_memory2 altera_avalon_onchip_memory2
#define ONCHIP_MEMORY2_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define ONCHIP_MEMORY2_ALLOW_MRAM_SIM_CONTENTS_ONLY_FILE 0
#define ONCHIP_MEMORY2_BASE 0x2040000
#define ONCHIP_MEMORY2_CONTENTS_INFO ""
#define ONCHIP_MEMORY2_DUAL_PORT 0
#define ONCHIP_MEMORY2_GUI_RAM_BLOCK_TYPE "AUTO"
#define ONCHIP_MEMORY2_INIT_CONTENTS_FILE "DE2_115_SD_CARD_NIOS_onchip_memory2"
#define ONCHIP_MEMORY2_INIT_MEM_CONTENT 1
#define ONCHIP_MEMORY2_INSTANCE_ID "NONE"
#define ONCHIP_MEMORY2_IRQ -1
#define ONCHIP_MEMORY2_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ONCHIP_MEMORY2_NAME "/dev/onchip_memory2"
#define ONCHIP_MEMORY2_NON_DEFAULT_INIT_FILE_ENABLED 0
#define ONCHIP_MEMORY2_RAM_BLOCK_TYPE "AUTO"
#define ONCHIP_MEMORY2_READ_DURING_WRITE_MODE "DONT_CARE"
#define ONCHIP_MEMORY2_SINGLE_CLOCK_OP 0
#define ONCHIP_MEMORY2_SIZE_MULTIPLE 1
#define ONCHIP_MEMORY2_SIZE_VALUE 262144
#define ONCHIP_MEMORY2_SPAN 262144
#define ONCHIP_MEMORY2_TYPE "altera_avalon_onchip_memory2"
#define ONCHIP_MEMORY2_WRITABLE 1


/*
 * pixel_index configuration
 *
 */

#define ALT_MODULE_CLASS_pixel_index altera_avalon_pio
#define PIXEL_INDEX_BASE 0x2083030
#define PIXEL_INDEX_BIT_CLEARING_EDGE_REGISTER 0
#define PIXEL_INDEX_BIT_MODIFYING_OUTPUT_REGISTER 0
#define PIXEL_INDEX_CAPTURE 0
#define PIXEL_INDEX_DATA_WIDTH 10
#define PIXEL_INDEX_DO_TEST_BENCH_WIRING 0
#define PIXEL_INDEX_DRIVEN_SIM_VALUE 0
#define PIXEL_INDEX_EDGE_TYPE "NONE"
#define PIXEL_INDEX_FREQ 100000000
#define PIXEL_INDEX_HAS_IN 0
#define PIXEL_INDEX_HAS_OUT 1
#define PIXEL_INDEX_HAS_TRI 0
#define PIXEL_INDEX_IRQ -1
#define PIXEL_INDEX_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PIXEL_INDEX_IRQ_TYPE "NONE"
#define PIXEL_INDEX_NAME "/dev/pixel_index"
#define PIXEL_INDEX_RESET_VALUE 0
#define PIXEL_INDEX_SPAN 16
#define PIXEL_INDEX_TYPE "altera_avalon_pio"


/*
 * results_input configuration
 *
 */

#define ALT_MODULE_CLASS_results_input altera_avalon_pio
#define RESULTS_INPUT_BASE 0x2083010
#define RESULTS_INPUT_BIT_CLEARING_EDGE_REGISTER 0
#define RESULTS_INPUT_BIT_MODIFYING_OUTPUT_REGISTER 0
#define RESULTS_INPUT_CAPTURE 0
#define RESULTS_INPUT_DATA_WIDTH 32
#define RESULTS_INPUT_DO_TEST_BENCH_WIRING 0
#define RESULTS_INPUT_DRIVEN_SIM_VALUE 0
#define RESULTS_INPUT_EDGE_TYPE "NONE"
#define RESULTS_INPUT_FREQ 100000000
#define RESULTS_INPUT_HAS_IN 1
#define RESULTS_INPUT_HAS_OUT 0
#define RESULTS_INPUT_HAS_TRI 0
#define RESULTS_INPUT_IRQ -1
#define RESULTS_INPUT_IRQ_INTERRUPT_CONTROLLER_ID -1
#define RESULTS_INPUT_IRQ_TYPE "NONE"
#define RESULTS_INPUT_NAME "/dev/results_input"
#define RESULTS_INPUT_RESET_VALUE 0
#define RESULTS_INPUT_SPAN 16
#define RESULTS_INPUT_TYPE "altera_avalon_pio"


/*
 * sd_clk configuration
 *
 */

#define ALT_MODULE_CLASS_sd_clk altera_avalon_pio
#define SD_CLK_BASE 0x50
#define SD_CLK_BIT_CLEARING_EDGE_REGISTER 0
#define SD_CLK_BIT_MODIFYING_OUTPUT_REGISTER 0
#define SD_CLK_CAPTURE 0
#define SD_CLK_DATA_WIDTH 1
#define SD_CLK_DO_TEST_BENCH_WIRING 0
#define SD_CLK_DRIVEN_SIM_VALUE 0
#define SD_CLK_EDGE_TYPE "NONE"
#define SD_CLK_FREQ 100000000
#define SD_CLK_HAS_IN 0
#define SD_CLK_HAS_OUT 1
#define SD_CLK_HAS_TRI 0
#define SD_CLK_IRQ -1
#define SD_CLK_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SD_CLK_IRQ_TYPE "NONE"
#define SD_CLK_NAME "/dev/sd_clk"
#define SD_CLK_RESET_VALUE 0
#define SD_CLK_SPAN 16
#define SD_CLK_TYPE "altera_avalon_pio"


/*
 * sd_cmd configuration
 *
 */

#define ALT_MODULE_CLASS_sd_cmd altera_avalon_pio
#define SD_CMD_BASE 0x40
#define SD_CMD_BIT_CLEARING_EDGE_REGISTER 0
#define SD_CMD_BIT_MODIFYING_OUTPUT_REGISTER 0
#define SD_CMD_CAPTURE 0
#define SD_CMD_DATA_WIDTH 1
#define SD_CMD_DO_TEST_BENCH_WIRING 0
#define SD_CMD_DRIVEN_SIM_VALUE 0
#define SD_CMD_EDGE_TYPE "NONE"
#define SD_CMD_FREQ 100000000
#define SD_CMD_HAS_IN 0
#define SD_CMD_HAS_OUT 0
#define SD_CMD_HAS_TRI 1
#define SD_CMD_IRQ -1
#define SD_CMD_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SD_CMD_IRQ_TYPE "NONE"
#define SD_CMD_NAME "/dev/sd_cmd"
#define SD_CMD_RESET_VALUE 0
#define SD_CMD_SPAN 16
#define SD_CMD_TYPE "altera_avalon_pio"


/*
 * sd_dat configuration
 *
 */

#define ALT_MODULE_CLASS_sd_dat altera_avalon_pio
#define SD_DAT_BASE 0x30
#define SD_DAT_BIT_CLEARING_EDGE_REGISTER 0
#define SD_DAT_BIT_MODIFYING_OUTPUT_REGISTER 0
#define SD_DAT_CAPTURE 0
#define SD_DAT_DATA_WIDTH 4
#define SD_DAT_DO_TEST_BENCH_WIRING 0
#define SD_DAT_DRIVEN_SIM_VALUE 0
#define SD_DAT_EDGE_TYPE "NONE"
#define SD_DAT_FREQ 100000000
#define SD_DAT_HAS_IN 0
#define SD_DAT_HAS_OUT 0
#define SD_DAT_HAS_TRI 1
#define SD_DAT_IRQ -1
#define SD_DAT_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SD_DAT_IRQ_TYPE "NONE"
#define SD_DAT_NAME "/dev/sd_dat"
#define SD_DAT_RESET_VALUE 0
#define SD_DAT_SPAN 16
#define SD_DAT_TYPE "altera_avalon_pio"


/*
 * sd_wp_n configuration
 *
 */

#define ALT_MODULE_CLASS_sd_wp_n altera_avalon_pio
#define SD_WP_N_BASE 0x20
#define SD_WP_N_BIT_CLEARING_EDGE_REGISTER 0
#define SD_WP_N_BIT_MODIFYING_OUTPUT_REGISTER 0
#define SD_WP_N_CAPTURE 0
#define SD_WP_N_DATA_WIDTH 1
#define SD_WP_N_DO_TEST_BENCH_WIRING 0
#define SD_WP_N_DRIVEN_SIM_VALUE 0
#define SD_WP_N_EDGE_TYPE "NONE"
#define SD_WP_N_FREQ 100000000
#define SD_WP_N_HAS_IN 1
#define SD_WP_N_HAS_OUT 0
#define SD_WP_N_HAS_TRI 0
#define SD_WP_N_IRQ -1
#define SD_WP_N_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SD_WP_N_IRQ_TYPE "NONE"
#define SD_WP_N_NAME "/dev/sd_wp_n"
#define SD_WP_N_RESET_VALUE 0
#define SD_WP_N_SPAN 16
#define SD_WP_N_TYPE "altera_avalon_pio"


/*
 * sw configuration
 *
 */

#define ALT_MODULE_CLASS_sw altera_avalon_pio
#define SW_BASE 0x2083070
#define SW_BIT_CLEARING_EDGE_REGISTER 0
#define SW_BIT_MODIFYING_OUTPUT_REGISTER 0
#define SW_CAPTURE 1
#define SW_DATA_WIDTH 18
#define SW_DO_TEST_BENCH_WIRING 0
#define SW_DRIVEN_SIM_VALUE 0
#define SW_EDGE_TYPE "FALLING"
#define SW_FREQ 100000000
#define SW_HAS_IN 1
#define SW_HAS_OUT 0
#define SW_HAS_TRI 0
#define SW_IRQ -1
#define SW_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SW_IRQ_TYPE "NONE"
#define SW_NAME "/dev/sw"
#define SW_RESET_VALUE 0
#define SW_SPAN 16
#define SW_TYPE "altera_avalon_pio"


/*
 * sync_data configuration
 *
 */

#define ALT_MODULE_CLASS_sync_data altera_avalon_pio
#define SYNC_DATA_BASE 0x2083040
#define SYNC_DATA_BIT_CLEARING_EDGE_REGISTER 0
#define SYNC_DATA_BIT_MODIFYING_OUTPUT_REGISTER 0
#define SYNC_DATA_CAPTURE 0
#define SYNC_DATA_DATA_WIDTH 16
#define SYNC_DATA_DO_TEST_BENCH_WIRING 0
#define SYNC_DATA_DRIVEN_SIM_VALUE 0
#define SYNC_DATA_EDGE_TYPE "NONE"
#define SYNC_DATA_FREQ 100000000
#define SYNC_DATA_HAS_IN 0
#define SYNC_DATA_HAS_OUT 1
#define SYNC_DATA_HAS_TRI 0
#define SYNC_DATA_IRQ -1
#define SYNC_DATA_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SYNC_DATA_IRQ_TYPE "NONE"
#define SYNC_DATA_NAME "/dev/sync_data"
#define SYNC_DATA_RESET_VALUE 0
#define SYNC_DATA_SPAN 16
#define SYNC_DATA_TYPE "altera_avalon_pio"


/*
 * timer configuration
 *
 */

#define ALT_MODULE_CLASS_timer altera_avalon_timer
#define TIMER_ALWAYS_RUN 0
#define TIMER_BASE 0x0
#define TIMER_COUNTER_SIZE 32
#define TIMER_FIXED_PERIOD 0
#define TIMER_FREQ 100000000
#define TIMER_IRQ 4
#define TIMER_IRQ_INTERRUPT_CONTROLLER_ID 0
#define TIMER_LOAD_VALUE 99999
#define TIMER_MULT 0.001
#define TIMER_NAME "/dev/timer"
#define TIMER_PERIOD 1
#define TIMER_PERIOD_UNITS "ms"
#define TIMER_RESET_OUTPUT 0
#define TIMER_SNAPSHOT 1
#define TIMER_SPAN 32
#define TIMER_TICKS_PER_SEC 1000
#define TIMER_TIMEOUT_PULSE_OUTPUT 0
#define TIMER_TYPE "altera_avalon_timer"


/*
 * video_character_buffer_with_dma_0_avalon_char_buffer_slave configuration
 *
 */

#define ALT_MODULE_CLASS_video_character_buffer_with_dma_0_avalon_char_buffer_slave altera_up_avalon_video_character_buffer_with_dma
#define VIDEO_CHARACTER_BUFFER_WITH_DMA_0_AVALON_CHAR_BUFFER_SLAVE_BASE 0x2080000
#define VIDEO_CHARACTER_BUFFER_WITH_DMA_0_AVALON_CHAR_BUFFER_SLAVE_IRQ -1
#define VIDEO_CHARACTER_BUFFER_WITH_DMA_0_AVALON_CHAR_BUFFER_SLAVE_IRQ_INTERRUPT_CONTROLLER_ID -1
#define VIDEO_CHARACTER_BUFFER_WITH_DMA_0_AVALON_CHAR_BUFFER_SLAVE_NAME "/dev/video_character_buffer_with_dma_0_avalon_char_buffer_slave"
#define VIDEO_CHARACTER_BUFFER_WITH_DMA_0_AVALON_CHAR_BUFFER_SLAVE_SPAN 8192
#define VIDEO_CHARACTER_BUFFER_WITH_DMA_0_AVALON_CHAR_BUFFER_SLAVE_TYPE "altera_up_avalon_video_character_buffer_with_dma"


/*
 * video_character_buffer_with_dma_0_avalon_char_control_slave configuration
 *
 */

#define ALT_MODULE_CLASS_video_character_buffer_with_dma_0_avalon_char_control_slave altera_up_avalon_video_character_buffer_with_dma
#define VIDEO_CHARACTER_BUFFER_WITH_DMA_0_AVALON_CHAR_CONTROL_SLAVE_BASE 0x20830b8
#define VIDEO_CHARACTER_BUFFER_WITH_DMA_0_AVALON_CHAR_CONTROL_SLAVE_IRQ -1
#define VIDEO_CHARACTER_BUFFER_WITH_DMA_0_AVALON_CHAR_CONTROL_SLAVE_IRQ_INTERRUPT_CONTROLLER_ID -1
#define VIDEO_CHARACTER_BUFFER_WITH_DMA_0_AVALON_CHAR_CONTROL_SLAVE_NAME "/dev/video_character_buffer_with_dma_0_avalon_char_control_slave"
#define VIDEO_CHARACTER_BUFFER_WITH_DMA_0_AVALON_CHAR_CONTROL_SLAVE_SPAN 8
#define VIDEO_CHARACTER_BUFFER_WITH_DMA_0_AVALON_CHAR_CONTROL_SLAVE_TYPE "altera_up_avalon_video_character_buffer_with_dma"


/*
 * weight_index configuration
 *
 */

#define ALT_MODULE_CLASS_weight_index altera_avalon_pio
#define WEIGHT_INDEX_BASE 0x2083020
#define WEIGHT_INDEX_BIT_CLEARING_EDGE_REGISTER 0
#define WEIGHT_INDEX_BIT_MODIFYING_OUTPUT_REGISTER 0
#define WEIGHT_INDEX_CAPTURE 0
#define WEIGHT_INDEX_DATA_WIDTH 13
#define WEIGHT_INDEX_DO_TEST_BENCH_WIRING 0
#define WEIGHT_INDEX_DRIVEN_SIM_VALUE 0
#define WEIGHT_INDEX_EDGE_TYPE "NONE"
#define WEIGHT_INDEX_FREQ 100000000
#define WEIGHT_INDEX_HAS_IN 0
#define WEIGHT_INDEX_HAS_OUT 1
#define WEIGHT_INDEX_HAS_TRI 0
#define WEIGHT_INDEX_IRQ -1
#define WEIGHT_INDEX_IRQ_INTERRUPT_CONTROLLER_ID -1
#define WEIGHT_INDEX_IRQ_TYPE "NONE"
#define WEIGHT_INDEX_NAME "/dev/weight_index"
#define WEIGHT_INDEX_RESET_VALUE 0
#define WEIGHT_INDEX_SPAN 16
#define WEIGHT_INDEX_TYPE "altera_avalon_pio"

#endif /* __SYSTEM_H_ */
