-- Test model for full adder
-- (C) 2005 Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_textio.all;
use std.textio.all;

entity full_adder_t is
end entity full_adder_t;

architecture test of full_adder_t is
	signal a_t, b_t, cin_t, s_t, cout_t: std_logic;
	
	procedure printStatus is
		variable l: line;
	begin
		write (l, now, right, 10, ns);
		write (l, string'(" in: a = "));
		write (l, a_t);
		write (l, string'("  in: b = "));
		write (l, b_t);
		write (l, string'("  in: cin = "));
		write (l, cin_t);
		write (l, "   ");
		write (l, string'(" out: s = "));
		write (l, s_t);
		write (l, string'("  out: cout= "));
		write (l, cout_t);
		writeline (output, l);
	end procedure printStatus;
	
begin
	t: entity work.full_adder(structural)
	port map (
		a => a_t,
		b => b_t,
		cin => cin_t,
		s => s_t,
		cout => cout_t
	);
	
	process 
	begin
		a_t <= '0';
		b_t <= '0';
		cin_t <= '0';
		wait for 30 ns;
		printStatus;
		
		a_t <= '0';
		b_t <= '1';
		cin_t <= '0';
		wait for 30 ns;
		printStatus;
		
		a_t <= '1';
		b_t <= '0';
		cin_t <= '0';
		wait for 30 ns;
		printStatus;
		
		a_t <= '1';
		b_t <= '1';
		cin_t <= '0';
		wait for 30 ns;
		printStatus;
		
		a_t <= '0';
		b_t <= '0';
		cin_t <= '1';
		wait for 30 ns;
		printStatus;
		
		a_t <= '0';
		b_t <= '1';
		cin_t <= '1';
		wait for 30 ns;
		printStatus;
		
		a_t <= '1';
		b_t <= '0';
		cin_t <= '1';
		wait for 30 ns;
		printStatus;
		
		a_t <= '1';
		b_t <= '1';
		cin_t <= '1';
		wait for 30 ns;
		printStatus;
		
		wait;
	end process;
	
end architecture test;
