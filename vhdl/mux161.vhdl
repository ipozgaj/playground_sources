-- Structural model of 16/1 multiplexer using four 4/1 multiplexers
-- (C) 2004  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

library ieee;
use ieee.std_logic_1164.all;

entity mux161 is
	port (
		sel: in std_logic_vector (3 downto 0);
		d: in std_logic_vector (15 downto 0);
		f: out std_logic
	);
end entity mux161;

architecture structural of mux161 is
	signal z0, z1, z2, z3: std_logic;
	signal tmp: std_logic_vector (3 downto 0);
	component mux41
		port (
			sel: in std_logic_vector (1 downto 0);
			d: in std_logic_vector (3 downto 0);
			z: out std_logic
		);
	end component mux41;

begin
	mux41_0: mux41 port map (sel(1 downto 0), d(3 downto 0), z0);
	mux41_1: mux41 port map (sel(1 downto 0), d(7 downto 4), z1);
	mux41_2: mux41 port map (sel(1 downto 0), d(11 downto 8), z2);
	mux41_3: mux41 port map (sel(1 downto 0), d(15 downto 12), z3);

	tmp <= (z3, z2, z1, z0);

	mux41_4: mux41 port map (sel(3 downto 2), tmp (3 downto 0), f);
end architecture structural;
