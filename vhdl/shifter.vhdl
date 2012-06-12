-- Behavioral model of shift register
-- (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

library ieee;
use ieee.std_logic_1164.all;

entity shifter is
	generic (
		bit_count: integer := 8;
		delay: time := 100 ns
	);
	port (
		sel: in std_logic_vector (1 downto 0);
		sin: in std_logic;
		clk: in std_logic;
		pin: in std_logic_vector (bit_count-1 downto 0);
		pout: out std_logic_vector (bit_count-1 downto 0)
	);
end entity shifter;

architecture behavioral of shifter is
	signal content: std_logic_vector (bit_count-1 downto 0);
begin
	process (clk)
	begin
		if (rising_edge (clk)) then
			case sel is
				when "00" => content <= content;
				when "01" => content <= sin & content (bit_count-1 downto 1);
				when "10" => content <= content (bit_count-2 downto 0) & sin;
				when "11" => content <= pin;
				when others => content <= "U";
			end case;
			pout <= content after delay;
		end if;
	end process;
end architecture behavioral;
