-- Behavioral model of 1-bit ALU
-- 
-- Supported functions:
---
--	SEL		function
--  --------------------
--	000		A + B + Cin
--	001		A - B - Cin
--	010		A and  B
--	011		A nand B
--	100		A or   B
--	101		A nor  B
--	110		A xor  B
--	111		not    A
-- 
-- (C) 2004  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

library ieee;
use ieee.std_logic_1164.all;

entity alu_primitive is
	port (
		a, b, cin: in std_logic;
		sel: in std_logic_vector (2 downto 0);
		r, cout: out std_logic
	); 
end entity alu_primitive;

architecture behavioral of alu_primitive is
begin
	process (a, b, cin, sel)
  		variable tmp: std_logic;
	begin
		case sel is
			when "000" => 
				r <= (a and not b and cin) or (not a and b and not cin) or 
					 (a and not b and not cin) or (a and b and cin);
				cout <= (a and b) or (b and cin) or (a and cin);
			
			when "001" =>
				r <= (not a and not b and cin) or (not a and b and not cin) or
					 (a and not b and not cin) or (a and b and cin);
				cout <= (not a and b) or (b and cin) or (not a and cin);

			when "010" =>
				r <= a and b;
				tmp := a and b;
				cout <= tmp or cin;

			when "011" =>
				r <= a nand b;
				tmp := a nand b;
				cout <= tmp or cin;
	
			when "100" =>
				r <= a or b;
				tmp := a or b;
				cout <= tmp or cin;
	
			when "101" =>
				r <= a nor b;
				tmp := a nor b;
				cout <= tmp or cin;

			when "110" =>
				r <= a xor b;
				tmp := a xor b;
				cout <= tmp or cin;

			when "111" =>
				r <= not a;
				tmp := not a;
				cout <= tmp or cin;
			
			when others =>
				r <= '0';
				cout <= '0';
		end case;
	end process;
end architecture behavioral;
