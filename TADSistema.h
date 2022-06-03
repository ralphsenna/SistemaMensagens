//Declaração de Structs

struct TpMensagem //688B
{
	TpMensagem *MensAnt; //4B
	TpMensagem *MensProx; //4B
	char Assunto[50], Mensagem[500], DataHora[30], LoginDest[50], LoginReme[50]; //50B + 500B + 30B + 50B = 50B = 680B
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
	TpUsuario *Usu;
	if (Serv!=NULL)
	{
		Usu = Serv->ServUsu;
		if (strchr(Login, '@'))
		{
			while (Usu!=NULL && strcmp(Usu->Login, Login)!=0)
				Usu = Usu->UsuProx;
			if (Usu!=NULL)
				return Usu;
			else
				return NULL;
		}
		else
			return NULL;
	}
}

TpMensagem* BuscaMensagem(TpDescritor D, char Login[50], char Assunto[50])
{
	TpUsuario *Usu=BuscaUsuario(D, Login);
	TpMensagem *Mens;
	if (Mens!=NULL)
	{
		Mens = Usu->UsuMens;
		while (Mens!=NULL && strcmp(Mens->Assunto, Assunto)!=0)
			Mens = Mens->MensProx;
		if (Mens!=NULL)
			return Mens;
		else
			return NULL;
	}
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

void RecebeDataHoraAtual(char DataHora[30])
{
	time_t Tempo = time(0);
	tm* DataHoraAtual = localtime(&Tempo);
	char Aux[10], DataHoraMens[30];
	strcpy(DataHoraMens, "\0");

	//Dia, Mês e Ano
	itoa(DataHoraAtual->tm_mday, Aux, 10);
	if (DataHoraAtual->tm_mday<10)
		strcat(DataHoraMens, "0");
	strcat(DataHoraMens, Aux);
	strcat(DataHoraMens, "/");
	itoa(DataHoraAtual->tm_mon+1, Aux, 10);
	if (DataHoraAtual->tm_mon<10)
		strcat(DataHoraMens, "0");
	strcat(DataHoraMens, Aux);
	strcat(DataHoraMens, "/");
	itoa(DataHoraAtual->tm_year+1900, Aux, 10);
	strcat(DataHoraMens, Aux);
	
	//Separador da Data e Hora
	strcat(DataHoraMens, " ");

	//Hora e Minuto
	itoa(DataHoraAtual->tm_hour, Aux, 10);
	if (DataHoraAtual->tm_hour<10)
		strcat(DataHoraMens, "0");
	strcat(DataHoraMens, Aux);
	strcat(DataHoraMens, ":");
	itoa(DataHoraAtual->tm_min, Aux, 10);
	if (DataHoraAtual->tm_min<10)
		strcat(DataHoraMens, "0");
	strcat(DataHoraMens, Aux);

	strcpy(DataHora, DataHoraMens);
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

void LinkarMensagens(TpMensagem *Mens, char NovoLogin[50])
{
	if (Mens!=NULL)
		while (Mens!=NULL)
		{
			strcpy(Mens->LoginDest, NovoLogin);
			Mens = Mens->MensProx;
		}
}

void LinkarUsuarios(TpUsuario *Usu, char NovoDominio[30])
{
	TpMensagem *Mens=Usu->UsuMens;
	char NovoLogin[50], *Div;
	if (strcmp(NovoDominio, "")!=0)
		if (Usu!=NULL)
			while (Usu!=NULL)
			{
				Div = strtok(Usu->Login, "@");
				strcpy(NovoLogin, Div);
				strcat(NovoLogin, "@");
				strcat(NovoLogin, NovoDominio);
				strcpy(Usu->Login, NovoLogin);
				LinkarMensagens(Mens, NovoLogin);
				Usu = Usu->UsuProx;
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
		printf("\n\n\t** LISTA DE TODOS OS SERVIDORES **\n");
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
		printf("\n\n** CONSULTA DE SERVIDORES **\n\n");
		printf("Qual nome do Dominio do Servidor que procura?\n");
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

void AlterarServidores(TpServidor *Serv, TpServidor RegServ)
{
	TpUsuario *Usu=Serv->ServUsu;
	strcpy(Serv->Dominio, RegServ.Dominio);
	strcpy(Serv->Local, RegServ.Local);
	LinkarUsuarios(Usu, RegServ.Dominio);
} 

void ExcluirServidores(TpDescritor &D, char Dominio[30])
{
	TpServidor *Aux;
	if (strcmp(D.Inicio->Dominio, Dominio)==0)
	{
		Aux = D.Inicio;
		LimparUsuarios(Aux);
		D.Inicio = D.Inicio->ServProx;
		D.Inicio->ServAnt = NULL;
	}
	else if (strcmp(D.Fim->Dominio, Dominio)==0)
	{
		Aux = D.Fim;
		LimparUsuarios(Aux);
		D.Fim = D.Fim->ServAnt;
		D.Fim->ServProx = NULL;
	}
	else
	{
		Aux = D.Inicio;
		while (strcmp(Aux->Dominio, Dominio)!=0)
			Aux = Aux->ServProx;	
		LimparUsuarios(Aux);
		Aux->ServProx->ServAnt = Aux->ServAnt;
		Aux->ServAnt->ServProx = Aux->ServProx;
	}
	delete(Aux);
}

void CadastrarUsuarioOrd(TpDescritor D, TpUsuario Usu)
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
			printf("\nQual Servidor?");
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
		printf("\n\n** CONSULTA DE USUARIOS **\n\n");
		printf("Qual Usuario deseja pesquisar?\n");
		fflush(stdin);
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
			fflush(stdin);
			gets(Login);
		}
	}
}

void AlterarUsuarios(TpUsuario *Usu, TpUsuario RegUsu)
{
	TpMensagem *Mens=Usu->UsuMens;
	strcpy(Usu->Login, RegUsu.Login);
	strcpy(Usu->Senha, RegUsu.Senha);
	Usu->Tipo = RegUsu.Tipo;
	LinkarMensagens(Mens, RegUsu.Login);
}

void ExcluirUsuario(TpDescritor D, char Login[50])
{
	TpServidor *Serv=BuscaServidor(D, "", Login);
	TpUsuario *Usu=Serv->ServUsu;
	if (strcmp(Usu->Login, Login)==0)
	{
		LimparMensagens(Usu);
		Serv->ServUsu = Usu->UsuProx;
		Usu->UsuProx->UsuAnt = NULL;
	}
	else
	{
		while (Usu->UsuProx!=NULL && strcmp(Usu->Login, Login)!=0)
			Usu = Usu->UsuProx;
		if (Usu->UsuProx==NULL)
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
	}
	delete(Usu);
}

void CadastrarMensagens(TpDescritor D, TpMensagem Mens)
{
	TpUsuario *Usu=BuscaUsuario(D, Mens.LoginDest);
	TpMensagem *Novo, *Aux;
	Novo = new TpMensagem;
	strcpy(Novo->Assunto, Mens.Assunto);
	strcpy(Novo->Mensagem, Mens.Mensagem);
	strcpy(Novo->DataHora, Mens.DataHora);
	strcpy(Novo->LoginDest, Mens.LoginDest);
	strcpy(Novo->LoginReme, Mens.LoginReme);
	Novo->MensProx = NULL;
	Novo->MensAnt = NULL;
	if (Usu->UsuMens==NULL)
		Usu->UsuMens = Novo;
	else
	{
		Aux = Usu->UsuMens;
		while (Aux->MensProx!=NULL)
			Aux = Aux->MensProx;
		Aux->MensProx = Novo;
		Novo->MensAnt = Aux;
	}
}

void ListarMensagens(TpDescritor D, char Login[50])
{
	TpUsuario *Usu=BuscaUsuario(D, Login);
	TpMensagem *Mens;
	if (D.Inicio==NULL)
		printf("\n\nLista de Servidores Vazia!\n");
	else
	{
		Mens = Usu->UsuMens;
		printf("\n\n\t** LISTA DE TODAS AS MENSAGENS DO USUARIO **\n");
		printf("--------------------------------------------------------------------\n");
		while (Mens!=NULL)
		{
			printf("| Remetente: %-53s |\n", Mens->LoginReme);
			printf("|                                                                  |\n");
			printf("| Destinatario: %-50s |\n", Mens->LoginDest);
			printf("--------------------------------------------------------------------\n");
			printf("| Assunto: %-55s |\n", Mens->Assunto);
			printf("|                                                                  |\n");
			printf("| Mensagem: %-54s |\n", Mens->Mensagem);
			printf("|                                                                  |\n");
			printf("|                                                                  |\n");
			printf("| %-64s |\n", Mens->DataHora);
			printf("--------------------------------------------------------------------\n");
			printf("|                                                                  |\n");
			printf("--------------------------------------------------------------------\n");
			Mens = Mens->MensProx;
		}
	}
	getch();
}

void ListarMensagensDia(TpDescritor D, char Login[50])
{
	TpUsuario *Usu=BuscaUsuario(D, Login);
	TpMensagem *Mens;
	bool Achou=false;
	char DataPesquisa[30], DataAux[30];
	int i;
	if (D.Inicio==NULL)
		printf("\n\nLista de Servidores Vazia!\n");
	else
	{
		printf("\n\nData a pesquisar (DD/MM/AAAA): ");
		fflush(stdin);
		gets(DataPesquisa);
		Mens = Usu->UsuMens;
		while (Mens!=NULL)
		{
			for (i=0; Mens->DataHora[i]!=' '; i++)
					DataAux[i] = Mens->DataHora[i];
			DataAux[i] = '\0';
			if (strcmp(DataAux, DataPesquisa)==0)
				Achou = true;
			Mens = Mens->MensProx;
		}
		if (Achou==true)
		{
			Mens = Usu->UsuMens;
			printf("\n\t** LISTA DE TODAS AS MENSAGENS DO DIA %s **\n", DataPesquisa);
			printf("--------------------------------------------------------------------\n");
			while (Mens!=NULL)
			{
				if (strcmp(DataAux, DataPesquisa)==0)
				{
					for (i=0; Mens->DataHora[i]!=' '; i++)
						DataAux[i] = Mens->DataHora[i];
					DataAux[i] = '\0';
					printf("| Remetente: %-53s |\n", Mens->LoginReme);
					printf("|                                                                  |\n");
					printf("| Destinatario: %-50s |\n", Mens->LoginDest);
					printf("--------------------------------------------------------------------\n");
					printf("| Assunto: %-55s |\n", Mens->Assunto);
					printf("|                                                                  |\n");
					printf("| Mensagem: %-54s |\n", Mens->Mensagem);
					printf("|                                                                  |\n");
					printf("|                                                                  |\n");
					printf("| %-64s |\n", Mens->DataHora);
					printf("--------------------------------------------------------------------\n");
					printf("|                                                                  |\n");
					printf("--------------------------------------------------------------------\n");
					Mens = Mens->MensProx;
				}
			}
		}
		else
			printf("\nNao ha Mensagens com esta Data!\n");
	}
	getch();
}

void ConsultarMensagem(TpDescritor D, char Login[50])
{
	TpServidor *Serv=D.Inicio;
	TpMensagem *Mens;
	char Assunto[50];
	if (Serv==NULL)
	{
		printf("\n\nLista de Servidores Vazia!\n");
		getch();
	}
	else
	{
		printf("\n\n** CONSULTA DE SERVIDORES **\n\n");
		printf("Qual o Assunto da Mensagem que esta procurando?\n");
		fflush(stdin);
		gets(Assunto);
		while (strcmp(Assunto, "\0")!=0)
		{
			Mens = BuscaMensagem(D, Login, Assunto);
			if (Mens!=NULL)
			{
				printf("\nRemetente: %s\nDestinatario: %s", Mens->LoginReme, Mens->LoginDest);
				printf("\nAssunto: %s\nMensagem: %s\n%s\n", Mens->Assunto, Mens->Mensagem, Mens->DataHora);
			}
			else
				printf("\nNao ha Mensagens com esse Assunto!\n");
			getch();
			printf("\nQual o Assunto da Mensagem que esta procurando?\n");
			fflush(stdin);
			gets(Assunto);
		}
	}
}

TpMensagem* ExcluirMensagem(TpDescritor D, char Login[50], char Assunto[50])
{
	TpUsuario *Usu=BuscaUsuario(D, Login);
	TpMensagem *Mens=Usu->UsuMens, *BackMens;
	if (strcmp(Assunto, Mens->Assunto)==0)
	{
		Usu->UsuMens = Mens->MensProx;
		Mens->MensProx->MensAnt = NULL;
	}
	else
	{
		while (Mens->MensProx!=NULL && strcmp(Mens->Assunto, Assunto)!=0)
			Mens = Mens->MensProx;
		if (Mens->MensProx==NULL)
			Mens->MensAnt->MensProx = NULL;
		else
		{
			Mens->MensProx->MensAnt = Mens->MensAnt;
			Mens->MensAnt->MensProx = Mens->MensProx;
		}
	}
	BackMens = Mens;
	delete(Mens);
	return BackMens;
}

//Outra versão de Mensagens
/* void ListarMensagensN(TpDescritor D, char Login[50])
{
	TpUsuario *Usu=BuscaUsuario(D, Login);
	TpMensagem *Mens;
	int i;
	if (D.Inicio==NULL)
		printf("\n\nLista de Servidores Vazia!\n");
	else
	{
		Mens = Usu->UsuMens;
		printf("\n\n\t** LISTA DE TODAS AS MENSAGENS DO USUARIO **\n");
		printf("--------------------------------------------------------------------\n");
		while (Mens!=NULL)
		{
			printf("| Remetente: %-53s |\n", Mens->LoginReme);
			printf("|                                                                  |\n");
			printf("| Destinatario: %-50s |\n", Mens->LoginDest);
			printf("--------------------------------------------------------------------\n");
			printf("| Assunto: %-55s |\n", Mens->Assunto);
			printf("|                                                                  |\n");
			printf("| Mensagem: %-54s |\n", Mens->Mensagem);
			for (i=0; Mens->Mensagem[i]!='\0'; i++)
				printf("")/
			printf("|                                                                  |\n");
			printf("|                                                                  |\n");
			printf("| %-64s |\n", Mens->DataHora);
			printf("--------------------------------------------------------------------\n");
			printf("|                                                                  |\n");
			printf("--------------------------------------------------------------------\n");
			Mens = Mens->MensProx;
		}
	}
	getch();
} */
