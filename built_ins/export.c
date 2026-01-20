#include "../minishell.h"

//commandline = ver se tem 2 aspas
//ver se tem =
//ver se tem espacos
//tem que ser export NAME ou export NAME=VALUE
//se for name
	//ver se ja exite na env, ver se ja esta no local var
		//se ja exite na env, nao faz nada
		//se ja exite no local var, passa pro env e remove do local var
		//se nao exite em nenhum dos dois, cria na env com valor vazio
//se for name=value
	//ver se ja exite na env, ver se ja esta no local var
		//se ja exite na env, atualiza o valor
		//se ja exite no local var, passa pro env e remove do local var e atualiza o valor
		//se nao exite em nenhum dos dois, cria na env com o valor
//error messages:
	//export: 'NAME=VALUE': not a valid identifier
	//export: 'NAME': not a valid identifier

void	export(char *commandline, t_mshell_data *data)
{
	printf("Command: %s", commandline);
	print_env(data);
}