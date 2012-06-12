-- Behavioral model for 4/1 multiplexer
-- (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

library ieee;
use ieee.std_logic_1164.all;

entity mux41 is
	port(
		d: in std_logic_vector (3 downto 0);
		sel: in std_logic_vector (1 downto 0);
		output: out std_logic
	);
end entity mux41;

architecture behavioral of mux41 is
begin
	process (d, sel)
	begin
		case sel is
			when "00" => output <= d(0);
			when "01" => output <= d(1);
			when "10" => output <= d(2);
			when "11" => output <= d(3);
			when others => output <= 'U';
		end case;
	end process;
end architecture behavioral;
