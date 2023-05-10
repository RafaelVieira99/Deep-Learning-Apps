--Descricao comportamental do controlador
-- José Azevedo

library ieee;
use ieee.std_logic_1164.all;

entity gamecontroller is
port(Jump,Start,Over,BackMainmenu,Ton,Credits: in std_logic;
		S: in std_logic_vector(1 downto 0);
		Gameover,Restart,Menu,TurnOn,JumpG,CreditsG: out std_logic;
		--NC_ld,NC_clr,PULSOS_ld,PULSOS_clr,Sel,p,seguinte :out std_logic;
		n: out std_logic_vector(1 downto 0)
		);
end gamecontroller;

--Descrição da arquitetura
architecture behavior of gamecontroller is
begin 
	proximo_estado: process(Jump,Start,Over,Credits,BackMainmenu,S) is
	begin
		n(1) <= (not(S(1)) and S(0) and Over ) or ((S(1)) and not(S(0)) and not(BackMainmenu)) or (not(S(1)) and not(S(0)) and Credits) or (S(1) and S(0) and Credits);
		n(0) <= (not(S(1)) and not(S(0)) and Start) or (not(S(1)) and S(0) and Jump and not(Over)) or (not(S(1)) and S(0) and not(Jump) and not(Over)) or (not(S(1)) and not(S(0)) and Credits and not(Start)) or (S(1) and S(0) and Credits); 
					
	end process proximo_estado;
	
	saidas: process(S,Jump,Start,Over,BackMainmenu,Ton,Credits) is
	begin

		
		Gameover <= ((S(1)) and not(S(0)) );
		Restart <= (not(S(1)) and not(S(0))) ;
		Menu <= (not(S(1)) and not(S(0)) ) or (S(1) and not(S(0)) and BackMainmenu) or(S(1) and S(0)) or (not(S(1)) and not(S(0)) and Credits );
		JumpG <= (not(S(1)) and S(0) and Jump) ;
		TurnOn <= (not(S(1)) and S(0) and Ton);
		CreditsG <= (not(S(1)) and not(S(0)) and Credits) or (S(1) and S(0) and Credits);
	end process saidas;
end behavior;