
library IEEE;
use  IEEE.STD_LOGIC_1164.all;
use  IEEE.STD_LOGIC_ARITH.all;
use  IEEE.STD_LOGIC_UNSIGNED.all;
LIBRARY lpm;
USE lpm.lpm_components.ALL;


entity contador_jumps is
	PORT(
		estado  : IN 	STD_LOGIC_VECTOR(1 DOWNTO 0);
		jump    : IN 	STD_LOGIC;
		score   : OUT 	STD_LOGIC_VECTOR(4 DOWNTO 0)
		);
end contador_jumps;


architecture behavior of contador_jumps is
begin 
	
	
    process(estado,jump) is
	
	variable counter : integer := 0;
	
    begin
	
	if(falling_edge(jump))then
		if ( (estado(0) = '1') and (estado(1) = '0') ) then
			counter := counter + 1;
		else
			counter := counter;
		end if;
	end if;
	
	
	score<=conv_std_logic_vector(counter,5);		
			
    end process;
end behavior;