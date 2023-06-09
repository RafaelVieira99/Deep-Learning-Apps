--------------------------------------------------------------------------------
--
--   FileName:         lcd_example.vhd
--   Dependencies:     none
--   Design Software:  Quartus II 32-bit Version 11.1 Build 173 SJ Full Version
--
--   HDL CODE IS PROVIDED "AS IS."  DIGI-KEY EXPRESSLY DISCLAIMS ANY
--   WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT
--   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
--   PARTICULAR PURPOSE, OR NON-INFRINGEMENT. IN NO EVENT SHALL DIGI-KEY
--   BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
--   DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF
--   PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
--   BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
--   ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS.
--
--   Version History
--   Version 1.0 6/13/2012 Scott Larson
--     Initial Public Release
--
--   Prints "123456789" on a HD44780 compatible 8-bit interface character LCD 
--   module using the lcd_controller.vhd component.
--
--------------------------------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE  IEEE.STD_LOGIC_ARITH.all;
USE  IEEE.STD_LOGIC_UNSIGNED.all;
ENTITY LCDNOMES IS
  PORT(
      clk       : IN  STD_LOGIC;  --system clock
      reset     : IN  STD_LOGIC;-- RESET
      rw, rs, e : OUT STD_LOGIC;  --read/write, setup/data, and enable for lcd
      lcd_data  : OUT STD_LOGIC_VECTOR(7 DOWNTO 0)); --data signals for lcd
END LCDNOMES;

ARCHITECTURE behavior OF LCDNOMES IS
  SIGNAL   lcd_enable : STD_LOGIC;
  SIGNAL   lcd_bus    : STD_LOGIC_VECTOR(9 DOWNTO 0);
  SIGNAL   lcd_busy   : STD_LOGIC;
  COMPONENT ze_controlador IS
    PORT(
       clk        : IN  STD_LOGIC; --system clock
       reset_n    : IN  STD_LOGIC; --active low reinitializes lcd
       lcd_enable : IN  STD_LOGIC; --latches data into lcd controller
       lcd_bus    : IN  STD_LOGIC_VECTOR(9 DOWNTO 0); --data and control signals
       busy       : OUT STD_LOGIC; --lcd controller busy/idle feedback
       rw, rs, e  : OUT STD_LOGIC; --read/write, setup/data, and enable for lcd
       lcd_data   : OUT STD_LOGIC_VECTOR(7 DOWNTO 0)); --data signals for lcd

  END COMPONENT;
BEGIN

  --instantiate the lcd controller
  dut: ze_controlador
    PORT MAP(clk => clk, reset_n => reset, lcd_enable => lcd_enable, lcd_bus => lcd_bus, 
             busy => lcd_busy, rw => rw, rs => rs, e => e, lcd_data => lcd_data);

  PROCESS(clk, reset)
    VARIABLE char  :  INTEGER RANGE 0 TO 32 := 0;
  BEGIN
    IF reset = '0' THEN
			char:=0;
			
    ELSIF(clk'EVENT AND clk = '1') THEN
      IF(lcd_busy = '0' AND lcd_enable = '0') THEN
        lcd_enable <= '1';
        IF(char < 33) THEN
          char := char + 1;
        END IF;
        CASE char IS
          WHEN 1 => lcd_bus <= "1001000100";--D
          WHEN 2 => lcd_bus <= "1000111010";--:
          WHEN 3 => lcd_bus <= "1001001010";--J
          WHEN 4 => lcd_bus <= "1000101110";--.
          WHEN 5 => lcd_bus <= "1001001100";--L
          WHEN 6 => lcd_bus <= "1001001111";--O
          WHEN 7 => lcd_bus <= "1001000010";--B
          WHEN 8 => lcd_bus <= "1001001111";--O
          WHEN 9 => lcd_bus <= "1000100000";
          WHEN 10 => lcd_bus <= "1001001010";--J
          WHEN 11 => lcd_bus <= "1000101110";--.
          WHEN 12 => lcd_bus <= "1001000100";--D
          WHEN 13 => lcd_bus <= "1001001001";--I
          WHEN 14 => lcd_bus <= "1001000001";--A
          WHEN 15 => lcd_bus <= "1001010011";--S
          WHEN 16 => lcd_bus <= "0011000000";-- Necessário para alterar a posição de escrita para a 2ªlinha
          WHEN 17 => lcd_bus <= "1000111010";
		     WHEN 18 => lcd_bus <= "1001000001";--A
          WHEN 19 =>  lcd_bus <= "1000111010";--: 
          WHEN 20 => lcd_bus <= "1001001010";--J
          WHEN 21 => lcd_bus <= "1001001111";--O
				 WHEN 22 => lcd_bus <= "1001010011";--S
				 WHEN 23 => lcd_bus <= "1001000101";--E
          WHEN 24 => lcd_bus <= "1000100000";--SPACE
          WHEN 25 => lcd_bus <= "1001010010";--R
          WHEN 26 => lcd_bus <= "1001000001";--A
          WHEN 27 => lcd_bus <= "1001000110";--F
          WHEN 28 => lcd_bus <= "1001000001";--A
          WHEN 29 => lcd_bus <= "1001000101";--E
          WHEN 30 => lcd_bus <= "1001001100";--L
          WHEN 31 => lcd_bus <= "1000100000";--SPACE
		      WHEN 32 => lcd_bus <= "1000100000";--SPACE
		  
          WHEN OTHERS => lcd_enable <= '0';
        END CASE;
      ELSE
        lcd_enable <= '0';
      END IF;
    END IF;
  END PROCESS;

END behavior;
