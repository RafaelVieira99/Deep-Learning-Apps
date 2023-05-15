
library IEEE;
use  IEEE.STD_LOGIC_1164.all;
use  IEEE.STD_LOGIC_ARITH.all;
use  IEEE.STD_LOGIC_UNSIGNED.all;
LIBRARY lpm;
USE lpm.lpm_components.ALL;

entity jump_controller is
	PORT(
	--------------------------------- key de jump -----------------------------------------------------
		jump		: IN 	STD_LOGIC;
		----------------------------- clock do jump e do spawn de touros (difficulty) -----------------
		difficulty       : IN    STD_LOGIC;
	-----------------------------   inputs do fundo (rom)     -----------------------------------------
		red  : IN 	STD_LOGIC_VECTOR(3 DOWNTO 0);
		green : IN 	STD_LOGIC_VECTOR(3 DOWNTO 0);
		blue: IN 	STD_LOGIC_VECTOR(3 DOWNTO 0);
	------------------------------    outputs (cores)     -------------------------------------------
		R	: OUT 	STD_LOGIC_VECTOR(9 DOWNTO 0);
		G   : OUT 	STD_LOGIC_VECTOR(9 DOWNTO 0);
		B	: OUT 	STD_LOGIC_VECTOR(9 DOWNTO 0);
	------------------------------- coordenadas do ecra vga -----------------------------------------	
		x: IN 	STD_LOGIC_VECTOR(9 DOWNTO 0);
		y: IN 	STD_LOGIC_VECTOR(9 DOWNTO 0);
	-----------------------------------------------------------------------------------------------
	
		turn_on,restart       : IN    STD_LOGIC := '0';
		c :  OUT 	STD_LOGIC;
		
		gameover: OUT STD_LOGIC := '0'
		
		
		
		);
end jump_controller;


architecture behavior of jump_controller is
begin 
    process(x,y) is

	variable linhas : integer := 0;
	variable colunas : integer := 0;
	variable counter : integer := 0;
	variable save : integer := 0;
	variable rising : integer := 1;
	variable salto : integer :=0;
	
    variable counter2 : integer := 0;
 	variable rising2 : integer := 0;
	variable save2 : integer := 0;
	
	variable aux: integer := 0;
	
	variable hold: integer :=0;
 
    begin
		linhas := CONV_INTEGER(unsigned(x));
		colunas := CONV_INTEGER(unsigned(y));
----------------------------------------------------------------
----------------------------------------------------------------
----------------------------------------------------------------
-------------------								-------------------
-------------------								-------------------
-------------------			JUMP CODE 		-------------------
-------------------								-------------------


		
		
		if(hold = 0) then

			if ( rising_edge(jump) )  then
					salto :=1;
			end if;
			
			if( jump = '0' ) then
					salto :=0;
			end if;
			
			if (salto = 1)  then
				if(rising_edge(difficulty)) then
					save:=save+1;
					if (save>=8) then
						save:=0;
						if ( counter>=200 ) then
							rising :=-1;
						end if;
						if ( counter < 0 ) then
							rising := 0;
							counter :=0;
						end if;
						counter:=counter+rising;
					end if;
				end if;
				if( (colunas>=80) and (colunas<=560) ) then
					if ((colunas > 150 and colunas<182) and ( (linhas>(400-counter)) and (linhas<(432-counter)) ))  then
						r<="1111111111";
						g<="0000000000";
						b<="0000000000";
					else 
						R<= red & "000000";
						G<= green & "000000";
						B<= blue & "000000";
					end if;
				else
					R<= "0000000000";
					G<= "0000000000";
					B<= "0000000000";

				end if;
			else
				rising :=1;
				counter:=0;
				if( (colunas>=80) and (colunas<=560) ) then
					if ((colunas > 150 and colunas<182) and ((linhas>400 and linhas<432)))  then--white
						r<="1111111111";
						g<="0000000000";
						b<="0000000000";
					else 
						R<= red & "000000";
						G<= green & "000000";
						B<= blue & "000000";
					end if;	
				else
					R<= "0000000000";
					G<= "0000000000";
					B<= "0000000000";
				end if;
			end if;
		
		

		
----------------------------------------------------------------
----------------------------------------------------------------
----------------------------------------------------------------
-------------------								-------------------
-------------------								-------------------
-------------------			TOURO CODE 		-------------------
-------------------								-------------------
		end if;
		
		if ( rising_edge(difficulty) )  then
			save2:=save2+1;
			if (save2>=14) then
				save2:=0;
				if ( counter2<500 and (restart = '0') ) then
					counter2:=counter2+1;
				else
					counter2:=0;
				end if;
				
			end if;
			
		end if;
		if (((colunas>(560-counter2)) and (colunas<(592-counter2))) and ((linhas>368)and(linhas<432)))  then
				r<="1111111111";
				g<="1000000111";
				b<="0000000000";
		end if;
		
		
-----------------------------------------------------------------------------------------------------------------------

		if ( rising_edge(turn_on) ) then 
			aux := aux+ 1;
		end if;
		
		if( aux>13 ) then
				gameover <= '0';
				hold :=0;
		end if;
		
		if (aux > 0) then
			if( ( ((counter2>(378))  and (counter2<(410))) or ((counter2>(378+32))  and (counter2<(410+32)))  ) and (counter < 10) ) then
				gameover	<='1';
				hold :=1;
			end if;
		end if; 
		
		if(restart='1') then
			 gameover <= '0';
			 hold:=0;
		end if;
		
		
		if (aux = 0) then
		  c<='0';
		else
		  c<='1';
	    end if;
    end process;
end behavior;








