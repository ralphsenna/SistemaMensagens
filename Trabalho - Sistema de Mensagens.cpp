#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "TADSistema.h"


//Funções de Chamada do TAD
//Não terminado (Mensagens não incluídas)
void RecuperaListas(TpDescritor &D, FILE *ArqServ, FILE *ArqUsu, FILE *ArqMens, TpServidor *Serv, TpUsuario *Usu, TpMensagem *Mens)
{
	TpServidor RegServ;
	TpUsuario RegUsu, *UsuAux;
	//TpMensagem MAtual;
	ArqServ = fopen("Servidores.dat", "rb");
	ArqUsu = fopen("Usuarios.dat", "rb");
	//ArqMens = fopen("Mensagens", "rb");
	rewind(ArqServ);
	rewind(ArqUsu);
	//rewind(ArqMens);
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
			fread(&RegUsu, sizeof(TpUsuario), 1, ArqUsu);
		}
		fread(&RegServ, sizeof(TpServidor), 1, ArqServ);
	}
}

//Não terminado (Mensagens não incluídas)
void SalvarListas(TpDescritor &D, TpServidor *Serv, TpUsuario *Usu, TpMensagem *Mens)
{
	FILE *ArqServ = fopen("Servidores.dat", "wb");
	FILE *ArqUsu = fopen("Usuarios.dat", "wb");
	//FILE *ArqMens = fopen("Mensagens.dat", "wb");
	TpServidor RegServ;
	TpUsuario RegUsu;
	//TpMensagem MAtual;
	fseek(ArqServ, 0, 2);
	fseek(ArqUsu, 0, 2);
	//fseek(ArqMens, 0, 2);
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
			Usu = Usu->UsuProx;
		}
		strcpy(RegUsu.Login, "");
		fwrite(&RegUsu, sizeof(TpUsuario), 1, ArqUsu);
		Serv = Serv->ServProx;
	}
	fclose(ArqServ);
	fclose(ArqUsu);
	//fclose(ArqMens);
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
		ArqServ = fopen("Servidores.dat", "ab");
		ArqUsu = fopen("Usuarios.dat", "ab");
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
		RecuperaListas(D, ArqServ, ArqUsu, ArqMens, Serv, Usu, Mens);
	}
	else
		RecuperaListas(D, ArqServ, ArqUsu, ArqMens, Serv, Usu, Mens);
	fclose(ArqServ);
	fclose(ArqUsu);
	fclose(ArqMens);
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
				gets(RegServ.Dominio);
				if (!BuscaServidor(D, RegServ.Dominio, ""))
				{
					printf("Novo Local: ");
					gets(RegServ.Local);
					AlterarServidor(Serv, RegServ);
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
		gets(RegServ.Dominio);
	}
}

void ExclusaoServidores(TpDescritor &D)
{
	TpServidor *Serv;
	char Dominio[30];
	printf("\n\n** EXCLUSAO DE SERVIDORES **\n\n");
	printf("Qual Servidor deseja excluir?\n");
	gets(Dominio);
	while (strcmp(Dominio, "\0")!=0)
	{
		if (strcmp(Dominio, "admin")!=0)
		{
			Serv = BuscaServidor(D, Dominio, "");
			if (Serv!=NULL)
			{
				ExcluirServidor(D, Dominio);
				printf("\nServidor %s excluido!\n", Dominio);
			}
			else
				printf("\nEste Servidor nao esta cadastrado!\n");
		}
		else
			printf("\nO servidor admin nao pode ser excluido!\n");
		getch();
		printf("\nQual Servidor deseja excluir?\n");
		gets(Dominio);
	}
}

void CadastroUsuarios(TpDescritor &D, char Tipo)
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
					scanf("%c", &Usu.Tipo);
				}
				else
					Usu.Tipo = 'C';
				CadastrarUsuarioOrd(D, Usu);
				printf("\nUsuario %s cadastrado com sucesso!\n", Usu.Login);
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
				gets(RegUsu.Login);
				strcat(RegUsu.Login, "@");
				Serv = BuscaServidor(D, "", RegUsuAux.Login);
				strcat(RegUsu.Login, Serv->Dominio);
				if (!BuscaUsuario(D, RegUsu.Login))
				{
					printf("Nova Senha: ");
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
				gets(RegUsu.Login);
			}
		}
	}
	else
	{
		BackRegUsu = RegUsuAux;
		printf("\n\nLogin: %s\tSenha: %s\tTipo: %c\nEncontrado na lista de Usuarios\n\n", RegUsuAux.Login, RegUsuAux.Senha, RegUsuAux.Tipo);
		printf("Novo Login (sem o Dominio): ");
		gets(RegUsu.Login);
		strcat(RegUsu.Login, "@");
		Serv = BuscaServidor(D, "", RegUsuAux.Login);
		strcat(RegUsu.Login, Serv->Dominio);
		if (!BuscaUsuario(D, RegUsu.Login))
		{
			printf("Nova Senha: ");
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

void ExclusaoUsuarios(TpDescritor &D, TpUsuario &RegUsu)
{
	TpServidor *Serv;
	TpUsuario *Usu;
	char Login[50];
	printf("\n\n** EXCLUSAO DE USUARIOS **\n\n");
	if (RegUsu.Tipo=='A')
	{
		printf("Qual Usuario deseja excluir?\n");
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
				printf("\nOpercao Cancelada!\n");
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
				gets(Login);
			}
		else
			printf("\nUsuario Padrao Admin nao pode ser excluido!\n");
		getch();
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

//Não terminado
char MenuUsuario(char Login[50])
{
	system("cls");
	printf("** MENU DO USUARIO **\n");
	printf("Usuario: %s\t\tComum\n\n", Login);
	printf("[A] Enviar Mensagens\n");
	printf("[B] Listar Mensagens recebidas\n");
	printf("[C] Buscar Mensagens recebidas\n");
	printf("[D] Deletar Mensagens recebidas\n");

	printf("\n[E] Listar Usuarios\n");
	printf("[F] Buscar Usuarios\n");
	printf("[G] Alterar Conta\n");
	printf("[H] Deletar Conta\n");

	printf("\n[ESC] Sair do Programa\n");
	printf("\nPressione a opcao desejada: ");
	return toupper(getche());
}

//Não terminado
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
	printf("[M] Buscar Mensagens recebidas\n");
	printf("[N] Deletar Mensagens recebidas\n");

	printf("\n[ESC] Sair do Programa\n");
	printf("\nPressione a opcao desejada: ");
	return toupper(getche());
}

//Não terminado
void MenuLogin(TpDescritor &D)
{
	TpUsuario RegUsu;
	char Opcao;
	system("cls");
	printf("\t\t** LOGIN DO USUARIO **\n\n");
	printf("Login: ");
	gets(RegUsu.Login);
	while (strcmp(RegUsu.Login, "\0")!=0)
	{
		printf("Senha: ");
		gets(RegUsu.Senha);
		RegUsu.Tipo = VerificaLogin(D, RegUsu);
		if (RegUsu.Tipo=='C')
			do
			{
				Opcao = MenuUsuario(RegUsu.Login);
				switch(Opcao)
				{
					case 'A':
							break;

					case 'B':
							break;

					case 'C':
							break;

					case 'D':
							break;

					case 'E':
							ListarUsuarios(D);
							break;

					case 'F':
							ConsultarUsuarios(D);
							break;

					case 'G':
							AlteracaoUsuarios(D, RegUsu);
							break;

					case 'H':
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
							break;	

					case 'L':
							break;	

					case 'M':
							break;	

					case 'N':
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
