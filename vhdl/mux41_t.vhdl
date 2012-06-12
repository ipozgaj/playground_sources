-- Test model for 4/1 multiplexer
-- (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_textio.all;
use std.textio.all;

entity mux41_t is 
end entity mux41_t;

architecture test of mux41_t is
	signal d_t: std_logic_vector (3 downto 0);
	signal sel_t: std_logic_vector (1 downto 0);
	signal output_t: std_logic;

	procedure printStatus is
		variable l: line;
	begin
		write (l, now, right, 10, ns);
		write (l, string'(" sel = "));
		write (l, sel_t);
		write (l, string'(" d = "));
		write (l, d_t);
		write (l, string'(" output = "));
		write (l, output_t);
		writeline (output, l);
	end procedure printStatus;
begin
	t: entity work.mux41
	port map (
		d => d_t,
		sel => sel_t,
		output =>output_t
	);

	process begin
		d_t <= "1111";
		sel_t <= "01";
		wait for 10 ns;
		printStatus;
		
		d_t <= "0100";
		sel_t <= "00";
		wait for 10 ns;
		printStatus;
	
		d_t <= "1100";
		sel_t <= "11";
		wait for 10 ns;
		printStatus;
	
		wait ;
	end process;
end architecture test;
