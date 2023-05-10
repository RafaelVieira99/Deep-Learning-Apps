
library IEEE;
use  IEEE.STD_LOGIC_1164.all;
use  IEEE.STD_LOGIC_ARITH.all;
use  IEEE.STD_LOGIC_UNSIGNED.all;
LIBRARY lpm;
USE lpm.lpm_components.ALL;


entity mainmenu is
	PORT(
	-----------------------------   inputs do fundo (rom)     -----------------------------------------
		red  : IN 	STD_LOGIC_VECTOR(3 DOWNTO 0);
		green : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
		blue: IN 	STD_LOGIC_VECTOR(3 DOWNTO 0);
		
		showmenu : in std_logic;
	------------------------------    outputs (cores)     -------------------------------------------
		R	: OUT 	STD_LOGIC_VECTOR(9 DOWNTO 0);
		G   : OUT 	STD_LOGIC_VECTOR(9 DOWNTO 0);
		B	: OUT 	STD_LOGIC_VECTOR(9 DOWNTO 0);
	------------------------------- coordenadas do ecra vga -----------------------------------------	
		x: IN 	STD_LOGIC_VECTOR(9 DOWNTO 0);
		y: IN 	STD_LOGIC_VECTOR(9 DOWNTO 0)
	-----------------------------------------------------------------------------------------------
		);
end mainmenu;


architecture behavior of mainmenu is
begin 
    process(x,y) is
								
    begin
	
		if( showmenu = '1' ) then
			if( (CONV_INTEGER(unsigned(y))>=80) and (CONV_INTEGER(unsigned(y))<=560) ) then
				R<= red & "000000";
				G<= green & "000000";
				B<= blue & "000000";
			else			
				R<= "0000000000";
				G<= "0000000000";
				B<= "0000000000";
			end if;
			
		else
				R<= "0000000000";
				G<= "0000000000";
				B<= "0000000000";
		end if;
			
			
    end process;
end behavior;