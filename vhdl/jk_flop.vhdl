-- Behavioral model of JK-flop, triggers on rising edge of clock
-- (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

library ieee;
use ieee.std_logic_1164.all;

entity jk_flop is
	port (
		j: in std_logic;
		k: in std_logic;
		clk: in std_logic;
		enable: in std_logic;
		reset: in std_logic;
		q: out std_logic;
		qn: out std_logic
	);
end entity jk_flop;

architecture behavioral of jk_flop is
begin
	process (clk, reset)
		variable sel: std_logic_vector (1 downto 0);
		variable state: std_logic;
	begin
		if (reset = '1') then
			q <= '1';
			qn <= '0';
			state := '1';
		else 
			if (rising_edge (clk) and enable = '1') then
				sel := (j, k);
				case sel is
					when "00" => state := state;
					when "01" => state := '0';
					when "10" => state := '1';
					when "11" => state := not state;
					when others => state := 'U';
				end case;
				q <= state;
				qn <= not state;
			end if;
		end if;
	end process;
end architecture behavioral;
