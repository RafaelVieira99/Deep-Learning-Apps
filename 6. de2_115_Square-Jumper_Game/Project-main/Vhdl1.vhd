
library IEEE;
use  IEEE.STD_LOGIC_1164.all;
use  IEEE.STD_LOGIC_ARITH.all;
use  IEEE.STD_LOGIC_UNSIGNED.all;
LIBRARY lpm;
USE lpm.lpm_components.ALL;


entity vga_control is
	PORT(
		char_in        		: IN 	STD_LOGIC_VECTOR(5 DOWNTO 0);
		char_out    		: OUT 	STD_LOGIC_VECTOR(5 DOWNTO 0);
		wraddress	    	: OUT 	STD_LOGIC_VECTOR(7 DOWNTO 0);
		insert,backspace	: IN	STD_LOGIC;
		wren   				: OUT	STD_LOGIC;
		l	    	   		: OUT 	STD_LOGIC_VECTOR(7 DOWNTO 0);
		c					: OUT 	STD_LOGIC_VECTOR(3 DOWNTO 0);
		restart				: OUT   STD_LOGIC
		);
end vga_control;


architecture behavior of vga_control is
begin 
    process(insert) is
	
								
	variable linhas : integer := 0; 
	variable colunas : integer := 0;
	
    begin
	
		if (insert='1') then
		
			if(backspace='1')then
				if( (linhas=0) and (colunas=0) ) then
					char_out<="000000";
					wraddress<="00000000";
					wren<='1'; 
					colunas := 0;
					linhas  := 0;
					
				elsif ( (colunas<1) and (linhas>15) ) then
					char_out<="000000";
					wraddress<= conv_std_logic_vector(linhas+colunas,8);
					wren<='1'; 
					colunas := 9;
					linhas := linhas-16;
				else
					char_out<="000000";			 
					wraddress<= conv_std_logic_vector(linhas+colunas,8);
					wren<='1'; 
					colunas := colunas-1;
					linhas  := linhas;
				end if;
			
			
			else
		
				if( (linhas=0) and (colunas=0) ) then
					char_out<=char_in;
					wraddress<="00000000";
					wren<='1'; 
					colunas := colunas+1;
					
				elsif ( (colunas>8) and (linhas=0) ) then
					char_out<=char_in;			 
					wraddress<= conv_std_logic_vector(linhas+colunas,8);
					wren<='1';
					linhas :=16;
					colunas := 0;
					
				elsif ( (colunas>8) and (linhas>15) ) then
					char_out<=char_in;	 
					wraddress<= conv_std_logic_vector(linhas+colunas,8);
					wren<='1'; 
					colunas := 0;
					linhas := linhas+16;
					
				elsif ( linhas>=112 )then ---
					restart<='1';
					
					colunas := 0;
					linhas := 0;
				
					char_out<=char_in;	 
					wraddress<= conv_std_logic_vector(linhas+colunas,8);
					wren<='1'; 

				else
					char_out<=char_in;			 
					wraddress<= conv_std_logic_vector(linhas+colunas,8);
					wren<='1'; 
					colunas := colunas+1;
					linhas  := linhas;
					
				end if;
				
			end if;
			
		end if;
		
		linhas:=linhas;
		colunas:=colunas;
		
		l<= conv_std_logic_vector(linhas,8);
		c<= conv_std_logic_vector(colunas,4);

		

		
    end process;
end behavior;