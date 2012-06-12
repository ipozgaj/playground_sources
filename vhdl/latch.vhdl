-- Structural model of latch device
-- (C) 2005 Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

library ieee;
use ieee.std_logic_1164.all;

entity latch is
	generic (
		N: integer := 8
	);
	port (
		d_in: in std_logic_vector (N-1 downto 0);
		enable, clk: in std_logic;
		d_out: out std_logic_vector (N-1 downto 0)
	);
end entity latch;

architecture structural of latch is
	component d_flop 
		port (
			d_in, enable, clk: in std_logic;
			d_out: out std_logic
		);
	end component d_flop;

begin
	t: for i in 0 to N-1 generate
		latch_element: d_flop port map (d_in(i), enable, clk, d_out(i));
	end generate;
end architecture structural;
