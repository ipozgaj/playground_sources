BEGIN { 
	srand()
	RS = "%%"
}

{
	quotes[NR] = $0 
}

END {
	print quotes[int(NR*rand())]
}
