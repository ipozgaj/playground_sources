-- Test model for BCD decoder for 8 segment LCD display
-- (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_textio.all;
use std.textio.all;

entity bcd_t is 
end entity bcd_t;

architecture test of bcd_t is
	signal sel_t: std_logic_vector (3 downto 0);
	signal output_t: std_logic_vector (7 downto 0);

	procedure printStatus is
		variable l: line;
	begin
		write (l, now, right, 10, ns);
		write (l, string'(" sel = "));
		write (l, sel_t);
		write (l, string'(" output = "));
		write (l, output_t);
		writeline (output, l);
	end procedure printStatus;
	
begin
	t: entity work.bcd
	port map (
		sel => sel_t,
		output => output_t
	);

	process begin
		sel_t <= "0100";
		wait for 10 ns;
		printStatus;
	
		sel_t <= "0001";
		wait for 10 ns;
		printStatus;
	
		sel_t <= "1001";
		wait for 10 ns;
		printStatus;
	
		wait ;
	end process;
end architecture test;
