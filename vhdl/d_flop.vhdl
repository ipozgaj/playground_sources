-- Behavioral model of D-flop (f=100MHz, trigger on rising edge of clock)
-- (C) 2005 Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

library ieee;
use ieee.std_logic_1164.all;

entity d_flop is
	generic (
		td: time := 10 ns
	);
	port (
		d_in, clk, enable: in std_logic;
		d_out: out std_logic
	);
end entity d_flop;

architecture behavioral of d_flop is
begin
	process (clk)
	begin
		if (rising_edge (clk) and enable='1') then
			d_out <= d_in after td;
		end if;
	end process;
end architecture behavioral;
