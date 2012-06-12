-- Test model for shift register
-- (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_textio.all;
use std.textio.all;

entity shifter_t is
end entity shifter_t;

architecture test of shifter_t is
	signal sel_t: std_logic_vector (1 downto 0);
	signal sin_t: std_logic;
	signal clk_t: std_logic;
	signal pin_t: std_logic_vector (7 downto 0);
	signal pout_t: std_logic_vector (7 downto 0);

	procedure printStatus is
		variable l: line;
	begin
		write (l, now, right, 10, ns);
		write (l, string'("  sel="));
		write (l, sel_t);
		write (l, string'(" sin="));
		write (l, sin_t);
		write (l, string'(" clk="));
		write (l, clk_t);
		write (l, string'(" pin="));
		write (l, pin_t);
		write (l, string'("    pout="));
		write (l, pout_t);
		writeline (output, l);
	end procedure printStatus;
	
begin
	t: entity work.shifter
	port map (
		sel => sel_t,
		sin => sin_t,
		clk => clk_t,
		pin => pin_t,
		pout => pout_t
	);
	
	process
	begin
		sin_t <= '1';
		pin_t <= "10010110";
		sel_t <= "11";
		clk_t <= '0';
		wait for 100 ns;
		printStatus;
		clk_t <= '1';
		wait for 100 ns;
		printStatus;
		clk_t <= '0';
		wait for 100 ns;
		printStatus;
		clk_t <= '1';
		wait for 100 ns;
		printStatus;
		
		sel_t <= "10";
		clk_t <= '0';
		wait for 100 ns;
		printStatus;
		clk_t <= '1';
		wait for 100 ns;
		printStatus;
		clk_t <= '0';
		wait for 100 ns;
		printStatus;
		clk_t <= '1';
		wait for 100 ns;
		printStatus;
		
		sin_t <= '0';
		sel_t <= "01";
		clk_t <= '0';
		wait for 100 ns;
		printStatus;
		clk_t <= '1';
		wait for 100 ns;
		printStatus;
		clk_t <= '0';
		wait for 100 ns;
		printStatus;
		clk_t <= '1';
		wait for 100 ns;
		printStatus;
		
		wait;
	end process;
end architecture test;
