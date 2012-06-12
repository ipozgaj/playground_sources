-- Beahvioral model of BCD decoder for 8 segment LCD display
-- (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

library ieee;
use ieee.std_logic_1164.all;

entity bcd is
	port(
		sel: in std_logic_vector (3 downto 0);
		output: out std_logic_vector (7 downto 0)
	);
end entity bcd;

architecture behavioral of bcd is
begin
	process (sel) is
	begin
		case sel is
			when "0000" => output <= "11111100";
			when "0001" => output <= "01100000";
			when "0010" => output <= "11011010";
			when "0011" => output <= "11110010";
			when "0100" => output <= "01100110";
			when "0101" => output <= "10110110";
			when "0110" => output <= "10111111";
			when "0111" => output <= "11100000";
			when "1000" => output <= "11111110";
			when "1001" => output <= "11100111";
			when others => output <= "UUUUUUUU";
		end case;
	end process;
end architecture behavioral;
