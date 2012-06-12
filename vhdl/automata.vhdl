-- Behavioral model of simple DFA
-- (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

library ieee;
use ieee.std_logic_1164.all;

entity automata is
	port (
		a: in std_logic;
		clk: in std_logic;
		reset: in std_logic;
		z: out std_logic
	);
end entity automata;

architecture behavioral of automata is
	subtype state_type is std_logic_vector (1 downto 0);
	signal state: state_type;

	procedure switchState (
		a: std_logic; 
		state: state_type; 
		next_state: out state_type;	
		z_new: out std_logic
	) is
		variable input: std_logic_vector (2 downto 0);
		variable output: std_logic_vector (2 downto 0);
	begin
		input := (state(1), state(0), a);
		case input is
			when "000" => output := "000";
			when "001" => output := "110";
			when "010" => output := "010";
			when "011" => output := "110";
			when "100" => output := "101";
			when "101" => output := "011";
			when "110" => output := "101";
			when "111" => output := "001";
			when others => output := "UUU";
		end case;
		next_state := output (2 downto 1);
		z_new := output(0);
	end procedure switchState;
	
begin
	process (clk, reset)
		variable next_state: state_type;
		variable z_new: std_logic;
	begin
		if (rising_edge (reset)) then
			state <= "00";
			z <= '0';
		else 
			if (rising_edge (clk)) then
				switchState (a, state, next_state, z_new);
				z <= z_new;
				state <= next_state;
			end if;
		end if;
	end process;
end architecture behavioral;
