-- Test model for DFA
-- (C)  2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_textio.all;
use std.textio.all;

entity automata_t is
end entity automata_t;

architecture test of automata_t is
	signal a_t: std_logic;
	signal z_t: std_logic;
	signal clk_t: std_logic;
	signal reset_t: std_logic;

	procedure printStatus is
		variable l: line;
	begin
		write (l, now, right, 10, ns);
		write (l, string'("  a="));
		write (l, a_t);
		write (l, string'(" clk="));
		write (l, clk_t);
		write (l, string'(" reset="));
		write (l, reset_t);
		write (l, string'("    z="));
		write (l, z_t);
		writeline (output, l);
	end procedure printStatus;
	
begin
	t: entity work.automata
	port map(
		a => a_t,
		z => z_t,
		clk => clk_t,
		reset => reset_t
	);

	process
	begin
		reset_t <= '0'; 
		wait for 10 ns;
		printStatus;
		reset_t <= '1'; 
		wait for 10 ns;
		printStatus;

		reset_t <= '0';

		a_t <= '1';
		clk_t <= '0';
		wait for 10 ns;
		printStatus;
		clk_t <= '1';
		wait for 10 ns;
		printStatus;
	
		a_t <= '0';
		clk_t <= '0';
		wait for 10 ns;
		printStatus;
		clk_t <= '1';
		wait for 10 ns;
		printStatus;
		
		a_t <= '1';
		clk_t <= '0';
		wait for 10 ns;
		printStatus;
		clk_t <= '1';
		wait for 10 ns;
		printStatus;

		wait;
	end process;
end architecture test;
