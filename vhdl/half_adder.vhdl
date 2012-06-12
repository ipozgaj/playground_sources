-- Behavioral model of half adder
-- (C) 2005 Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

library ieee;
use ieee.std_logic_1164.all;

entity half_adder is
	generic (
		td: time := 10 ns
	);
	port (
		a, b: in std_logic;
		s, cout: out std_logic
	);
end entity half_adder;

architecture behavioral of half_adder is
begin
	s <= a xor b after td;
	cout <= a and b after td;
end architecture behavioral;
