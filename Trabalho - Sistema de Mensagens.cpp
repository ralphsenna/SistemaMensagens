#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <time.h>
#include "TADSistema.h"


//Funções de Chamada do TAD
void RecuperaListas(TpDescritor &D, FILE *ArqServ, FILE *ArqUsu, FILE *ArqMens, TpServidor *Serv, TpUsuario *Usu, TpMensagem *Mens)
{
	TpServidor RegServ;
	TpUsuario RegUsu, *UsuAux;
	TpMensagem RegMens, *MensAux;
	ArqServ = fopen("Servidores.dat", "rb");
	ArqUsu = fopen("Usuarios.dat", "rb");
	ArqMens = fopen("Mensagens.dat", "rb");
	rewind(ArqServ);
	rewind(ArqUsu);
	rewind(ArqMens);
	fread(&RegServ, sizeof(TpServidor), 1, ArqServ);
	while (!feof(ArqServ))
	{
		Serv = new TpServidor;
		strcpy(Serv->Dominio, RegServ.Dominio);
		strcpy(Serv->Local, RegServ.Local);
		Serv->ServAnt = NULL;
		Serv->ServProx = NULL;
		Serv->ServUsu = NULL;
		if (D.Inicio==NULL)
		{
			D.Inicio = Serv;
			D.Fim = Serv;
		}
		else
		{
			D.Fim->ServProx = Serv;
			Serv->ServAnt = D.Fim;
			D.Fim = Serv;
		}
		fread(&RegUsu, sizeof(TpUsuario), 1, ArqUsu);
		while (!feof(ArqUsu) && BuscaServidor(D, "", RegUsu.Login)!=NULL)
		{
			Usu = new TpUsuario;
			strcpy(Usu->Login, RegUsu.Login);
			strcpy(Usu->Senha, RegUsu.Senha);
			Usu->Tipo = RegUsu.Tipo;
			Usu->UsuAnt = NULL;
			Usu->UsuProx = NULL;
			Usu->UsuMens = NULL;
			if (D.Fim->ServUsu==NULL)
				D.Fim->ServUsu = Usu;
			else
			{
				UsuAux->UsuProx = Usu;
				Usu->UsuAnt = UsuAux;
			}		
			UsuAux = Usu;
			fread(&RegMens, sizeof(TpMensagem), 1, ArqMens);
			while (!feof(ArqMens) && BuscaUsuario(D, RegMens.LoginDest)!=NULL)
			{
				Mens = new TpMensagem;
				strcpy(Mens->Assunto, RegMens.Assunto);
				strcpy(Mens->Mensagem, RegMens.Mensagem);
				strcpy(Mens->DataHora, RegMens.DataHora);
				strcpy(Mens->LoginDest, RegMens.LoginDest);
				strcpy(Mens->LoginReme, RegMens.LoginReme);
				Mens->MensProx = NULL;
				Mens->MensAnt = NULL;
				if (UsuAux->UsuMens==NULL)
					UsuAux->UsuMens = Mens;
				else
				{
					MensAux->MensProx = Mens;
					Mens->MensAnt = MensAux;
				}
				MensAux = Mens;
				fread(&RegMens, sizeof(TpMensagem), 1, ArqMens);
			}
			fread(&RegUsu, sizeof(TpUsuario), 1, ArqUsu);
		}
		fread(&RegServ, sizeof(TpServidor), 1, ArqServ);
	}
}

void SalvarListas(TpDescritor &D, TpServidor *Serv, TpUsuario *Usu, TpMensagem *Mens)
{
	FILE *ArqServ = fopen("Servidores.dat", "wb");
	FILE *ArqUsu = fopen("Usuarios.dat", "wb");
	FILE *ArqMens = fopen("Mensagens.dat", "wb");
	TpServidor RegServ;
	TpUsuario RegUsu;
	TpMensagem RegMens;
	Serv = D.Inicio;
	while (Serv!=NULL)
	{
		strcpy(RegServ.Dominio, Serv->Dominio);
		strcpy(RegServ.Local, Serv->Local);
		RegServ.ServAnt = NULL;
		RegServ.ServProx = NULL;
		RegServ.ServUsu = NULL;
		fwrite(&RegServ, sizeof(TpServidor), 1, ArqServ);
		Usu = Serv->ServUsu;
		while (Usu!=NULL)
		{
			strcpy(RegUsu.Login, Usu->Login);
			strcpy(RegUsu.Senha, Usu->Senha);
			RegUsu.Tipo = Usu->Tipo;
			RegUsu.UsuAnt = NULL;
			RegUsu.UsuProx = NULL;
			RegUsu.UsuMens = NULL;
			fwrite(&RegUsu, sizeof(TpUsuario), 1, ArqUsu);
			Mens = Usu->UsuMens;
			while (Mens!=NULL)
			{
				strcpy(RegMens.Assunto, Mens->Assunto);
				strcpy(RegMens.Mensagem, Mens->Mensagem);
				strcpy(RegMens.DataHora, Mens->DataHora);
				strcpy(RegMens.LoginDest, Mens->LoginDest);
				strcpy(RegMens.LoginReme, Mens->LoginReme);
				RegMens.MensProx = NULL;
				RegMens.MensAnt = NULL;
				fwrite(&RegMens, sizeof(TpMensagem), 1, ArqMens);
				Mens = Mens->MensProx;
			}
			strcpy(RegMens.LoginDest, "");
			fwrite(&RegMens, sizeof(TpMensagem), 1, ArqMens);
			Usu = Usu->UsuProx;
		}
		strcpy(RegUsu.Login, "");
		fwrite(&RegUsu, sizeof(TpUsuario), 1, ArqUsu);
		Serv = Serv->ServProx;
	}
	fclose(ArqServ);
	fclose(ArqUsu);
	fclose(ArqMens);
}

void AbrindoPrograma(TpDescritor &D, TpServidor *Serv, TpUsuario *Usu, TpMensagem *Mens)
{
	FILE *ArqServ = fopen("Servidores.dat", "rb");
	FILE *ArqUsu = fopen("Usuarios.dat", "rb");
	FILE *ArqMens = fopen("Mensagens.dat", "rb");
	TpServidor RegServ;
	TpUsuario RegUsu;
	if (ArqServ==NULL)
	{
		ArqServ = fopen("Servidores.dat", "wb");
		ArqUsu = fopen("Usuarios.dat", "wb");
		ArqMens = fopen("Mensagens.dat", "wb");
		strcpy(RegUsu.Login, "radmin@admin");
		strcpy(RegUsu.Senha, "123");
		RegUsu.Tipo = 'A';
		RegUsu.UsuAnt = NULL;
		RegUsu.UsuProx = NULL;
		RegUsu.UsuMens = NULL;
		fwrite(&RegUsu, sizeof(TpUsuario), 1, ArqUsu);
		strcpy(RegServ.Dominio, "admin");
		strcpy(RegServ.Local, "Meu Computador");
		RegServ.ServAnt = NULL;
		RegServ.ServProx = NULL;
		RegServ.ServUsu = NULL;
		fwrite(&RegServ, sizeof(TpServidor), 1, ArqServ);
		strcpy(RegServ.Dominio, "hotmail.com");
		strcpy(RegServ.Local, "Brasil");
		RegServ.ServAnt = NULL;
		RegServ.ServProx = NULL;
		RegServ.ServUsu = NULL;
		fwrite(&RegServ, sizeof(TpServidor), 1, ArqServ);
		fclose(ArqServ);
		fclose(ArqUsu);
		fclose(ArqMens);
		RecuperaListas(D, ArqServ, ArqUsu, ArqMens, Serv, Usu, Mens);
	}
	else
	{
		RecuperaListas(D, ArqServ, ArqUsu, ArqMens, Serv, Usu, Mens);
		fclose(ArqServ);
		fclose(ArqUsu);
		fclose(ArqMens);
	}
}

void CadastroServidores(TpDescritor &D)
{
	TpServidor Serv;
	printf("\n\n** CADASTRO DE SERVIDORES **\n\n");
	printf("Dominio do Servidor: ");
	fflush(stdin);
	gets(Serv.Dominio);
	while (strcmp(Serv.Dominio, "\0")!=0)
	{
		if (!BuscaServidor(D, Serv.Dominio, ""))
		{
			printf("Localizacao: ");
			fflush(stdin);
			gets(Serv.Local);
			CadastrarServidorOrd(D, Serv);
			printf("\nServidor %s cadastrado com sucesso!\n", Serv.Dominio);
		}
		else
			printf("\nServidor ja existente!\n");
		getch();
		printf("\nDominio do Servidor: ");
		fflush(stdin);
		gets(Serv.Dominio);
	}
}

void AlteracaoServidores(TpDescritor D, char Login[50])
{
	TpServidor *Serv, RegServ, BackRegServ;
	TpUsuario *Usu;
	printf("\n\n** ALTERACAO DE SERVIDORES **\n\n");
	printf("Qual nome do Dominio do Servidor que deseja alterar?\n");
	fflush(stdin);
	gets(RegServ.Dominio);
	while (strcmp(RegServ.Dominio, "\0")!=0)
	{
		if (strcmp(RegServ.Dominio, "admin")!=0)
		{
			Serv = BuscaServidor(D, RegServ.Dominio, "");
			if (Serv!=NULL)
			{
				strcpy(BackRegServ.Dominio, Serv->Dominio);
				strcpy(BackRegServ.Local, Serv->Local);
				printf("\nDominio: %s\tLocal: %s\nEncontrado na lista de Servidores\n\n", Serv->Dominio, Serv->Local);
				printf("Novo Dominio: ");
				fflush(stdin);
				gets(RegServ.Dominio);
				if (!BuscaServidor(D, RegServ.Dominio, ""))
				{
					printf("Novo Local: ");
					fflush(stdin);
					gets(RegServ.Local);
					AlterarServidores(Serv, RegServ);
					printf("\nDominio: %s\tLocal: %s\n\nFoi Alterado para:", BackRegServ.Dominio, BackRegServ.Local);
					printf("\nDominio: %s\tLocal: %s\n", RegServ.Dominio, RegServ.Local);
					Usu = BuscaUsuario(D, Login);
					strcpy(Login, Usu->Login);
				}
				else
					printf("\nEste Servidor ja esta cadastrado!\n");
			}
			else
				printf("\nEste Servidor nao esta cadastrado!\n");
		}
		else
			printf("\nO servidor admin nao pode ser alterado!\n");
		getch();
		printf("\nQual nome do Dominio do Servidor que deseja alterar?\n");
		fflush(stdin);
		gets(RegServ.Dominio);
	}
}

void ExclusaoServidores(TpDescritor &D)
{
	TpServidor *Serv;
	char Dominio[30];
	printf("\n\n** EXCLUSAO DE SERVIDORES **\n\n");
	printf("Qual Servidor deseja excluir?\n");
	fflush(stdin);
	gets(Dominio);
	while (strcmp(Dominio, "\0")!=0)
	{
		if (strcmp(Dominio, "admin")!=0)
		{
			Serv = BuscaServidor(D, Dominio, "");
			if (Serv!=NULL)
			{
				ExcluirServidores(D, Dominio);
				printf("\nServidor %s excluido!\n", Dominio);
			}
			else
				printf("\nEste Servidor nao esta cadastrado!\n");
		}
		else
			printf("\nO servidor admin nao pode ser excluido!\n");
		getch();
		printf("\nQual Servidor deseja excluir?\n");
		fflush(stdin);
		gets(Dominio);
	}
}

void CadastroUsuarios(TpDescritor D, char Tipo)
{
	TpUsuario Usu;
	char Resp;
	printf("\n\n** CADASTRO DE USUARIOS **\n\n");
	printf("Login do Usuario: ");
	fflush(stdin);
	gets(Usu.Login);
	while (strcmp(Usu.Login, "\0")!=0)
	{
		Resp = SeparaDominio(D, Usu.Login);
		if (Resp=='T')
		{
			if(!BuscaUsuario(D, Usu.Login))
			{
				printf("Senha: ");
				fflush(stdin);
				gets(Usu.Senha);
				if (Tipo=='A')
				{
					printf("Tipo de Usuario (A) Administrador ou (C) Comum: ");
					toupper(getche());
				}
				else
					Usu.Tipo = 'C';
				CadastrarUsuarioOrd(D, Usu);
				printf("\n\nsuario %s cadastrado com sucesso!\n", Usu.Login);
			}
			else
				printf("\nUsuario ja existente neste Servidor!\n");
		}
		else if (Resp=='L')
			printf("\nLogin Invalido!\n");
		else
			printf("\nDominio nao existente!\n");
		getch();
		printf("\nLogin do Usuario: ");
		fflush(stdin);
		gets(Usu.Login);
	}
}

void AlteracaoUsuarios(TpDescritor D, TpUsuario &RegUsuAux)
{
	TpServidor *Serv;
	TpUsuario *Usu, RegUsu, BackRegUsu;
	printf("\n\n** ALTERACAO DE USUARIOS **\n\n");
	if (RegUsuAux.Tipo=='A')
	{
		printf("Qual conta de Usuario deseja alterar?\n");
		fflush(stdin);
		gets(RegUsu.Login);
		while (strcmp(RegUsu.Login, "\0")!=0)
		{
			Usu = BuscaUsuario(D, RegUsu.Login);
			if (Usu!=NULL)
			{
				strcpy(BackRegUsu.Login, Usu->Login);
				strcpy(BackRegUsu.Senha, Usu->Senha);
				BackRegUsu.Tipo = Usu->Tipo;
				printf("\nLogin: %s\tSenha: %s\tTipo: %c\nEncontrado na lista de Usuarios\n\n", Usu->Login, Usu->Senha, Usu->Tipo);
				printf("Novo Login (sem o Dominio): ");
				fflush(stdin);
				gets(RegUsu.Login);
				strcat(RegUsu.Login, "@");
				Serv = BuscaServidor(D, "", RegUsuAux.Login);
				strcat(RegUsu.Login, Serv->Dominio);
				if (!BuscaUsuario(D, RegUsu.Login))
				{
					printf("Nova Senha: ");
					fflush(stdin);
					gets(RegUsu.Senha);
					if (strcmp(Serv->Dominio, "admin")!=0)
					{
						printf("Novo Tipo: ");
						scanf("%c", &RegUsu.Tipo);
					}
					else
						RegUsu.Tipo = Usu->Tipo;
					AlterarUsuarios(Usu, RegUsu);
					printf("\nLogin: %s\tSenha: %s\tTipo: %c\n\nFoi Alterado para:", BackRegUsu.Login, BackRegUsu.Senha, BackRegUsu.Tipo);
					printf("\nLogin: %s\tSenha: %s\tTipo: %c\n", RegUsu.Login, RegUsu.Senha, RegUsu.Tipo);
					RegUsuAux = RegUsu;
				}
				else
					printf("\nEste Usuario ja esta cadastrado!\n");
			}
			getch();
			if (RegUsuAux.Tipo!='C')
			{
				printf("\nQual conta de Usuario deseja alterar?\n");
				fflush(stdin);
				gets(RegUsu.Login);
			}
		}
	}
	else
	{
		BackRegUsu = RegUsuAux;
		printf("\n\nLogin: %s\tSenha: %s\tTipo: %c\nEncontrado na lista de Usuarios\n\n", RegUsuAux.Login, RegUsuAux.Senha, RegUsuAux.Tipo);
		printf("Novo Login (sem o Dominio): ");
		fflush(stdin);
		gets(RegUsu.Login);
		strcat(RegUsu.Login, "@");
		Serv = BuscaServidor(D, "", RegUsuAux.Login);
		strcat(RegUsu.Login, Serv->Dominio);
		if (!BuscaUsuario(D, RegUsu.Login))
		{
			printf("Nova Senha: ");
			fflush(stdin);
			gets(RegUsu.Senha);
			RegUsu.Tipo = Usu->Tipo;
			AlterarUsuarios(Usu, RegUsu);
			printf("\nLogin: %s\tSenha: %s\tTipo: %c\n\nFoi Alterado para:", BackRegUsu.Login, BackRegUsu.Senha, BackRegUsu.Tipo);
			printf("\nLogin: %s\tSenha: %s\tTipo: %c\n", RegUsu.Login, RegUsu.Senha, RegUsu.Tipo);
			RegUsuAux = RegUsu;
		}
		else
			printf("\nEste Usuario ja esta cadastrado!\n");
		getch();
	}
}

void ExclusaoUsuarios(TpDescritor D, TpUsuario &RegUsu)
{
	TpServidor *Serv;
	TpUsuario *Usu;
	char Login[50];
	printf("\n\n** EXCLUSAO DE USUARIOS **\n\n");
	if (RegUsu.Tipo=='A')
	{
		printf("Qual Usuario deseja excluir?\n");
		fflush(stdin);
		gets(Login);
		Serv = BuscaServidor(D, "", RegUsu.Login);
		if (strcmp(RegUsu.Login, Login)==0 && strcmp(Serv->Dominio, "admin")!=0)
		{
			printf("\nTem certeza que deseja excluir sua Conta (S ou N)?\n");
			if (toupper(getch())=='S')
			{
				Usu = BuscaUsuario(D, RegUsu.Login);
				ExcluirUsuario(D, RegUsu.Login);
				printf("\nUsuario %s excluido!\n", RegUsu.Login);
				strcpy(RegUsu.Login, "");
			}
			else
			{
				printf("\nOpercao Cancelada!\n");
				getch();
			}
		}
		else if (strcmp(RegUsu.Login, Login)!=0)
			while (strcmp(Login, "\0")!=0)
			{
				Usu = BuscaUsuario(D, Login);
				if (Usu!=NULL)
				{
					ExcluirUsuario(D, Login);
					printf("\nUsuario %s excluido!\n", Login);
				}
				else
					printf("\nEste Usuario nao esta cadastrado!\n");
				getch();
				printf("\nQual Usuario deseja excluir?\n");
				fflush(stdin);
				gets(Login);
			}
		else
		{
			printf("\nUsuario Padrao Admin nao pode ser excluido!\n");
			getch();
		}
	}
	else
	{
		printf("Tem certeza que deseja excluir sua Conta (S ou N)?\n");
		if (toupper(getche())=='S')
		{
			Usu = BuscaUsuario(D, RegUsu.Login);
			ExcluirUsuario(D, RegUsu.Login);
			printf("\n\nUsuario %s excluido!\n", RegUsu.Login);
			strcpy(RegUsu.Login, "");
		}
		else
			printf("\n\nOpercao Cancelada!\n");
		getch();
	}
}

void EnviarMensagens(TpDescritor D, char LoginReme[50])
{
	TpMensagem Mens;
	printf("\n\n** ENVIO DE MENSAGENS **\n\n");
	printf("Para qual Usuario deseja enviar?\n");
	fflush(stdin);
	gets(Mens.LoginDest);
	while (strcmp(Mens.LoginDest, "\0")!=0)
	{
		if (BuscaUsuario(D, Mens.LoginDest))
		{
			printf("\nAssunto: ");
			fflush(stdin);
			gets(Mens.Assunto);
			printf("Mensagem: ");
			fflush(stdin);
			gets(Mens.Mensagem);
			strcpy(Mens.LoginReme, LoginReme);
			RecebeDataHoraAtual(Mens.DataHora);
			CadastrarMensagens(D, Mens);
			printf("\nMensagem enviada para %s com sucesso!\n", Mens.LoginDest);
		}
		else
			printf("\nUsuario nao existe!\n");
		getch();
		printf("\nPara qual Usuario deseja enviar?\n");
		fflush(stdin);
		gets(Mens.LoginDest);
	}
}

void ExclusaoMensagens(TpDescritor D, char Login[50])
{
	TpMensagem *Mens;
	char Assunto[50];
	printf("\n\n** EXCLUSAO DE SERVIDORES POR ASSUNTO **\n\n");
	printf("Qual Mensagem deseja excluir?\n");
	fflush(stdin);
	gets(Assunto);
	while (strcmp(Assunto, "\0")!=0)
	{
		Mens = BuscaMensagem(D, Login, Assunto);
		if (Mens!=NULL)
		{
			Mens = ExcluirMensagem(D, Login, Assunto);
			printf("\nAssunto: %s\nMensagem: %s\n", Mens->Assunto, Mens->Mensagem);
			printf("\nMensagem excluida!\n");
		}
		else
			printf("\nEsta Mensagem nao existe!\n");
		getch();
		printf("\nQual Mensagem deseja excluir?\n");
		fflush(stdin);
		gets(Assunto);
	}
}

char MenuUsuario(char Login[50])
{
	system("cls");
	printf("** MENU DO USUARIO **\n");
	printf("Usuario: %s\t\tComum\n\n", Login);
	printf("[A] Enviar Mensagens\n");
	printf("[B] Listar Mensagens recebidas\n");
	printf("[C] Listar Mensagens recebidas de um determinado Dia\n");
	printf("[D] Buscar Mensagens recebidas\n");
	printf("[E] Deletar Mensagens recebidas\n");

	printf("\n[F] Listar Usuarios\n");
	printf("[G] Buscar Usuarios\n");
	printf("[H] Alterar Conta\n");
	printf("[I] Deletar Conta\n");

	printf("\n[ESC] Sair do Programa\n");
	printf("\nPressione a opcao desejada: ");
	return toupper(getche());
}

char MenuAdmin(char Login[50])
{
	system("cls");
	printf("** MENU DO ADMINISTRADOR **\n");
	printf("Usuario: %s\t\tAdministrador\n\n", Login);
	printf("[A] Cadastrar Servidores\n");
	printf("[B] Listar Servidores\n");
	printf("[C] Consultar Servidores\n");
	printf("[D] Alterar Servidores\n");
	printf("[E] Deletar Servidores\n");

	printf("\n[F] Cadastrar Usuarios\n");
	printf("[G] Listar Usuarios\n");
	printf("[H] Consultar Usuarios\n");
	printf("[I] Alterar Usuarios\n");
	printf("[J] Deletar Usuarios\n");

	printf("\n[K] Enviar Mensagens\n");
	printf("[L] Listar Mensagens recebidas\n");
	printf("[M] Listar Mensagens recebidas de um determinado Dia\n");
	printf("[N] Buscar Mensagens recebidas\n");
	printf("[O] Deletar Mensagens recebidas\n");

	printf("\n[ESC] Sair do Programa\n");
	printf("\nPressione a opcao desejada: ");
	return toupper(getche());
}

void MenuLogin(TpDescritor &D)
{
	TpUsuario RegUsu;
	char Opcao;
	system("cls");
	printf("\t\t** LOGIN DO USUARIO **\n\n");
	printf("Login: ");
	fflush(stdin);
	gets(RegUsu.Login);
	while (strcmp(RegUsu.Login, "\0")!=0)
	{
		printf("Senha: ");
		fflush(stdin);
		gets(RegUsu.Senha);
		RegUsu.Tipo = VerificaLogin(D, RegUsu);
		if (RegUsu.Tipo=='C')
			do
			{
				Opcao = MenuUsuario(RegUsu.Login);
				switch(Opcao)
				{
					case 'A':
							EnviarMensagens(D, RegUsu.Login);
							break;

					case 'B':
							ListarMensagens(D, RegUsu.Login);
							break;

					case 'C':
							ListarMensagensDia(D, RegUsu.Login);
							break;

					case 'D':
							ConsultarMensagem(D, RegUsu.Login);
							break;

					case 'E':
							ExclusaoMensagens(D, RegUsu.Login);
							break;

					case 'F':
							ListarUsuarios(D);
							break;

					case 'G':
							ConsultarUsuarios(D);
							break;

					case 'H':
							AlteracaoUsuarios(D, RegUsu);
							break;

					case 'I':
							ExclusaoUsuarios(D, RegUsu);
							if (strcmp(RegUsu.Login, "")==0)
								Opcao = 27;
							break;
				}
			}while(Opcao!=27);
		else if (RegUsu.Tipo=='A')
			do
			{
				Opcao = MenuAdmin(RegUsu.Login);
				switch(Opcao)
				{
					case 'A':
							CadastroServidores(D);
							break;

					case 'B':
							ListarServidores(D);
							break;

					case 'C':
							ConsultarServidores(D);
							break;

					case 'D':
							AlteracaoServidores(D, RegUsu.Login);
							break;

					case 'E':
							ExclusaoServidores(D);
							break;

					case 'F':
							CadastroUsuarios(D, RegUsu.Tipo);
							break;

					case 'G':
							ListarUsuarios(D);
							break;

					case 'H':
							ConsultarUsuarios(D);
							break;		

					case 'I':
							AlteracaoUsuarios(D, RegUsu);
							if (RegUsu.Tipo=='C')
							{
								printf("Tipo de Usuario alterado, efutue o Login novamente!\n");
								getch();
								Opcao = 27;
							}
							break;	

					case 'J':
							ExclusaoUsuarios(D, RegUsu);
							if (strcmp(RegUsu.Login, "")==0)
								Opcao = 27;
							break;	

					case 'K':
							EnviarMensagens(D, RegUsu.Login);
							break;	

					case 'L':
							ListarMensagens(D, RegUsu.Login);
							break;	

					case 'M':
							ListarMensagensDia(D, RegUsu.Login);
							break;	

					case 'N':
							ConsultarMensagem(D, RegUsu.Login);
							break;	

					case 'O':
							ExclusaoMensagens(D, RegUsu.Login);
							break;	
				}
			}while(Opcao!=27);
		else if (RegUsu.Tipo=='S')
		{
			printf("\nSenha Incorreta!");
			getch();
		}	
		else
		{
			printf("\nUsuario Inexiste!");
			getch();
		}
		system("cls");
		printf("\t\t** LOGIN DO USUARIO **\n\n");
		printf("Login: ");
		fflush(stdin);
		gets(RegUsu.Login);
	}
}

char MenuInicial(void)
{
	system("cls");
	printf("\t\t** SISTEMA DE MENSAGENS **\n\n");
	printf("### Menu de Opcoes ###\n");
	printf("[A] Cadastrar Usuario\n");
	printf("[B] Login de Usuario\n");
	printf("\n[ESC] Sair do Programa\n");
	printf("\nPressione a opcao desejada: ");
	return toupper(getche());
}

int main(void)
{
	TpDescritor Descritor;
	TpServidor *Servidores;
	TpUsuario *Usuarios;
	TpMensagem *Mensagens;
	char Opcao;
	Inicializar(Descritor);
	AbrindoPrograma(Descritor, Servidores, Usuarios, Mensagens);
	do
	{
		Opcao = MenuInicial();
		switch(Opcao)
		{
			case 'A':
					CadastroUsuarios(Descritor, 'C');
					break;

			case 'B':
					MenuLogin(Descritor);
					break;
		}
	}while(Opcao!=27);
	SalvarListas(Descritor, Servidores, Usuarios, Mensagens);
	return 0;
}
