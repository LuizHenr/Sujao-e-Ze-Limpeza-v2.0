/*
------------------------------------------------------------------------------------------------------------
Copyright (c) 2014, Luiz Henrique Morais Aguiar e Cauê Zaghetto.
Brasília - DF, Brazil. All rights reserved.

The redistribution and use of this software (with or without changes)
is allowed without the payment of fees or royalties provided that:
 - source code distributions include the above copyright notice, this
   list of conditions and the following disclaimer;
 - binary distributions include the above copyright notice, this list
   of conditions and the following disclaimer in their documentation.
This software is provided 'as is' with no explicit or implied warranties
in respect of its operation, including, but not limited to, correctness
and fitness for purpose.
------------------------------------------------------------------------------------------------------------
Issue Date: 21/01/2017

 This file contains the source code of Sujão & Zé Limpeza v2.0 software.

*/

#include <graphics.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define qt_col 22
#define qt_lin 22
#define tam_cel 25
#define cmp_limpo ' '
#define cmp_obstaculo 'X'
#define bat_max 200  
#define custo_bat 2


int getPixLin(int);
int getPixCol(int);
int getLinPix(int);
int getColPix(int);
int calc_dist(int,int,int,int);
void limpa_string(char*);

struct agente{
	int col;
	int lin;
	int bateria;   
	int col_dst;
	int lin_dst;
	int passos_limpos;
};

struct sujeira{
	int col;
	int lin;
};




int main(int argc, char* argv[]){
	
				//variaveis
	int i,j;
	int posanta1=0, posanta2=0;
	char tecla = '\0';
	struct agente ag1,ag2;
	struct sujeira sjs[99]; 
	int qt_sjs = 0;   
	char campo[qt_col+2][qt_lin+2]; 
	char caminho[300];
	int atraso_vel = 100;
	int bat1=0,bat2=0;
	FILE *arq;
	
	srand(time(NULL));
	
	
	strcpy(caminho,*argv);
	limpa_string(caminho);
	strcat(caminho,"Imagens e Mapas\\");
	
			//imprime a legenda
	system("title Sujão e Zé Limpeza v2.0");
	system("color 0f");
	printf("\n\t\t     *********************************\n");
	printf("\t\t     ***  Suj\203o & Z\202 Limpeza v2.0  ***\n");
	printf("\t\t     ***  ^^^^^^^^^^^^^^^^^^^^^^^  ***\n");
	printf("\t\t     *********************************\n");
	
	printf("\n\n\t-> Legenda dos Comandos:\n");
	printf("\n  ESC   Sair do programa");
	printf("\n   +    Aumenta a velocidade");
	printf("\n   -    Diminui a velocidade");
	printf("\n SPACE  Pausa\n");
	

			//imprime o ambiente grafico
	initwindow(800,600,"Sujão & Zé limpeza v2.0");
	     
	setfillstyle(4,9);
	bar(0,0,799,799);
	
	setfillstyle(1,15);
	bar(getPixCol(-2),0,getPixCol(qt_col+2),799);
	
	setcolor(0);
	rectangle(getPixCol(-2),0,getPixCol(qt_col+2),799);
	
	
			//Imprime a tela de boas vindas e solicita a escolha do mapa
	readimagefile(strcat(caminho,"inicio.bmp"),getPixCol(-1),getPixLin(0),getPixCol(qt_col)+20,getPixLin(7));
	limpa_string(caminho);

	rectangle(getPixCol(2),getPixLin(8)-15,getPixCol(10),getPixLin(15)-15);

	readimagefile(strcat(caminho,"mapa1.bmp"),getPixCol(12),getPixLin(8)-15,getPixCol(20),getPixLin(15)-15);
	limpa_string(caminho);
	rectangle(getPixCol(12),getPixLin(8)-15,getPixCol(20),getPixLin(15)-15);
	
	readimagefile(strcat(caminho,"mapa2.bmp"),getPixCol(2),getPixLin(15)+10,getPixCol(10),getPixLin(22)+10);
    limpa_string(caminho);
	rectangle(getPixCol(2),getPixLin(15)+10,getPixCol(10),getPixLin(22)+10);
	
	readimagefile(strcat(caminho,"mapa3.bmp"),getPixCol(12),getPixLin(15)+10,getPixCol(20),getPixLin(22)+10);
	limpa_string(caminho);
	rectangle(getPixCol(12),getPixLin(15)+10,getPixCol(20),getPixLin(22)+10);
	

	
	while(tecla!=13 && tecla!=32){
	
	    if(ismouseclick(WM_LBUTTONDOWN)){
			int x,y;
			getmouseclick(WM_MOUSEMOVE, x, y);
			
			if(x>getPixCol(2) && x<getPixCol(10) && y>getPixLin(8)-15 && y<getPixLin(15)-15){
				strcat(caminho,"mapa0.txt");
				break;
			}
			if(x>getPixCol(12) && x<getPixCol(20) && y>getPixLin(8)-15 && y<getPixLin(15)-15){
				strcat(caminho,"mapa1.txt");
				break;
			}
			if(x> getPixCol(2) && x<getPixCol(10) && y>getPixLin(15)+10 && y<getPixLin(22)+10){
				strcat(caminho,"mapa2.txt");
				break;
			}
			if(x>getPixCol(12) && x<getPixCol(20) && y>getPixLin(15)+10 && y<getPixLin(22)+10){
				strcat(caminho,"mapa3.txt");
				break;
			}
			
	    }
		
		if(kbhit()){
			tecla = getch();
			if(tecla == 27)
			   	exit(0);
			if(tecla==13 || tecla ==32)
				strcat(caminho,"mapa0.txt");		   	
		}
		
	}
	

	
	
			//após o mapa ter sido escolhido, limpa o ambiente 
	setfillstyle(1,15);
	bar(getPixCol(-2),0,getPixCol(qt_col+2),799);
	
	setlinestyle(0,0,1); 
	rectangle(getPixCol(0),getPixLin(0),getPixCol(qt_col),getPixLin(qt_lin));
	
			//Carrega o arquivo do mapa escolhido
    arq= fopen(caminho,"r");
    	limpa_string(caminho);
    	j=0,i=0;
    	
		if(arq==NULL)
			printf("\n * Erro ao ler arquivo o arquivo do Mapa \a\n");
		do{
				fscanf(arq,"%c",&tecla);
				if(tecla != '\n'){
				
					if(tecla=='1')
						campo[i][j] = cmp_obstaculo;
					if(tecla=='0')
						campo[i][j] = cmp_limpo;
					if(tecla=='#')
						break;		
					
					i++;	
				}
				else{
					j++;
					i=0;
				}
			
			}while(true);	
    fclose(arq);
    
    
    
   	      //sorteia as posicoes iniciais dos agentes
	do{
		ag1.col = (rand()%qt_col);
		ag1.lin = (rand()%qt_lin);
	}while(campo[ag1.col][ag1.lin] != cmp_limpo );
	ag2.col = 0;
	ag2.lin = 0;
	
	ag1.bateria = bat_max;
	ag2.bateria = bat_max;
	
		     //sorteia as posicoes de destino dos dois agentes
	ag1.col_dst = -1;
	ag1.lin_dst = -1;
	ag2.col_dst = -1;
	ag2.lin_dst = -1;


    for(i=0;i<99;i++){
       sjs[i].col = -1;
       sjs[i].lin = -1;
    }
    

    ag1.passos_limpos = 10; //  passos ag sujao
    ag2.passos_limpos = 0; //  passos ag limpeza
	
	
	
			//desenha os obstaculos
	for(i=0;i<qt_col;i++){     
		for(j=0;j<qt_lin;j++){    
			putpixel(getPixCol(i),getPixLin(j),0);
			if(campo[i][j] == cmp_obstaculo){
				readimagefile(strcat(caminho,"obstaculo.bmp"),getPixCol(i)+1,getPixLin(j)+1,getPixCol(i)+tam_cel-1,getPixLin(j)+tam_cel-1);
				limpa_string(caminho);
			}
		}
	}
	
	
			//desenha a bateria
	readimagefile(strcat(caminho,"bateria.bmp"),getPixCol(-2)+2,getPixLin(-1)+1,getPixCol(-2)+2+50,getPixLin(-1)+1+52);
	limpa_string(caminho);
	line(getPixCol(-2),getPixLin(1)+3,getPixCol(0),getPixLin(1)+3);   
	line(getPixCol(0)+2,0,getPixCol(0)+2,getPixLin(0));  
	setcolor(WHITE);
	line(getPixCol(0),getPixLin(1),getPixCol(0),getPixLin(1)+2); 
	
	
	
			//imprime a energia do ag1
	bar(10,50,65,115);
	setcolor(0);
	rectangle(10,50,65,115);
	readimagefile(strcat(caminho,"ag1feliz.bmp"), 25+1,57+1,25+tam_cel-1,57+tam_cel-1);
	limpa_string(caminho);
	readimagefile(strcat(caminho,"raio.bmp"), 12,58+tam_cel+1,12+20,58+tam_cel*2-1);
	limpa_string(caminho);   	
	
			//imprime energia do ag2
	bar(735,50,790,115);
	setcolor(0);
	rectangle(735,50,790,115); 
	readimagefile(strcat(caminho,"ag2feliz.bmp"), 750+1,57+1,750+tam_cel-1,57+tam_cel-2);
	limpa_string(caminho);
	readimagefile(strcat(caminho,"raio.bmp"), 737,58+tam_cel+1,737+20,58+tam_cel*2-1);
	limpa_string(caminho);
	

    
	while(tecla != 27){  //ESC para sair
		
		
	   		//imprime os niveis de bateria
	   	setcolor(BLACK);
	   	setbkcolor(WHITE);
		bgiout<<ag1.bateria<<"  ";
	   	outstreamxy(32,58+tam_cel+5);		
		bgiout<<ag2.bateria<<"  ";
	   	outstreamxy(757,58+tam_cel+5);
	
	
			     //limpa o campo de visao ag2
  		if(ag2.bateria !=0){
					
			setcolor(WHITE);
			setlinestyle(1,0,2);
			
			if(ag2.lin>1)
				if(campo[ag2.col][ag2.lin-1]!= cmp_obstaculo && campo[ag2.col][ag2.lin-2]!= cmp_obstaculo){ //cima
					line(getPixCol(ag2.col)+(tam_cel/2), getPixLin(ag2.lin),getPixCol(ag2.col)+(tam_cel/2), getPixLin(ag2.lin-2)+1);
				}
			if(ag2.lin>1 && ag2.col<qt_col-2)
				if(campo[ag2.col+1][ag2.lin-1]!= cmp_obstaculo && campo[ag2.col+2][ag2.lin-2]!= cmp_obstaculo){ //diagonal sup dir
					line(getPixCol(ag2.col)+tam_cel, getPixLin(ag2.lin), getPixCol(ag2.col+2)+(tam_cel/2), getPixLin(ag2.lin-2)+ (tam_cel/2));
				}
			if(ag2.col<qt_col-2)
				if(campo[ag2.col+1][ag2.lin]!= cmp_obstaculo && campo[ag2.col+2][ag2.lin]!= cmp_obstaculo){ //direita		
					line(getPixCol(ag2.col)+tam_cel, getPixLin(ag2.lin)+(tam_cel/2),getPixCol(ag2.col+2)+tam_cel-1, getPixLin(ag2.lin)+ (tam_cel/2));
				}
			if(ag2.col<qt_col-2 && ag2.lin<qt_lin-2)
				if(campo[ag2.col+1][ag2.lin+1]!= cmp_obstaculo && campo[ag2.col+2][ag2.lin+2]!= cmp_obstaculo){ //diagonal inf dir
					line(getPixCol(ag2.col)+tam_cel, getPixLin(ag2.lin)+tam_cel,getPixCol(ag2.col+2)+(tam_cel/2), getPixLin(ag2.lin+2)+ (tam_cel/2));
				}
			if(ag2.lin<qt_lin-2)
				if(campo[ag2.col][ag2.lin+1]!= cmp_obstaculo && campo[ag2.col][ag2.lin+2]!= cmp_obstaculo){//baixo
					line(getPixCol(ag2.col)+(tam_cel/2), getPixLin(ag2.lin)+tam_cel,getPixCol(ag2.col)+(tam_cel/2), getPixLin(ag2.lin+2)+ tam_cel-1);		
				}
			if(ag2.col>1 &&ag2.lin<qt_lin-2)
				if(campo[ag2.col-1][ag2.lin+1]!= cmp_obstaculo && campo[ag2.col-2][ag2.lin+2]!= cmp_obstaculo){// diagonal inf esq
					line(getPixCol(ag2.col), getPixLin(ag2.lin)+tam_cel,getPixCol(ag2.col-2)+(tam_cel/2), getPixLin(ag2.lin+2)+ (tam_cel/2));
				}
			if(ag2.col>1)
				if(campo[ag2.col-1][ag2.lin]!= cmp_obstaculo && campo[ag2.col-2][ag2.lin]!= cmp_obstaculo){  //esquerda
					line(getPixCol(ag2.col), getPixLin(ag2.lin)+(tam_cel/2),getPixCol(ag2.col-2)+1, getPixLin(ag2.lin)+ (tam_cel/2));
				}
			if(ag2.col>1 && ag2.lin>1)
				if(campo[ag2.col-1][ag2.lin-1]!= cmp_obstaculo && campo[ag2.col-2][ag2.lin-2]!= cmp_obstaculo){//diagonal sup esq
					line(getPixCol(ag2.col), getPixLin(ag2.lin),getPixCol(ag2.col-2)+(tam_cel/2), getPixLin(ag2.lin-2)+ (tam_cel/2));
				}
					
						
					//imprime os pontos que dividem as células do grid
			for(int c=ag2.col-2;c<=ag2.col+2;c++){
				for(int l=ag2.lin-2;l<=ag2.lin+2;l++){
					if(campo[c][l]==cmp_limpo || campo[c][l]=='1' || campo[c][l]=='2'||campo[c][l]==1){
						if(c>=0 && c<qt_col && l>=0 && l<qt_lin)
							putpixel(getPixCol(c),getPixLin(l),0);
					}
				}	
			}    
						
					
		}
		


		
//             ************   Movimento do Agente 1  - Sujão   ************  
		
				//agente 1 - recarregando
		if(ag1.col==0 && ag1.lin==0){
			ag1.bateria = ag1.bateria + 5;
			if(ag1.bateria > bat_max){
				ag1.bateria = bat_max;
			}
			else{
				campo[ag1.col_dst][ag1.lin_dst] = cmp_limpo;
				
				ag1.col_dst = 0;
				ag1.lin_dst = 0;
			}
		}
		
		if(ag1.bateria!=0){
			
			
			if(ag1.col == ag1.col_dst && ag1.lin == ag1.lin_dst){    //se o ag1 atingiu seu destino
				ag1.col_dst = -1;
				ag1.lin_dst = -1;
				
				campo[ag1.col][ag1.lin] = cmp_limpo;
				
			}
			
			      
			if((qt_col+qt_lin)*2.2 > ag1.bateria*custo_bat ){  //verifica se o nivel de bateria esta baixo em relacao a distancia atual 
				campo[ag1.col_dst][ag1.lin_dst] = cmp_limpo;
				ag1.col_dst = 0;
				ag1.lin_dst = 0;
			}
			
				
			if(ag1.col_dst == -1 || ag1.lin_dst == -1){    //se não houver destino, sorteia uma posicao no tabuleiro
				
				do{
					ag1.col_dst = (rand()%qt_col);
			        ag1.lin_dst = (rand()%qt_lin);
				}while(campo[ag1.col_dst][ag1.lin_dst] != cmp_limpo);		
		        
		        campo[ag1.col_dst][ag1.lin_dst] = '1';
		        
			}
			else{      //movimento do agente 1
				
				int maisperto = 0,distperto=-1;
				int col_aux,lin_aux;
					
				col_aux = ag1.col;
				lin_aux = ag1.lin;
				campo[ag2.col][ag2.lin] = 2;
				
				
						//1=top    2=right    3=bottom   4=left				
				if(campo[ag1.col][ag1.lin-1]!=cmp_obstaculo && campo[ag1.col][ag1.lin-1]!=1){
					if(posanta1!=3){
						distperto = calc_dist(ag1.col,ag1.lin-1,ag1.col_dst,ag1.lin_dst);  	maisperto = 1;
					}  
				}
				if(campo[ag1.col+1][ag1.lin]!=cmp_obstaculo && campo[ag1.col+1][ag1.lin]!=1){
					if(posanta1!=4){
						if(distperto==-1){
							distperto = calc_dist(ag1.col+1,ag1.lin,ag1.col_dst,ag1.lin_dst);
							maisperto = 2;
						}
						else{
							if(calc_dist(ag1.col+1,ag1.lin,ag1.col_dst,ag1.lin_dst) < distperto){
								if(distperto--<0)distperto=0;  
								maisperto = 2;
							}
							else if(calc_dist(ag1.col+1,ag1.lin,ag1.col_dst,ag1.lin_dst) == distperto){
								if(rand()%2)
									maisperto = 2;
							}
						}
					}
				}
				
				if(campo[ag1.col][ag1.lin+1]!=cmp_obstaculo && campo[ag1.col][ag1.lin+1]!=1){
					if(posanta1!=1){
						if(distperto==-1){
							distperto = calc_dist(ag1.col,ag1.lin+1,ag1.col_dst,ag1.lin_dst);
							maisperto = 3;
						}
						else{
							if(calc_dist(ag1.col,ag1.lin+1,ag1.col_dst,ag1.lin_dst) < distperto){
								if(distperto--<0)distperto=0;  
								maisperto = 3;
							}
							else if(calc_dist(ag1.col,ag1.lin+1,ag1.col_dst,ag1.lin_dst) == distperto){
								if(rand()%2) 
								maisperto = 3;
							}
							
						}
					}
				}
				
				if(campo[ag1.col-1][ag1.lin]!=cmp_obstaculo && campo[ag1.col-1][ag1.lin]!=1){
					if(posanta1!=2){
						if(distperto==-1){
							distperto = calc_dist(ag1.col-1,ag1.lin,ag1.col_dst,ag1.lin_dst);
							maisperto = 4;
						}
						else{
							if(calc_dist(ag1.col-1,ag1.lin,ag1.col_dst,ag1.lin_dst) < distperto){
								if(distperto--<0)distperto=0; 
								maisperto = 4;
							}
							else if(calc_dist(ag1.col-1,ag1.lin,ag1.col_dst,ag1.lin_dst) < distperto){
								if(rand()%2)
								maisperto = 4;
							}
						
						}
					}
				}
					
							
				
				
					  	 //apaga o rastro
				setcolor(15);
				bar(getPixCol(ag1.col)+1,getPixLin(ag1.lin)+1,getPixCol(ag1.col)+tam_cel,getPixLin(ag1.lin)+tam_cel);
			    campo[ag1.col][ag1.lin] = cmp_limpo;
							
						//move o agente
				if(maisperto==1){ ag1.lin -= 1;  posanta1 = 1; }
				if(maisperto==2){ ag1.col += 1;  if(ag1.lin != 0 && ag1.col-1 != 0) posanta1 = 2; }
				if(maisperto==3){ ag1.lin += 1;  if(ag1.lin-1 != 0 && ag1.col != 0) posanta1 = 3; }
				if(maisperto==4){ ag1.col -= 1;  posanta1 = 4; }
			
				
					 	//verifica se a nova posicao excedeu os limites do campo
				if(ag1.col==-1 || ag1.col==qt_col || ag1.lin==-1 || ag1.lin==qt_lin){
					campo[ag1.col_dst][ag1.lin_dst] = cmp_limpo;
					ag1.col_dst = -1;
					ag1.lin_dst = -1;
				}
				if(ag1.col==-1)	 	ag1.col=0;
				if(ag1.col==qt_col) ag1.col=qt_col-1;
				if(ag1.lin==-1)  	ag1.lin=0;
				if(ag1.lin==qt_lin) ag1.lin=qt_lin-1;
				

						//calcula o gasto da bateria
				if(ag1.col == col_aux && ag1.lin == lin_aux){
					posanta1=0;
				}
				else{
					if(bat1==custo_bat-1){
						if(ag1.bateria--<=0) ag1.bateria++;
						bat1=0;
					}
					else{
						bat1++;
					} 
				}
				
				
						//verifica se está na hora de sujar o ambiente
				ag1.passos_limpos--;
				
				if(ag1.col!=0 && ag1.lin!=0){
					
					if(ag1.passos_limpos <= 0){  
						int i,j=-1,k=0;
						for(i=0;i<=qt_sjs;i++){
							if(sjs[i].col == -1 || sjs[i].lin == -1){
								if(j==-1)
			  						j=i;
							}
							else{
								k = i;
							}
						}
						
						qt_sjs = k+1;     
					
						if(j==-1){
							j = qt_sjs;
						}	
						
						sjs[j].col = ag1.col;
						sjs[j].lin = ag1.lin;
						
						ag1.passos_limpos = 10;	
						
					} 
				}	
		    }   
        		 
		} 
			
		campo[ag1.col][ag1.lin] = 1;
				
	

//             ************   Movimento do Agente 2  - Zé Limpeza   ************  
					 	
				// ag2 recarregando	 	
		if(ag2.col==0 && ag2.lin==0){
			ag2.bateria = ag2.bateria + 5;
			if(ag2.bateria > bat_max){
				ag2.bateria = bat_max;
			}
			else{
				campo[ag2.col_dst][ag2.lin_dst] = cmp_limpo;
				
				ag2.col_dst = 0;
				ag2.lin_dst = 0;
			}
		}
		
		if(ag2.bateria!=0){
			
			if(ag2.col == ag2.col_dst && ag2.lin == ag2.lin_dst){   // se o ag2 atingiu seu objetivo...
				
				for(int i=0;i<=qt_sjs;i++){         //se alguma sujeira fosse o objetivo, então limpa
					if(sjs[i].col != -1 && sjs[i].lin != -1){
						if(sjs[i].col == ag2.col && sjs[i].lin == ag2.lin){
					     	sjs[i].col = -1;
							sjs[i].lin = -1;
							ag2.passos_limpos=0;
						}
					}
				}
				campo[ag2.col_dst][ag2.lin_dst] = cmp_limpo;
				
				ag2.col_dst = -1;
				ag2.lin_dst = -1;
				
			}
		
		
			if((qt_col+qt_lin)*2.2 > ag2.bateria*custo_bat){  //verifica se o nivel de bateria esta baixo em relacao a distancia atual
				campo[ag2.col_dst][ag2.lin_dst] = cmp_limpo;
				ag2.col_dst = 0;
				ag2.lin_dst = 0;
			
			}
			else{
					
			    for(int i=ag2.lin-2;i<=ag2.lin+2;i++){        //verifica se há alguma sujeira no campo de visao do agente 2
					for(int j=ag2.col-2;j<=ag2.col+2;j++){
						if(i>-1 &&i<qt_lin && j>-1&&j<qt_col){
							if(qt_sjs!=0){
								for(int k=0;k<qt_sjs;k++){
									if(sjs[k].col == j && sjs[k].lin == i){
										campo[ag2.col_dst][ag2.lin_dst] = cmp_limpo;
										ag2.col_dst = sjs[k].col;
										ag2.lin_dst = sjs[k].lin; 
										break;
									}
								}
							}
						}
					}
				}
			}
		
		
		
			if(ag2.col_dst == -1 || ag2.lin_dst == -1){     //se estiver sem destino, sorteia um novo destino
				
				campo[ag2.col_dst][ag2.lin_dst] = cmp_limpo;
				
				do{
					ag2.col_dst = 2+ (rand()%(qt_col-4));
		        	ag2.lin_dst = 2+ (rand()%(qt_lin-4));
		        }while(campo[ag2.col_dst][ag2.lin_dst] != cmp_limpo);
				
		        campo[ag2.col_dst][ag2.lin_dst] = '2';
	
			}
			else{       //movimento agente 2
				
				int maisperto=0,distperto=-1;
				int col_aux,lin_aux;
				
				col_aux = ag2.col;
				lin_aux = ag2.lin;
				campo[ag1.col][ag1.lin] = 1;
				
	
				ag2.passos_limpos++;
		
				
				if(ag2.col_dst==0 && ag2.lin_dst == 0){   //verifica se existe sujeira na celula onde está
					
					for(int k=0;k<qt_sjs;k++){
						if(sjs[k].col == ag2.col && sjs[k].lin == ag2.lin){
							sjs[k].col = -1;
							sjs[k].lin = -1;
							break;
						}
					}
		
				}
				
				
				
						//1=top    2=right    3=bottom   4=left				
				if(campo[ag2.col][ag2.lin-1]!=cmp_obstaculo && campo[ag2.col][ag2.lin-1]!=1){
					if(posanta2!=3){
						distperto = calc_dist(ag2.col,ag2.lin-1,ag2.col_dst,ag2.lin_dst);  	
						maisperto = 1;
					}  
				}
				if(campo[ag2.col+1][ag2.lin]!=cmp_obstaculo && campo[ag2.col+1][ag2.lin]!=1){
					if(posanta2!=4){
						if(distperto==-1){
							distperto = calc_dist(ag2.col+1,ag2.lin,ag2.col_dst,ag2.lin_dst);
							maisperto = 2;
						}
						else{
							if(calc_dist(ag2.col+1,ag2.lin,ag2.col_dst,ag2.lin_dst) < distperto){
								if(distperto--<0)distperto=0;  
								maisperto = 2;
							}
							else if(calc_dist(ag2.col+1,ag2.lin,ag2.col_dst,ag2.lin_dst) == distperto){
								if(rand()%2)
									maisperto = 2;
							}
						}
					}
				}
				
				if(campo[ag2.col][ag2.lin+1]!=cmp_obstaculo && campo[ag2.col][ag2.lin+1]!=1){
					if(posanta2!=1){
						if(distperto==-1){
							distperto = calc_dist(ag2.col,ag2.lin+1,ag2.col_dst,ag2.lin_dst);
							maisperto = 3;
						}
						else{
							if(calc_dist(ag2.col,ag2.lin+1,ag2.col_dst,ag2.lin_dst) < distperto){
								if(distperto--<0)distperto=0;  
								maisperto = 3;
							}
							else if(calc_dist(ag2.col,ag2.lin+1,ag2.col_dst,ag2.lin_dst) == distperto){
								if(rand()%2) 
								maisperto = 3;
							}	
						}
					}
				}
				
				if(campo[ag2.col-1][ag2.lin]!=cmp_obstaculo && campo[ag2.col-1][ag2.lin]!=1){
					if(posanta2!=2){
						if(distperto==-1){
							distperto = calc_dist(ag2.col-1,ag2.lin,ag2.col_dst,ag2.lin_dst);
							maisperto = 4;
						}
						else{
							if(calc_dist(ag2.col-1,ag2.lin,ag2.col_dst,ag2.lin_dst) < distperto){
								if(distperto--<0)distperto=0; 
								maisperto = 4;
							}
							else if(calc_dist(ag2.col-1,ag2.lin,ag2.col_dst,ag2.lin_dst) < distperto){
								if(rand()%2)
								maisperto = 4;
							}
						}
					}
				}
					
							
				
				
					  	 //apaga o rastro
				setcolor(15);
				bar(getPixCol(ag2.col)+1,getPixLin(ag2.lin)+1,getPixCol(ag2.col)+tam_cel,getPixLin(ag2.lin)+tam_cel);
			    campo[ag2.col][ag2.lin] = cmp_limpo;
							
						//move o agente
				if(maisperto==1){ ag2.lin -= 1;  posanta2 = 1; }
				if(maisperto==2){ ag2.col += 1;  if(ag2.lin != 0 && ag2.col-1 != 0) 
				posanta2 = 2; }
				if(maisperto==3){ ag2.lin += 1;  if(ag2.lin-1 != 0 && ag2.col != 0) 
				posanta2 = 3; }
				if(maisperto==4){ ag2.col -= 1;  posanta2 = 4; }
				
				
						 //verifica se a posicao excedeu os limites do campo
				if(ag2.col==-1 || ag2.col==qt_col || ag2.lin==-1 || ag2.lin==qt_lin){
					campo[ag2.col_dst][ag2.lin_dst] = cmp_limpo;
					ag2.col_dst = -1;
					ag2.lin_dst = -1;
				}
				if(ag2.col==-1)	 	ag2.col=0;
				if(ag2.col==qt_col) ag2.col=qt_col-1;
				if(ag2.lin==-1)  	ag2.lin=0;
				if(ag2.lin==qt_lin) ag2.lin=qt_lin-1;
				
							//calcula o gasto da energia do agente 2
				if(ag2.col == col_aux && ag2.lin == lin_aux){
					posanta2=0;
				}
				else{
					if(bat2==custo_bat-1){
						if(ag2.bateria--<=0) ag2.bateria++;
						bat2=0;
					}
					else{
						bat2++;
					} 
					
					
				}
						
	
			}
			
			
			
			  
				     //imprime o campo de visão do agente 2
			setcolor(GREEN);
			setlinestyle(1,0,1);
						
			if(ag2.lin>1)
				if(campo[ag2.col][ag2.lin-1]!= cmp_obstaculo && campo[ag2.col][ag2.lin-2]!= cmp_obstaculo){ //cima
					line(getPixCol(ag2.col)+(tam_cel/2), getPixLin(ag2.lin),getPixCol(ag2.col)+(tam_cel/2), getPixLin(ag2.lin-2)+1);
				}
			if(ag2.lin>1 && ag2.col<qt_col-2)
				if(campo[ag2.col+1][ag2.lin-1]!= cmp_obstaculo && campo[ag2.col+2][ag2.lin-2]!= cmp_obstaculo){ //diagonal sup dir
					line(getPixCol(ag2.col)+tam_cel, getPixLin(ag2.lin), getPixCol(ag2.col+2)+(tam_cel/2), getPixLin(ag2.lin-2)+ (tam_cel/2));
				}
			if(ag2.col<qt_col-2)
				if(campo[ag2.col+1][ag2.lin]!= cmp_obstaculo && campo[ag2.col+2][ag2.lin]!= cmp_obstaculo){ //direita
					line(getPixCol(ag2.col)+tam_cel, getPixLin(ag2.lin)+(tam_cel/2),getPixCol(ag2.col+2)+tam_cel-1, getPixLin(ag2.lin)+ (tam_cel/2));
				}
			if(ag2.col<qt_col-2 && ag2.lin<qt_lin-2)
				if(campo[ag2.col+1][ag2.lin+1]!= cmp_obstaculo && campo[ag2.col+2][ag2.lin+2]!= cmp_obstaculo){ //diagonal inf dir
					line(getPixCol(ag2.col)+tam_cel, getPixLin(ag2.lin)+tam_cel,getPixCol(ag2.col+2)+(tam_cel/2), getPixLin(ag2.lin+2)+ (tam_cel/2));
				}
			if(ag2.lin<qt_lin-2)
				if(campo[ag2.col][ag2.lin+1]!= cmp_obstaculo && campo[ag2.col][ag2.lin+2]!= cmp_obstaculo){//baixo
					line(getPixCol(ag2.col)+(tam_cel/2), getPixLin(ag2.lin)+tam_cel,getPixCol(ag2.col)+(tam_cel/2), getPixLin(ag2.lin+2)+ tam_cel-1);
				}
			if(ag2.col>1 &&ag2.lin<qt_lin-2)
				if(campo[ag2.col-1][ag2.lin+1]!= cmp_obstaculo && campo[ag2.col-2][ag2.lin+2]!= cmp_obstaculo){// diagonal inf esq
					line(getPixCol(ag2.col), getPixLin(ag2.lin)+tam_cel,getPixCol(ag2.col-2)+(tam_cel/2), getPixLin(ag2.lin+2)+ (tam_cel/2));
				}
			if(ag2.col>1)
				if(campo[ag2.col-1][ag2.lin]!= cmp_obstaculo && campo[ag2.col-2][ag2.lin]!= cmp_obstaculo){  //esquerda		
					line(getPixCol(ag2.col), getPixLin(ag2.lin)+(tam_cel/2),getPixCol(ag2.col-2)+1, getPixLin(ag2.lin)+ (tam_cel/2));
				}
			if(ag2.col>1 && ag2.lin>1)
				if(campo[ag2.col-1][ag2.lin-1]!= cmp_obstaculo && campo[ag2.col-2][ag2.lin-2]!= cmp_obstaculo){//diagonal sup esq
					line(getPixCol(ag2.col), getPixLin(ag2.lin),getPixCol(ag2.col-2)+(tam_cel/2), getPixLin(ag2.lin-2)+ (tam_cel/2));
				}
			
			
		}
		 
		
		       //imprime as sujeiras
		for(int i=0;i<=qt_sjs;i++){     
			if(sjs[i].col != -1 && sjs[i].lin != -1){
				
				readimagefile(strcat(caminho,"sujeira.bmp"),getPixCol(sjs[i].col)+1,
															getPixLin(sjs[i].lin)+1,
															getPixCol(sjs[i].col)+tam_cel-1,
															getPixLin(sjs[i].lin)+tam_cel-1);
				limpa_string(caminho);
				campo[sjs[i].col][sjs[i].lin] = 'S';
				
			}
		}
		
		
		    				   	//imprime os 2 agentes
		    	//ag1
		if(ag1.col==0 && ag1.lin==0)  //dormindo base
			readimagefile(strcat(caminho,"ag1dormindo.bmp"), getPixCol(ag1.col)+1,getPixLin(ag1.lin)+1,getPixCol(ag1.col)+tam_cel-1,getPixLin(ag1.lin)+tam_cel-1);
    	else if(ag1.bateria==0)   //morto-sem energia
			readimagefile(strcat(caminho,"ag1morto.bmp"), getPixCol(ag1.col)+1,getPixLin(ag1.lin)+1,getPixCol(ag1.col)+tam_cel-1,getPixLin(ag1.lin)+tam_cel-1);
        else if(ag1.passos_limpos>7) //zombando-acabou de sujar
			readimagefile(strcat(caminho,"ag1zombando.bmp"), getPixCol(ag1.col)+1,getPixLin(ag1.lin)+1,getPixCol(ag1.col)+tam_cel-1,getPixLin(ag1.lin)+tam_cel-1);
		else if(ag1.bateria>bat_max*0.7 && ag1.bateria <=bat_max) //feliz-bateria cheia
			readimagefile(strcat(caminho,"ag1feliz.bmp"), getPixCol(ag1.col)+1,getPixLin(ag1.lin)+1,getPixCol(ag1.col)+tam_cel-1,getPixLin(ag1.lin)+tam_cel-1);
		else if(ag1.bateria>bat_max*0.35 && ag1.bateria <=bat_max*0.7) // alegre-bateria mediana
			readimagefile(strcat(caminho,"ag1alegre.bmp"), getPixCol(ag1.col)+1,getPixLin(ag1.lin)+1,getPixCol(ag1.col)+tam_cel-1,getPixLin(ag1.lin)+tam_cel-1);
    	else if(ag1.bateria>0 && ag1.bateria <=bat_max*0.35)  //preocupado-bateria baixa
			readimagefile(strcat(caminho,"ag1preocupado.bmp"), getPixCol(ag1.col)+1,getPixLin(ag1.lin)+1,getPixCol(ag1.col)+tam_cel-1,getPixLin(ag1.lin)+tam_cel-1);
    	limpa_string(caminho);
		
             	//ag2
        if(ag2.col==0 && ag2.lin==0)  //dormindo base
			readimagefile(strcat(caminho,"ag2dormindo.bmp"), getPixCol(ag2.col)+1,getPixLin(ag2.lin)+1,getPixCol(ag2.col)+tam_cel-1,getPixLin(ag2.lin)+tam_cel-1);
		else if(ag2.bateria==0)   //morto - sem energia
			readimagefile(strcat(caminho,"ag2morto.bmp"), getPixCol(ag2.col)+1,getPixLin(ag2.lin)+1,getPixCol(ag2.col)+tam_cel-1,getPixLin(ag2.lin)+tam_cel-1);
		else if(ag2.passos_limpos>bat_max && ag2.passos_limpos<=bat_max*2) //raiva-muito tempo sem limpar
			readimagefile(strcat(caminho,"ag2raiva.bmp"), getPixCol(ag2.col)+1,getPixLin(ag2.lin)+1,getPixCol(ag2.col)+tam_cel-1,getPixLin(ag2.lin)+tam_cel-1);
    	else if(ag2.passos_limpos>bat_max*2) //mais raiva - mais tempo sem limpar
			readimagefile(strcat(caminho,"ag2muitaraiva.bmp"), getPixCol(ag2.col)+1,getPixLin(ag2.lin)+1,getPixCol(ag2.col)+tam_cel-1,getPixLin(ag2.lin)+tam_cel-1);
		else if(ag2.bateria>bat_max*0.8 && ag2.bateria <=bat_max) //muito feliz-bateria cheia
			readimagefile(strcat(caminho,"ag2muitofeliz.bmp"), getPixCol(ag2.col)+1,getPixLin(ag2.lin)+1,getPixCol(ag2.col)+tam_cel-1,getPixLin(ag2.lin)+tam_cel-1);
    	else if(ag2.bateria>bat_max*0.6 && ag2.bateria <=bat_max*0.8) // feliz-bateria alta
			readimagefile(strcat(caminho,"ag2feliz.bmp"), getPixCol(ag2.col)+1,getPixLin(ag2.lin)+1,getPixCol(ag2.col)+tam_cel-1,getPixLin(ag2.lin)+tam_cel-1);
    	else if(ag2.bateria>bat_max*0.5 && ag2.bateria <=bat_max*0.6)  //alegre-bateria mediana
			readimagefile(strcat(caminho,"ag2alegre.bmp"), getPixCol(ag2.col)+1,getPixLin(ag2.lin)+1,getPixCol(ag2.col)+tam_cel-1,getPixLin(ag2.lin)+tam_cel-1);
    	else if(ag2.bateria>bat_max*0.4 && ag2.bateria <=bat_max*0.5)  //indiferente-bateria mediana
			readimagefile(strcat(caminho,"ag2indiferente.bmp"), getPixCol(ag2.col)+1,getPixLin(ag2.lin)+1,getPixCol(ag2.col)+tam_cel-1,getPixLin(ag2.lin)+tam_cel-1);
		else if(ag2.bateria>bat_max*0.2 && ag2.bateria <=bat_max*0.4)  //triste-bateria baixa
			readimagefile(strcat(caminho,"ag2triste.bmp"), getPixCol(ag2.col)+1,getPixLin(ag2.lin)+1,getPixCol(ag2.col)+tam_cel-1,getPixLin(ag2.lin)+tam_cel-1);
    	else if(ag2.bateria>0 && ag2.bateria <=bat_max*0.2) //cansado-bateria muito baixa
			readimagefile(strcat(caminho,"ag2cansado.bmp"), getPixCol(ag2.col)+1,getPixLin(ag2.lin)+1,getPixCol(ag2.col)+tam_cel-1,getPixLin(ag2.lin)+tam_cel-1);
        limpa_string(caminho);	
        	
				//imprime as faíscas na celula de recarregamento 
		if(ag1.col!=0 && ag1.lin !=0 && ag2.col!=0 && ag2.lin !=0  ){
			bar(getPixCol(0)+1,getPixLin(0)+1,getPixCol(0)+tam_cel+1,getPixLin(0)+tam_cel+1);	
		
			for(int i=0;i<20;i++){
				putpixel(getPixCol(0)+1+rand()%tam_cel,getPixLin(0)+1+rand()%tam_cel,COLOR(200,200,0));	
			}
		}
		
		
    	campo[ag2.col][ag2.lin] = 2;
    	
		
				
		if(kbhit()){    // se alguma tecla for pressionada durante a execução do programa...
		    
			tecla = getch();

			if(tecla == 32){   //espaco = pausa	
			    getch();
			}
			if(tecla == 43){   // +  =  mais velocidade
				atraso_vel -= 25;
			    if(atraso_vel<0)
			    	atraso_vel = 0;
			}
			if(tecla == 45 ){   // -  =  menos velocidade
				atraso_vel += 25;
			    if(atraso_vel>1500)
			    	atraso_vel = 1500;
			}
			
		}
			
		delay(atraso_vel);	
		
	}


    closegraph();
	return 0;

}



int getPixLin(int lin){
	lin++;
    return tam_cel*lin;
}


int getPixCol(int col){
	col++;
	return getPixLin(col)+75;
}


int getLinPix(int y){
    return (y/tam_cel) -1;
}

int getColPix(int x){
    return getLinPix(x)-4;
}


int calc_dist(int p1x,int p1y,int p2x,int p2y){
	
	return sqrt((pow(abs(p1x-p2x),2)) + (pow(abs(p1y-p2y),2)));
}

void limpa_string(char *string){
	
	for(int i= strlen(string);;i--){
		if(string[i] == '\\')
			break;
		else
			string[i] = '\0';	
	}
	
	
}


