-- Behavioral and structural model of 1-bit full adder
-- (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

library ieee;
use ieee.std_logic_1164.all;

entity full_adder is
	generic (
		td: time := 10 ns
	);
	port (
		a, b, cin: in std_logic;
		s, cout: out std_logic
	);
end entity full_adder;

architecture behavioral of full_adder is
begin
	process (a, b, cin)
		variable input: std_logic_vector (2 downto 0);
		variable output: std_logic_vector (1 downto 0);
	begin
		input := (a, b, cin);

		case input is
			when "000" => output := "00";
			when "001" => output := "01";
			when "010" => output := "01";
			when "011" => output := "10";
			when "100" => output := "01";
			when "101" => output := "10";
			when "110" => output := "10";
			when "111" => output := "11";
			when others => output := "UU";
		end case;

		s <= output(0);
		cout <= output(1);
	end process;
end architecture behavioral;

architecture structural of full_adder is
	component half_adder
		generic (
			td : time := 10 ns
		);
		port (
			a, b: in std_logic;
			s, cout: out std_logic
		);
	end component half_adder;

	signal i_s, i_c1, i_c2: std_logic;
	
begin
	half_adder_0: half_adder port map (a, b, i_s, i_c1);
	half_adder_1: half_adder port map (i_s, cin, s, i_c2);
	cout <= i_c1 or i_c2;
end architecture structural;
