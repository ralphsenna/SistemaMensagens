#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "TADSistema.h"

//Declaração de Funções
void MenuLogin(TpDescritor &D);

//Funções de Chamada do TAD
//Não terminado (Mensagens não incluídas)
void RecuperaListas(TpDescritor &D, FILE *ArqServ, FILE *ArqUsu, FILE *ArqMens, TpServidor *Serv, TpUsuario *Usu, TpMensagem *Mens)
{
	TpServidor SAtual;
	TpUsuario UAtual, *UAux;
	//TpMensagem MAtual;
	ArqServ = fopen("Servidores.dat", "rb");
	ArqUsu = fopen("Usuarios.dat", "rb");
	//ArqMens = fopen("Mensagens", "rb");
	rewind(ArqServ);
	rewind(ArqUsu);
	//rewind(ArqMens);
	fread(&SAtual, sizeof(TpServidor), 1, ArqServ);
	while (!feof(ArqServ))
	{
		Serv = new TpServidor;
		strcpy(Serv->Dominio, SAtual.Dominio);
		strcpy(Serv->Local, SAtual.Local);
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
		fread(&UAtual, sizeof(TpUsuario), 1, ArqUsu);
		while (!feof(ArqUsu) && BuscaServidor(D, "", UAtual.Login)!=NULL)
		{
			Usu = new TpUsuario;
			strcpy(Usu->Login, UAtual.Login);
			strcpy(Usu->Senha, UAtual.Senha);
			Usu->Tipo = UAtual.Tipo;
			Usu->UsuAnt = NULL;
			Usu->UsuProx = NULL;
			Usu->UsuMens = NULL;
			if (D.Fim->ServUsu==NULL)
				D.Fim->ServUsu = Usu;
			else
			{
				UAux->UsuProx = Usu;
				Usu->UsuAnt = UAux;

				/* D.Fim->ServUsu->UsuProx = Usu;
				Usu->UsuAnt = D.Fim->ServUsu; */
			}		
			UAux = Usu;	
			fread(&UAtual, sizeof(TpUsuario), 1, ArqUsu);
		}
		fread(&SAtual, sizeof(TpServidor), 1, ArqServ);
	}
}

//Não terminado (Mensagens não incluídas)
void SalvarListas(TpDescritor &D, TpServidor *Serv, TpUsuario *Usu, TpMensagem *Mens)
{
	FILE *ArqServ = fopen("Servidores.dat", "wb");
	FILE *ArqUsu = fopen("Usuarios.dat", "wb");
	//FILE *ArqMens = fopen("Mensagens.dat", "wb");
	TpServidor SAtual;
	TpUsuario UAtual;
	//TpMensagem MAtual;
	fseek(ArqServ, 0, 2);
	fseek(ArqUsu, 0, 2);
	//fseek(ArqMens, 0, 2);
	Serv = D.Inicio;
	while (Serv!=NULL)
	{
		strcpy(SAtual.Dominio, Serv->Dominio);
		strcpy(SAtual.Local, Serv->Local);
		SAtual.ServAnt = NULL;
		SAtual.ServProx = NULL;
		SAtual.ServUsu = NULL;
		fwrite(&SAtual, sizeof(TpServidor), 1, ArqServ);
		Usu = Serv->ServUsu;
		while (Usu!=NULL)
		{
			strcpy(UAtual.Login, Usu->Login);
			strcpy(UAtual.Senha, Usu->Senha);
			UAtual.Tipo = Usu->Tipo;
			UAtual.UsuAnt = NULL;
			UAtual.UsuProx = NULL;
			UAtual.UsuMens = NULL;
			fwrite(&UAtual, sizeof(TpUsuario), 1, ArqUsu);
			Usu = Usu->UsuProx;
		}
		strcpy(UAtual.Login, "");
		fwrite(&UAtual, sizeof(TpUsuario), 1, ArqUsu);
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
	TpServidor SAtual;
	TpUsuario UAtual;
	if (ArqServ==NULL)
	{
		ArqServ = fopen("Servidores.dat", "ab");
		ArqUsu = fopen("Usuarios.dat", "ab");
		strcpy(UAtual.Login, "radmin@admin");
		strcpy(UAtual.Senha, "123");
		UAtual.Tipo = 'A';
		UAtual.UsuAnt = NULL;
		UAtual.UsuProx = NULL;
		UAtual.UsuMens = NULL;
		fwrite(&UAtual, sizeof(TpUsuario), 1, ArqUsu);
		strcpy(SAtual.Dominio, "admin");
		strcpy(SAtual.Local, "Meu Computador");
		SAtual.ServAnt = NULL;
		SAtual.ServProx = NULL;
		SAtual.ServUsu = NULL;
		fwrite(&SAtual, sizeof(TpServidor), 1, ArqServ);
		strcpy(SAtual.Dominio, "hotmail.com");
		strcpy(SAtual.Local, "Brasil");
		SAtual.ServAnt = NULL;
		SAtual.ServProx = NULL;
		SAtual.ServUsu = NULL;
		fwrite(&SAtual, sizeof(TpServidor), 1, ArqServ);
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

void ExclusaoServidores(TpDescritor &D)
{
	TpServidor *Aux;
	char Dominio[30];
	printf("\n\n** EXCLUSAO DE SERVIDORES **\n\n");
	printf("Qual Servidor deseja excluir?\n");
	gets(Dominio);
	while (strcmp(Dominio, "\0")!=0)
	{
		if (strcmp(Dominio, "admin")!=0)
		{
			Aux = BuscaServidor(D, Dominio, "");
			if (Aux!=NULL)
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
					Usu.Tipo = toupper(getche());
				}
				else
					Usu.Tipo = 'C';
				CadastrarUsuarioOrd(D, Usu);
				printf("\n\nUsuario %s cadastrado com sucesso!\n", Usu.Login);
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

void ExclusaoUsuarios(TpDescritor &D, TpUsuario &Usuario)
{
	TpServidor *Serv;
	TpUsuario *Usu;
	char Login[50];
	printf("\n\n** EXCLUSAO DE USUARIOS **\n\n");
	if (Usuario.Tipo=='A')
	{
		printf("Qual Usuario deseja excluir?\n");
		gets(Login);
		Serv = BuscaServidor(D, "", Usuario.Login);
		if (strcmp(Usuario.Login, Login)==0 && strcmp(Serv->Dominio, "admin")!=0)
		{
			printf("\nTem certeza que deseja excluir sua Conta (S ou N)?\n");
			if (toupper(getch())=='S')
			{
				Usu = BuscaUsuario(D, Usuario.Login);
				ExcluirUsuario(D, Usuario.Login);
				printf("\nUsuario %s excluido!\n", Usuario.Login);
				strcpy(Usuario.Login, "");
			}
			else
				printf("\nOpercao Cancelada!\n");
		}
		else if (strcmp(Usuario.Login, Login)!=0)
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
			Usu = BuscaUsuario(D, Usuario.Login);
			ExcluirUsuario(D, Usuario.Login);
			printf("\n\nUsuario %s excluido!\n", Usuario.Login);
			strcpy(Usuario.Login, "");
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
	TpUsuario Usu;
	char Opcao;
	system("cls");
	printf("\t\t** LOGIN DO USUARIO **\n\n");
	printf("Login: ");
	gets(Usu.Login);
	while (strcmp(Usu.Login, "\0")!=0)
	{
		printf("Senha: ");
		gets(Usu.Senha);
		Usu.Tipo = VerificaLogin(D, Usu);
		if (Usu.Tipo=='C')
			do
			{
				Opcao = MenuUsuario(Usu.Login);
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
							break;

					case 'G':
							break;

					case 'H':
							ExclusaoUsuarios(D, Usu);
							if (strcmp(Usu.Login, "")==0)
								Opcao = 27;
							break;
				}
			}while(Opcao!=27);
		else if (Usu.Tipo=='A')
			do
			{
				Opcao = MenuAdmin(Usu.Login);
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
							//AlterarServidores();
							break;

					case 'E':
							ExclusaoServidores(D);
							break;

					case 'F':
							CadastroUsuarios(D, Usu.Tipo);
							break;

					case 'G':
							ListarUsuarios(D);
							break;

					case 'H':
							//ConsultarUsuarios();
							break;		

					case 'I':
							//AlterarUsuarios();
							break;	

					case 'J':
							ExclusaoUsuarios(D, Usu);
							if (strcmp(Usu.Login, "")==0)
								Opcao = 27;
							break;	
				}
			}while(Opcao!=27);
		else if (Usu.Tipo=='S')
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
		gets(Usu.Login);
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
