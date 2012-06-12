-- Test model for half adder
-- (C) 2005 Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_textio.all;
use std.textio.all;

entity half_adder_t is
end entity half_adder_t;

architecture test of half_adder_t is
	signal a_t, b_t, s_t, cout_t: std_logic;
	
	procedure printStatus is
		variable l: line;
	begin
		write (l, now, right, 10, ns);
		write (l, string'(" in: a = "));
		write (l, a_t);
		write (l, string'("  in: b = "));
		write (l, b_t);
		write (l, "   ");
		write (l, string'(" out: s = "));
		write (l, s_t);
		write (l, string'("  out: cout= "));
		write (l, cout_t);
		writeline (output, l);
	end procedure printStatus;
	
begin
	t: entity work.half_adder
	port map (
		a => a_t,
		b => b_t,
		s => s_t,
		cout => cout_t
	);
	
	process 
	begin
--		a_t <= '0';
--		b_t <= '0';
--		wait for 10 ns;
--		printStatus;
--		
--		a_t <= '0';
--		b_t <= '1';
--		wait for 10 ns;
--		printStatus;
--		
--		a_t <= '1';
--		b_t <= '0';
--		wait for 10 ns;
--		printStatus;
--		
--		a_t <= '1';
--		b_t <= '1';
--		wait for 10 ns;
--		printStatus;
--		
		for i in 0 to 1 loop
			for j in 0 to 1 loop
				a_t <= std_logic'val(i);
				b_t <= std_logic'val(j);
				wait for 10 ns;
				printStatus;
			end loop;
		end loop;
		wait;
	end process;
	
end architecture test;
