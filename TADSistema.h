struct TpMensagem //578B
{
	TpMensagem *MensAnt; //4B
	TpMensagem *MensProx; //4B
	char Assunto[50], Mensagem[500], DataHora[20]; //50B + 500B + 20B = 570B
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
	TpServidor *SAtual;
	char *Div, LoginBackup[50];
	strcpy(LoginBackup, Login);
	SAtual = D.Inicio;
	if (strcmp(Dominio, "")!=0)
	{
		while (SAtual!=NULL && strcmp(SAtual->Dominio, Dominio)!=0)
			SAtual = SAtual->ServProx;
	}
	else if (strcmp(Login, "")!=0)
	{
		if (strchr(Login, '@'))
		{
			Div = strtok(Login, "@");
			Div = strtok(NULL, "@");
			while (SAtual!=NULL && strcmp(SAtual->Dominio, Div)!=0)
				SAtual = SAtual->ServProx;
			strcpy(Login, LoginBackup);
		}
		else
			SAtual = NULL;
	}
	if (SAtual!=NULL)
		return SAtual;
	else
		return NULL;
}

TpUsuario* BuscaUsuario(TpDescritor D, char Login[50])
{
	TpServidor *SAtual=BuscaServidor(D, "", Login);
	TpUsuario *UAtual=SAtual->ServUsu;
	char LoginBackup[50];
	while (UAtual!=NULL && strcmp(UAtual->Login, Login)!=0)
		UAtual = UAtual->UsuProx;
	strcpy(Login, LoginBackup);
	if (UAtual!=NULL)
		return UAtual;
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
	TpServidor *Aux=D.Inicio;
	char *Div, LoginBackup[50];
	strcpy(LoginBackup, Login);
	if (strchr(Login, '@'))
	{
		Div = strtok(Login, "@");
		Div = strtok(NULL, "@");
		while (Aux!=NULL && strcmp(Aux->Dominio, Div)!=0)
			Aux = Aux->ServProx;
		strcpy(Login, LoginBackup);
		if (Aux!=NULL)
			return 'T';
		else
			return 'F';
	}
	else
		return 'L';
}

void LimparMensagens(TpUsuario *Usu)
{
	TpMensagem *MAuxF;
	MAuxF = Usu->UsuMens;
	if (MAuxF!=NULL)
	{
		while (MAuxF->MensProx!=NULL)
			MAuxF = MAuxF->MensProx;
		while (MAuxF->MensAnt!=NULL)
		{
			MAuxF = MAuxF->MensAnt;
			delete(MAuxF->MensProx);
		}
		delete(MAuxF);
		Usu->UsuMens = NULL;
	}
}

void LimparUsuarios(TpServidor *Serv)
{
	TpUsuario *UAuxF;
	UAuxF = Serv->ServUsu;
	if (UAuxF!=NULL)
	{
		while (UAuxF->UsuProx!=NULL)
			UAuxF = UAuxF->UsuProx;
		while (UAuxF->UsuAnt!=NULL)
		{
			//LimparMensagens(UAuxF);
			UAuxF = UAuxF->UsuAnt;
			delete(UAuxF->UsuProx);
		}
		delete(UAuxF);
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
			Aux = D.Inicio;
			while (strcmp(Serv.Dominio, Aux->ServProx->Dominio)>0)
                Aux = Aux->ServProx;
			Novo->ServProx = Aux->ServProx;
			Novo->ServAnt = Aux;
			Aux->ServProx = Novo;
			Aux->ServProx->ServAnt = Novo;
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
		getch();
	}
}

void ConsultarServidores(TpDescritor D)
{
	TpServidor *Aux=D.Inicio;
	char Dominio[30];
	if (Aux==NULL)
		printf("\n\nLista de Servidores Vazia!\n");
	else
	{
		printf("\n\nQual nome do Dominio do Servidor que procura?\n");
		gets(Dominio);
		while (strcmp(Dominio, "\0")!=0)
		{
			Aux = BuscaServidor(D, Dominio, "");
			if (Aux!=NULL)
				printf("\nDominio: %s\tLocal: %s\n", Aux->Dominio, Aux->Local);
			else
				printf("\nEste Servidor nao esta cadastrado!\n");
			getch();
			printf("\nQual nome do Dominio do Servidor que procura?\n");
			gets(Dominio);
		}
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
		Aux = Serv->ServUsu->UsuProx;
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
	TpServidor *ServAux;
	TpUsuario *UsuAux;
	char Dominio[30];
	if (D.Inicio==NULL)
		printf("\n\nLista de Servidores Vazia!\n");
	else
	{
		printf("\n\nTodos os Usuarios do Sistema (T) ou Apenas os Usuarios de um Servidor Especifico (E)?\n");
		if (toupper(getch())=='T')
		{
			ServAux = D.Inicio;
			printf("\n\t** LISTA DE TODOS OS USUARIOS **\n");
			while (ServAux!=NULL)
			{
				UsuAux = ServAux->ServUsu;
				if (UsuAux!=NULL)
				{
					printf("-----------------------------------------\n");
					printf("| Dominio: %-20s |\n", ServAux->Dominio);
					printf("-----------------------------------------\n");
					printf("| Login do Usuario              | Tipo  |\n");
					printf("-----------------------------------------\n");
					printf("|                               |       |\n");
					while (UsuAux!=NULL)
					{
						printf("| %-29s | %-5c |\n", UsuAux->Login, UsuAux->Tipo);
						UsuAux = UsuAux->UsuProx;
					}
					printf("-----------------------------------------\n\n");
				}
				ServAux = ServAux->ServProx;
			}
		}
		else
		{
			printf("\nQual Servidor?\n");
			gets(Dominio);
			ServAux = BuscaServidor(D, Dominio, "");
			if (ServAux!=NULL)
			{
				UsuAux = ServAux->ServUsu;
				if (UsuAux!=NULL)
				{
					printf("\n\n\t** LISTA DE USUARIOS DO SERVIDOR \"%s\" **\n", Dominio);
					printf("---------------------------------\n");
					printf("| Dominio: %-20s |\n", Dominio);
					printf("---------------------------------\n");
					printf("| Login do Usuario              |\n");
					printf("---------------------------------\n");
					printf("|                               |\n");
					while (UsuAux!=NULL)
					{
						printf("| %-29s |\n", UsuAux->Login);
						UsuAux = UsuAux->UsuProx;
					}
					printf("---------------------------------\n");
				}
				else
					printf("\nServidor nao contem Usuarios!\n");
			}
			else
				printf("\nServidor nao existe!\n");
		}
		getch();
	}
}

//Não terminado
TpUsuario* ExcluirUsuario(TpUsuario *Usu,char Login[50])
{
	TpUsuario *UsuAux;
	UsuAux = Usu;
	if(strcmp(UsuAux->Login,Login)==0)
	{
		Usu->UsuProx->UsuAnt = Usu;
		Usu = Usu->UsuProx;
		delete(UsuAux);
	}
	else
	{
		while(UsuAux->UsuProx!=NULL && strcmp(UsuAux->Login,Login)!=0)
			UsuAux = UsuAux -> UsuProx;
		if(strcmp(UsuAux->Login,Login)==0)
		{
			if(UsuAux->UsuProx==NULL)
			{
				LimparMensagens(UsuAux);
				UsuAux->UsuAnt->UsuProx = NULL;
			}
			else
			{
				LimparMensagens(UsuAux);
				UsuAux->UsuProx->UsuAnt = UsuAux->UsuAnt;
				UsuAux->UsuAnt->UsuProx = UsuAux->UsuProx;
			}
			delete(UsuAux);
			printf("Usuario deletado\n");
		}
		else
		{
			printf("Usuario nao encontrado\n");
		}
	}
	return Usu;
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
			Aux->MensProx->MensAnt = Aux->MensAnt;
			Aux->MensAnt->MensProx = Aux->MensProx;
			delete(Aux);
		}
		else
		{
			printf("Assunto não encontrado\n");	
		}	
	}
	return Mens;
}