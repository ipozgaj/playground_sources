-- Test model for JK-flop
-- (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_textio.all;
use std.textio.all;

entity jk_flop_t is
end entity jk_flop_t;

architecture test of jk_flop_t is
	signal reset_t: std_logic;
	signal enable_t: std_logic;
	signal j_t: std_logic;
	signal k_t: std_logic;
	signal q_t: std_logic;
	signal qn_t: std_logic;
	signal clk_t: std_logic;

	procedure printStatus is
		variable l: line;
	begin
		write (l, now, right, 10, ns);
		write (l, string'(" clk = "));
		write (l, clk_t);
		write (l, string'(" enable = "));
		write (l, enable_t);
		write (l, string'(" reset = "));
		write (l, reset_t);
		write (l, string'("    j = "));
		write (l, j_t);
		write (l, string'(" k = "));
		write (l, k_t);
		write (l, string'("   q = "));
		write (l, q_t);
		write (l, string'(" qn = "));
		write (l, qn_t);
		writeline (output, l);
	end procedure printStatus;

begin
	t:  entity work.jk_flop
	port map (
		reset => reset_t,
		enable => enable_t,
		j => j_t,
		k => k_t,
		q => q_t,
		qn => qn_t,
		clk => clk_t
	);

	process begin
		printStatus;
		reset_t <= '1';
		wait for 10 ns;
		printStatus;

		reset_t <= '0';

		j_t <= '0';
		k_t <= '1';
		enable_t <= '1';
		clk_t <= '0';
		wait for 10 ns;
		printStatus;
		clk_t <= '1';
		wait for 10 ns;
		printStatus;

		j_t <= '1';
		k_t <= '1';
		enable_t <= '1';
		clk_t <= '0';
		wait for 10 ns;
		printStatus;
		clk_t <= '1';
		wait for 10 ns;
		printStatus;
		
		j_t <= '1';
		k_t <= '1';
		enable_t <= '0';
		clk_t <= '0';
		wait for 10 ns;
		printStatus;
		clk_t <= '1';
		wait for 10 ns;
		printStatus;

		wait;
	end process;
end architecture test;
