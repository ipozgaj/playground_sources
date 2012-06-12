-- Test model for D-flop
-- (C) 2005 Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_textio.all;
use std.textio.all;

entity d_flop_t is
end entity d_flop_t;

architecture test of d_flop_t is
	signal d_in_t, d_out_t, clk_t, enable_t: std_logic;

	procedure printStatus is
		variable l: line;
	begin
		write (l, now, right, 10, ns);
		write (l, string'("  d_in="));
		write (l, d_in_t);
		write (l, string'("  clk="));
		write (l, clk_t);
		write (l, string'("  enable="));
		write (l, enable_t);
		write (l, string'("  d_out="));
		write (l, d_out_t);
		writeline (output, l);
	end procedure printStatus;
	
begin
	t: entity work.d_flop
	port map (
		d_in => d_in_t,
		clk => clk_t,
		enable => enable_t,
		d_out => d_out_t
	);

	process
	begin
		enable_t <= '1';
		clk_t <= '0';
		d_in_t <= '1';
		wait for 5 ns;
		printStatus;
		clk_t <= '1';
		wait for 5 ns;
		printStatus;
		wait for 5 ns;
		printStatus;
		
		wait;
	end process;
end architecture test;
