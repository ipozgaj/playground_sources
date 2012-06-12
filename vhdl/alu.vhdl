-- Structural model of 8 bit ALU using eight 1 bit ALUs
-- (C) 2004  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

library ieee;
use ieee.std_logic_1164.all;

entity alu is
	port (
		a, b: in std_logic_vector (7 downto 0);
		cin: in std_logic;
		sel: in std_logic_vector (2 downto 0);
		r: out std_logic_vector (7 downto 0);
		cout: out std_logic
	);
end entity alu;

architecture structural of alu is
	signal tmp: std_logic_vector (6 downto 0);
	component alu_primitive
		port (
			a, b, cin: in std_logic;
			sel: in std_logic_vector (2 downto 0);
			r, cout: out std_logic
		);
	end component alu_primitive;
	
begin
	alu1: alu_primitive port map (a(0), b(0), cin, sel, r(0), tmp(0));
	alu2: alu_primitive port map (a(1), b(1), tmp(0), sel, r(1), tmp(1));
	alu3: alu_primitive port map (a(2), b(2), tmp(1), sel, r(2), tmp(2));
	alu4: alu_primitive port map (a(3), b(3), tmp(2), sel, r(3), tmp(3));
	alu5: alu_primitive port map (a(4), b(4), tmp(3), sel, r(4), tmp(4));
	alu6: alu_primitive port map (a(5), b(5), tmp(4), sel, r(5), tmp(5));
	alu7: alu_primitive port map (a(6), b(6), tmp(5), sel, r(6), tmp(6));
	alu8: alu_primitive port map (a(7), b(7), tmp(6), sel, r(7), cout);
end architecture structural;
