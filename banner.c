#include "minishell.h"

void	print_banner(void)
{
	printf("\n"
		   YELLOW"   ∧＿∧  \n"
		   "  (｡"RESET"･"YELLOW"ω"RESET"･"YELLOW"｡)つ"RESET"━☆"MAGENTA" ・*"RESET"。\n"
		   YELLOW " ⊂/    /  "MAGENTA"      ・゜\n"
		   YELLOW"  しーＪ    "MAGENTA"      °"RESET"。+*"MAGENTA" °。\n"
		   "                  "MAGENTA" ."RESET"・゜ \n"
		   "                  "MAGENTA"   ゜｡ﾟﾟ"RESET"･｡･ﾟﾟ\n"
		   MAGENTA" _  _  __  __ _  __  ____  _  _  ____  __    __   \n"
		   "( \\/ )(  )(  ( \\(  )/ ___)/ )( \\(  __)(  )  (  )  \n"
		   YELLOW"/ \\/ \\ )( /    / )( \\___ \\) __ ( ) _) / (_/\\/ (_/\\\n"
		   MAGENTA"\\_)(_/(__)\\_)__)(__)(____/\\_)(_/(____)\\____/\\____/\n"
		   "\n");
}