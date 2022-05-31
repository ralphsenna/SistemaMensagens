//Declaração de Structs
struct TpData //20B
{
	int Ano, Mes, Dia, Hora, Minuto; //4B + 4B + 4B + 4B + 4B = 20B
};

struct TpMensagem //578B
{
	TpMensagem *MensAnt; //4B
	TpMensagem *MensProx; //4B
	TpData DataHora;
	char Login[50],Assunto[50], Mensagem[500]; //50B + 500B + 20B = 570B
};

struct TpUsuario //93B
{
	TpUsuario *UsuAnt; //4B
	TpUsuario *UsuProx; //4B
	TpMensagem *UsuMens; //4B
	char Login[50], Senha[30], Tipo; //50B + 30B + 1B = 81B
};

struct TpServidor //72B
{
	TpServidor *ServAnt; //4B
	TpServidor *ServProx; //4B
	TpUsuario *ServUsu; //4B
	char Dominio[30], Local[30]; //30B + 30B = 60B
};

struct TpDescritor //13B
{
	char Raiz[5]; //5B
	TpServidor *Inicio, *Fim; //4B + 4B = 8B
};

void Inicializar(TpDescritor &D)
{
	strcpy(D.Raiz, "Raiz");
	D.Inicio = NULL;
	D.Fim = NULL;
}

TpServidor* BuscaServidor(TpDescritor D, char Dominio[30], char Login[50])
{
	TpServidor *Serv;
	char *Div, LoginBackup[50];
	strcpy(LoginBackup, Login);
	Serv = D.Inicio;
	if (strcmp(Dominio, "")!=0)
	{
		while (Serv!=NULL && strcmp(Serv->Dominio, Dominio)!=0)
			Serv = Serv->ServProx;
	}
	else if (strcmp(Login, "")!=0)
	{
		if (strchr(Login, '@'))
		{
			Div = strtok(Login, "@");
			Div = strtok(NULL, "@");
			while (Serv!=NULL && strcmp(Serv->Dominio, Div)!=0)
				Serv = Serv->ServProx;
			strcpy(Login, LoginBackup);
		}
		else
			Serv = NULL;
	}
	if (Serv!=NULL && (strcmp(Dominio, "")!=0 || strcmp(Login, "")!=0))
		return Serv;
	else
		return NULL;
}

TpUsuario* BuscaUsuario(TpDescritor D, char Login[50])
{
	TpServidor *Serv=BuscaServidor(D, "", Login);
	TpUsuario *Usu=Serv->ServUsu;
	char LoginBackup[50];
	if (strchr(Login, '@'))
	{
		strcpy(LoginBackup, Login);
		while (Usu!=NULL && strcmp(Usu->Login, Login)!=0)
			Usu = Usu->UsuProx;
		strcpy(Login, LoginBackup);
		if (Usu!=NULL)
			return Usu;
		else
			return NULL;
	}
	else
		return NULL;
}

char VerificaLogin(TpDescritor D, TpUsuario U) 
{
	TpServidor *SAtual;
	TpUsuario *UAtual;
	char Tipo;
	SAtual = BuscaServidor(D, "", U.Login);
	if (SAtual!=NULL)
	{
		UAtual = SAtual->ServUsu;
		while (UAtual!=NULL && strcmp(UAtual->Login, U.Login)!=0)
			UAtual = UAtual->UsuProx;
		if (strcmp(U.Login, UAtual->Login)==0 && strcmp(U.Senha, UAtual->Senha)==0)
			Tipo = UAtual->Tipo;
		else if (UAtual!=NULL && strcmp(U.Senha, UAtual->Senha)!=0)
			Tipo ='S';
	}
	else
		Tipo = 'X';
	return Tipo;
}

char SeparaDominio(TpDescritor D, char Login[50])
{
	TpServidor *Serv=D.Inicio;
	char *Div, LoginBackup[50];
	strcpy(LoginBackup, Login);
	if (strchr(Login, '@'))
	{
		Div = strtok(Login, "@");
		Div = strtok(NULL, "@");
		while (Serv!=NULL && strcmp(Serv->Dominio, Div)!=0)
			Serv = Serv->ServProx;
		strcpy(Login, LoginBackup);
		if (Serv!=NULL)
			return 'T';
		else
			return 'F';
	}
	else
		return 'L';
}

void LimparMensagens(TpUsuario *Usu)
{
	TpMensagem *MensF;
	MensF = Usu->UsuMens;
	if (MensF!=NULL)
	{
		while (MensF->MensProx!=NULL)
			MensF = MensF->MensProx;
		while (MensF->MensAnt!=NULL)
		{
			MensF = MensF->MensAnt;
			delete(MensF->MensProx);
		}
		delete(MensF);
		Usu->UsuMens = NULL;
	}
}

void LimparUsuarios(TpServidor *Serv)
{
	TpUsuario *UsuF;
	UsuF = Serv->ServUsu;
	if (UsuF!=NULL)
	{
		while (UsuF->UsuProx!=NULL)
			UsuF = UsuF->UsuProx;
		while (UsuF->UsuAnt!=NULL)
		{
			LimparMensagens(UsuF);
			UsuF = UsuF->UsuAnt;
			delete(UsuF->UsuProx);
		}
		delete(UsuF);
		Serv->ServUsu = NULL;
	}
}

void CadastrarServidorOrd(TpDescritor &D, TpServidor Serv)
{
	TpServidor *Novo, *Aux;
	Novo = new TpServidor;
	strcpy(Novo->Dominio, Serv.Dominio);
	strcpy(Novo->Local, Serv.Local);
	Novo->ServProx = NULL;
	Novo->ServAnt = NULL;
	Novo->ServUsu = NULL;
	if (D.Inicio==NULL)
	{
		D.Inicio = Novo;
		D.Fim = Novo;
	}
	else
	{
		if (strcmp(Serv.Dominio, D.Inicio->Dominio)<=0)
		{
			D.Inicio->ServAnt = Novo;
			Novo->ServProx = D.Inicio;
			D.Inicio = Novo;	
		}
		else if (strcmp(Serv.Dominio, D.Fim->Dominio)>=0)
		{
			D.Fim->ServProx = Novo;
			Novo->ServAnt = D.Fim;
			D.Fim = Novo;
		}
		else
		{
			Aux = D.Inicio->ServProx;
			while (strcmp(Serv.Dominio, Aux->Dominio)>0)
                Aux = Aux->ServProx;
			Novo->ServProx = Aux;
			Novo->ServAnt = Aux->ServAnt;
			Aux->ServAnt->ServProx = Novo;
			Novo->ServProx->ServAnt = Novo;
		}
	}
}

void ListarServidores(TpDescritor D)
{
	TpServidor *Aux=D.Inicio;
	if (Aux==NULL)
		printf("\n\nLista de Servidores Vazia!\n");
	else
	{
		printf("\n\n  ** LISTA DE TODOS OS SERVIDORES **\n");
		printf("-----------------------------------------------\n");
		printf("| Dominio                 | Local             |\n");
		printf("-----------------------------------------------\n");
		printf("|                         |                   |\n");
		while (Aux!=NULL)
		{
			printf("| %-23s | %-17s |\n", Aux->Dominio, Aux->Local);
			Aux = Aux->ServProx;
		}
		printf("-----------------------------------------------\n");
	}
	getch();
}

void ConsultarServidores(TpDescritor D)
{
	TpServidor *Serv=D.Inicio;
	char Dominio[30];
	if (Serv==NULL)
	{
		printf("\n\nLista de Servidores Vazia!\n");
		getch();
	}
	else
	{
		printf("\n\nQual nome do Dominio do Servidor que procura?\n");
		gets(Dominio);
		while (strcmp(Dominio, "\0")!=0)
		{
			Serv = BuscaServidor(D, Dominio, "");
			if (Serv!=NULL)
				printf("\nDominio: %s\tLocal: %s\n", Serv->Dominio, Serv->Local);
			else
				printf("\nEste Servidor nao esta cadastrado!\n");
			getch();
			printf("\nQual nome do Dominio do Servidor que procura?\n");
			gets(Dominio);
		}
	}
}

//Não terminado
void AlterarServidor(TpDescritor &D, char Dominio[30])
{
	TpServidor *Serv;
	char novoD[30];
	Serv = BuscaServidor(D,Dominio,"");
	printf("Digite o novo dominio ou digite ENTER para cancelar:\n");
	scanf("%s",&novoD);
	if(BuscaServidor(D,novoD,"")==NULL)
	{
		//Serv->Dominio = novoD;
		//Serv = LinkarUsuarios(Serv);
	}
	else
	{
		printf("Servidor já existente\n");
	}
} 

void ExcluirServidor(TpDescritor &D, char Dominio[30])
{
	TpServidor *Aux;
	if (strcmp(D.Inicio->Dominio, Dominio)==0)
	{
		Aux = D.Inicio;
		LimparUsuarios(Aux);
		D.Inicio = D.Inicio->ServProx;
		D.Inicio->ServAnt = NULL;
		delete(Aux);
	}
	else if (strcmp(D.Fim->Dominio, Dominio)==0)
	{
		Aux = D.Fim;
		LimparUsuarios(Aux);
		D.Fim = D.Fim->ServAnt;
		D.Fim->ServProx = NULL;
		delete(Aux);
	}
	else
	{
		Aux = D.Inicio;
		while (strcmp(Aux->Dominio, Dominio)!=0)
			Aux = Aux->ServProx;	
		LimparUsuarios(Aux);
		Aux->ServProx->ServAnt = Aux->ServAnt;
		Aux->ServAnt->ServProx = Aux->ServProx;
		delete(Aux);
	}
}

void CadastrarUsuarioOrd(TpDescritor &D, TpUsuario Usu)
{
	TpUsuario *Novo, *Aux;
	TpServidor *Serv=BuscaServidor(D, "", Usu.Login);
	Novo = new TpUsuario;
	strcpy(Novo->Login, Usu.Login);
	strcpy(Novo->Senha, Usu.Senha);
	Novo->Tipo = Usu.Tipo;
	Novo->UsuProx = NULL;
	Novo->UsuAnt = NULL;
	Novo->UsuMens = NULL;
	if (Serv->ServUsu==NULL)
		Serv->ServUsu = Novo;
	else if (strcmp(Novo->Login, Serv->ServUsu->Login)<=0)
	{
		Novo->UsuProx = Serv->ServUsu;
		Novo->UsuProx->UsuAnt = Novo;
		Serv->ServUsu = Novo;	
	}
	else
	{
		Aux = Serv->ServUsu;
		while (Aux->UsuProx!=NULL && strcmp(Novo->Login, Aux->Login)>0)
			Aux = Aux->UsuProx;
		if (strcmp(Novo->Login, Aux->Login)<=0)
		{
			Novo->UsuProx = Aux;
			Novo->UsuAnt = Aux->UsuAnt;
			Aux->UsuAnt = Novo;
			Novo->UsuAnt->UsuProx = Novo;	
		}
		else
		{
			Novo->UsuAnt = Aux;
			Aux->UsuProx = Novo;
		}
	}
}

void ListarUsuarios(TpDescritor D)
{
	TpServidor *Serv;
	TpUsuario *Usu;
	char Dominio[30];
	if (D.Inicio==NULL)
		printf("\n\nLista de Servidores Vazia!\n");
	else
	{
		printf("\n\nTodos os Usuarios do Sistema (T) ou Apenas os Usuarios de um Servidor Especifico (E)?\n");
		if (toupper(getche())=='T')
		{
			Serv = D.Inicio;
			printf("\n\t** LISTA DE TODOS OS USUARIOS **\n");
			while (Serv!=NULL)
			{
				Usu = Serv->ServUsu;
				if (Usu!=NULL)
				{
					printf("-----------------------------------------\n");
					printf("| Dominio: %-28s |\n", Serv->Dominio);
					printf("-----------------------------------------\n");
					printf("| Login do Usuario              | Tipo  |\n");
					printf("-----------------------------------------\n");
					printf("|                               |       |\n");
					while (Usu!=NULL)
					{
						printf("| %-29s | %-5c |\n", Usu->Login, Usu->Tipo);
						Usu = Usu->UsuProx;
					}
					printf("-----------------------------------------\n\n");
				}
				Serv = Serv->ServProx;
			}
		}
		else
		{
			printf("\nQual Servidor?\n");
			gets(Dominio);
			Serv = BuscaServidor(D, Dominio, "");
			if (Serv!=NULL)
			{
				Usu = Serv->ServUsu;
				if (Usu!=NULL)
				{
					printf("\n\n\t** LISTA DE USUARIOS DO SERVIDOR \"%s\" **\n", Dominio);
					printf("---------------------------------\n");
					printf("| Dominio: %-20s |\n", Dominio);
					printf("---------------------------------\n");
					printf("| Login do Usuario              |\n");
					printf("---------------------------------\n");
					printf("|                               |\n");
					while (Usu!=NULL)
					{
						printf("| %-29s |\n", Usu->Login);
						Usu = Usu->UsuProx;
					}
					printf("---------------------------------\n");
				}
				else
					printf("\nServidor nao contem Usuarios!\n");
			}
			else
				printf("\nServidor nao existe!\n");
		}
	}
	getch();
}

void ConsultarUsuarios(TpDescritor D)
{
	TpServidor *Serv=D.Inicio;
	TpUsuario *Usu;
	char Login[50];
	if (Serv==NULL)
	{
		printf("\n\nLista de Servidores Vazia!\n");
		getch();
	}
	else
	{
		printf("\n\nQual Usuario deseja pesquisar?\n");
		gets(Login);
		while (strcmp(Login, "\0")!=0)
		{
			Usu = BuscaUsuario(D, Login);
			if (Usu!=NULL)
				printf("\nUsuario: %s\n", Usu->Login);
			else
				printf("\nEste Usuario nao esta cadastrado!\n");
			getch();
			printf("\nQual Usuario deseja pesquisar?\n");
			gets(Login);
		}
	}
}

//Não terminado
/* void AlterarUsuarios()
{

} */

void ExcluirUsuario(TpDescritor D, char Login[50])
{
	TpServidor *Serv=BuscaServidor(D, "", Login);
	TpUsuario *Usu=Serv->ServUsu;
	if (strcmp(Usu->Login, Login)==0)
	{
		LimparMensagens(Usu);
		Serv->ServUsu = Usu->UsuProx;
		Usu->UsuProx->UsuAnt = NULL;
		delete(Usu);
	}
	else
	{
		while (Usu!=NULL && strcmp(Usu->Login, Login)!=0)
			Usu = Usu->UsuProx;
		if(Usu->UsuProx==NULL)
		{
			LimparMensagens(Usu);
			Usu->UsuAnt->UsuProx = NULL;
		}
		else
		{
			LimparMensagens(Usu);
			Usu->UsuProx->UsuAnt = Usu->UsuAnt;
			Usu->UsuAnt->UsuProx = Usu->UsuProx;
		}
		delete(Usu);
	}
}

//Não terminado
TpMensagem* CadastrarMensagem(TpMensagem *Mens,char Login[50],char Mensagem[500], char Assunto[50])
{
	TpMensagem *Novo,*Aux;
	Novo = new TpMensagem;
	strcpy(Novo->Assunto,Assunto);
	strcpy(Novo->Mensagem,Mensagem);
	strcpy(Novo->Login,Login);
	Novo->MensProx = NULL;
	Aux = Mens;
	while(Aux->MensProx!=NULL)
		Aux = Aux->MensProx;
	Aux -> MensProx = Novo;
	Novo -> MensAnt = Aux;
	return Mens;
}

//Não terminado
TpMensagem* ExcluirMensagem(char Assunto[50],TpMensagem *Mens)
{
	TpMensagem *Aux;
	Aux = Mens;
	if(strcmp(Assunto,Mens->Assunto)==0)
	{
		Mens = Mens->MensProx;
		delete(Aux);
	}
	else
	{
		while(Aux->MensProx!=NULL && strcmp(Aux->Assunto,Assunto)!=0)
			Aux = Aux->MensProx;
		if(strcmp(Aux->Assunto,Assunto)==0)
		{
			if(Aux->MensProx==NULL)
			{
				Aux->MensAnt->MensProx = NULL;
				delete(Aux);
			}
			else
			{
				Aux->MensProx->MensAnt = Aux->MensAnt;
				Aux->MensAnt->MensProx = Aux->MensProx;
				delete(Aux);
			}
		}
		else
		{
			printf("Assunto não encontrado\n");	
		}	
	}
	return Mens;
}

//Não terminado
/* TpData MostraHoraAtual()
{
	time_t agr = time(0);
	tm* lt = localtime(&agr);
	TpData data;
	data.ano = 1900+lt->tm_year;
	data.mes = lt->tm_mon;
	data.dia = lt->tm_mday;
	data.hora = lt->tm_hour;
	data.minuto = lt->tm_min;
	return data;
}

TpServidor* LinkarUsuarios(TpServidor *Serv)
{
	TpUsuario *UAux;
	char novoD[30];
	strcpy(novoD,Serv->Dominio);
	UAux = Serv->ServUsu;
	if (UAuxF!=NULL)
	{
		while (UAux->UsuProx!=NULL)
		{
			strcpy(novoD, strtok(UAux->Login, "@"));
			strcat(novoD, '@');
			strcat(novoD, Serv->Dominio);
			UAux = UAux->UsuProx;
		}
	}
	return Serv;
}
*/
