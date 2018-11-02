/*
 * File: dbox.c
 *     Dialog Box support.
 * 
 * Descri��o: 
 *     Ger�ncia de caixas de di�logo.
 *     Faz parte do m�dulo Window Manager do tipo MB.
 *     Obs: Dialogbox � basicamente um 'form'.
 *
 * History:
 *     2015 - Created by fred Nora.
 */

 
#include <kernel.h>


//Prot�tipo do procedimento.
unsigned long 
DialogBoxProcedure ( struct window_d *window, 
                     int msg, 
					 unsigned long long1, 
					 unsigned long long2 );
					 
					 
					 
					 
					 
					 

/*
//rotina interna de suporta ao mouseHandler 
#define _MOUSE_X_SIGN 0x10
#define _MOUSE_Y_SIGN 0x20

void
update_mouse1()
{

char x = (mouse_packet_data & _MOUSE_X_SIGN);
char y = ( mouse_packet_data & _MOUSE_Y_SIGN );


//do_x:
	    //x 
		//checando o sinal para x.
		//se for diferente de 0 ent�o x � negativo
		if(x != 0)
		{
			//complemento de 2.
			mouse_packet_x = ~mouse_packet_x + 1;
			mouse_x = mouse_x - mouse_packet_x;
		}else{

			mouse_x = mouse_x + mouse_packet_x;
		}

		
//do_y:		
		//y
		//se for diferente de 0 ent�o y � negativo
		if(y != 0)
		{
			//complemento de 2.
			mouse_packet_y = ~mouse_packet_y + 1;
			mouse_y = mouse_y + mouse_packet_y;
		}else{
			mouse_y = mouse_y - mouse_packet_y;
		};		
		
    return;		
};
*/





/*
 * mouse_write:
 *     Envia um byte para a porta 0x60.
 *     (Nelson Cole) 
 */
void mouse_write (unsigned char write){
	
	kbdc_wait(1);
	outportb (0x64,0xD4);
	
	kbdc_wait(1);
	outportb (0x60,write);
};


/*
 * mouse_read:
 *     Pega um byte na porta 0x60.
 *     (Nelson Cole) 
 */
unsigned char mouse_read (){
	
	kbdc_wait(0);
	
	return inportb(0x60);
};



/*
 * getMouseData:
 *     Essa fun��o � usada pela rotina kernelPS2MouseDriverReadData.
 * Input a value from the keyboard controller's data port, after checking
 * to make sure that there's some mouse data there for us.
 */
static unsigned char getMouseData (void){
	
    unsigned char data = 0;

    while ((data & 0x21) != 0x21)
        kernelProcessorInPort8(0x64, data);

    kernelProcessorInPort8(0x60, data);
	
	return (data);
};



/*
 * kernelPS2MouseDriverReadData:  
 *     Pega os bytes e salva em um array.
 *     Exibe um caractere na tela, dado as cordenadas.
 *     This gets called whenever there is a mouse interrupt
 *     @todo: Rever as entradas no array.         
 */
void kernelPS2MouseDriverReadData (void){
    //#suspensa !	
};





/*
 **********************************************************
 * load_mouse_bmp:  ## teste ##
 *     Carregando o arquivo MOUSE.BMP que � o ponteiro de mouse.
 *     Usar isso na inicializa��o do mouse.
 *     #bugbug isso pode ir para windowLoadGramadoIcons
 */
int load_mouse_bmp (){
	
	int Status = 1;
	int Index;
    struct page_frame_d *pf;
	unsigned long fileret;
	
#ifdef KERNEL_VERBOSE
	printf("load_mouse_bmp:\n");
#endif	
	
	// #Importante
	// Fizemos um teste e funcionou com 500 p�ginas.
	// Foi suficiente para testarmos um pano de fundo.
	// Ret = (void*) allocPageFrames(500);  
	
    // Alocando duas p�ginas para um BMP pequeno. 8KB.	
	mouseBMPBuffer = (void *) allocPageFrames(2);
	//mouseBMPBuffer = (void*) allocPageFrames(10);
	//mouseBMPBuffer = (void*) allocPageFrames(100);  //400KB
	
	if ( (void *) mouseBMPBuffer == NULL ){
		
	    printf("unblocked-ldisc-load_mouse_bmp: mouseBMPBuffer\n");
		goto fail;		
	};
	
	
	// #debug
	//printf("\n");
	//printf("BaseOfList={%x} Showing #32 \n",mouseBMPBuffer);
    //for(Index = 0; Index < 32; Index++)   	
	//{  
    //    pf = (void*) pageframeAllocList[Index]; 
	//	
	//	if( (void*) pf == NULL ){
	//	    printf("null\n");	 
	//	}
	//   if( (void*) pf != NULL ){
	//	    printf("id={%d} used={%d} magic={%d} free={%d} handle={%x} next={%x}\n",
	//          pf->id ,pf->used ,pf->magic ,pf->free ,pf ,pf->next ); 	
	//	}
	//}
	
	//
	// ## Loading ...  ##
	//
	
	//read_fntos( (char *) arg1);
	
	
	//===================================
	// MOUSE
	fileret = (unsigned long) fsLoadFile( "MOUSE   BMP", 
	                            (unsigned long) mouseBMPBuffer );
								  
	if( fileret != 0 )
	{
		printf("MOUSE.BMP FAIL\n");		
		// Escrevendo string na janela.
		//draw_text( gui->main, 10, 500, COLOR_WINDOWTEXT, "MOUSE.BMP FAIL");
		goto fail;	
	};
	
	// Render BMP file on backbuffer.
	//bmpDisplayBMP( mouseBMPBuffer, 20, 20 );
	//refresh_rectangle( 20, 20, 16, 16 );	
    //===================================							
	
	
	//===================================
	//DENNIS
	//fileret = (unsigned long) fsLoadFile( "DENNIS  BMP", 
	//                              (unsigned long) mouseBMPBuffer );
								  
	//if( fileret != 0 )
	//{
	//	printf("DENNIS  BMP FAIL\n");		
		// Escrevendo string na janela.
		//draw_text( gui->main, 10, 500, COLOR_WINDOWTEXT, "MOUSE.BMP FAIL");
	//	goto fail;	
	//};
	
	// Render BMP file on backbuffer.
	//bmpDisplayBMP( mouseBMPBuffer, 0, 0 );
    //refresh_rectangle( 0, 0, 800, 600 );	
	//refresh_rectangle( 20, 40, 400, 400 );
	//===================================
	
	
//refresh_rectangle:

	//Isso funcionou ...
	//refresh_rectangle( 20, 20, 16, 16 );
	Status = (int) 0;
	goto done;
	
fail:
    printf("fail\n");
    Status = (int) 1;		
done:

#ifdef KERNEL_VERBOSE
    printf("done\n");
#endif	
	//refresh_screen();
    return (int) Status;	
};



//?? isso t� sem prot�tipo ??
void set_current_mouse_responder ( int i ){
	
    current_mouse_responder = i;	
};

//?? isso t� sem prot�tipo ??
int get_current_mouse_responder (){
	
    return (int) current_mouse_responder;	
};
					 



					 
					 
					 


/*
 **************
 * mouseHandler:
 *     Handler de mouse. 
 *
 * *Importante: 
 *     Se estamos aqui � porque os dados dispon�veis no 
 * controlador 8042 pertencem ao mouse.
 *
 * @todo: Essa rotina n�o pertence ao line discipline.
 * Obs: Temos externs no in�cio desse arquivo.
 * 
 */ 
#define MOUSE_DATA_BIT 1
#define MOUSE_SIG_BIT  2
#define MOUSE_F_BIT  0x20
#define MOUSE_V_BIT  0x08 

//contador
static int count_mouse = 0;

// Buffer.
static char buffer_mouse[3];


int flagRefreshMouseOver;
	
void mouseHandler (){
	
    // #importante:
	// Essa ser� a thread que receber� a mensagem
	struct thread_d *t;
	
	//#bugbug:
	//usaremos isso provis�riamente para evitar acessar estrutura 
	//inv�lida.
	//t = (struct thread_d *) threadList[current_thread];
	//if ( (void *) t == NULL )
	//{
		//printf("mouseHandler: debug current thread fail\n")
	//	return;
	//}
	
	// #importante:
	// Essa ser� a janela afetada por qualquer
	// evento de mouse.
	
    struct window_d *Window;

	// ID de janela.
	
	int wID; 	
	
	
	// Coordenadas do mouse.
    // Obs: Isso pode ser global.
    // O tratador em assembly tem as vari�veis globais do posicionamento.
    
	int posX=0;
    int posY=0;	

    //Char para o cursor provis�rio.
	//#todo: Isso foi subtituido por uma bmp. Podemos deletar.
    //static char mouse_char[] = "T";

	char *_byte;


	// Lendo um char no controlador.
	
	*_byte = (char) mouse_read();
	
	
	// #importante:
	// Contagem de interru��es:
	// Obs: Precisamos esperar 3 interrup��es.
	
	//#bugbug essa vari�vel est� inicializando toda vez que 
	//se chama o handler porque ela � interna. ??
	
	switch ( count_mouse )
	{
		// Essa foi a primeira interrup��o.
		case 0:
		    //Pegamos o primeiro char.
		    buffer_mouse[0] = (char) *_byte;
            if(*_byte & MOUSE_V_BIT)
                count_mouse++;
		    break;
			
		// Essa foi a segunda interrup��o.	
		case 1:
		    //Pegamos o segundo char.
		    buffer_mouse[1] = (char) *_byte;
			count_mouse++;
		    break;
			
		// Essa foi a terceira interrup��o.	
		case 2:
		    //Pegamos o terceiro char.
            buffer_mouse[2] = (char) *_byte;
			count_mouse = 0;
			
			//#importante:
			//A partir de agora j� temos os tr�s chars.
			
			//??
			//Colocando os tr�s chars em vari�veis globais.
            //Isso ficar� assim caso n�o aja overflow ...
			mouse_packet_data = buffer_mouse[0];
	 	    mouse_packet_x = buffer_mouse[1];       
		    mouse_packet_y = buffer_mouse[2];
            
			// #importante:
			// #todo: Isso est� em Assembly, l� em hwlib.inc, 
			// mas queremos que seja feito em C.
			// #obs: Uma rotina interna aqui nesse arquivo est� tentando isso.
			
		    //salvando antes de atualizar.
			saved_mouse_x = mouse_x;
		    saved_mouse_y = mouse_y;			
			
			update_mouse();			
			
		    // #importante:
			// Agora vamos manipular os valores obtidos atrav�s da 
			// fun��o de atualiza��o dos valores.
			
			mouse_x = (mouse_x & 0x00000FFF ); 
		    mouse_y = (mouse_y & 0x00000FFF );
		
		    // Checando limites.
            // #todo: Os valores foram determinados. 
			// Precisamos usar vari�veis.
			
			if( mouse_x < 1 ){ mouse_x = 1; }	
		    if(	mouse_y < 1 ){ mouse_y = 1; }
		    if(	mouse_x > (800-16) ){ mouse_x = (800-16); }
		    if(	mouse_y > (600-16) ){ mouse_y = (600-16); }

		    // # Draw BMP #
		    // Isso est� funcionando bem.
			// #todo: precisamos colocar no backbuffer o ret�ngulo salvo previamente.
            // #todo: Precisamos salvar o conte�do de um ret�ngulo 
			// que est� no backbuffer, para depois pintarmos o mouse.
						
			
			//  copiar para o lfb o antigo ret�ngulo. Para apagar o ponteiro que est� no lfb.
			refresh_rectangle ( saved_mouse_x, saved_mouse_y, 20, 20 );	
			
			bmpDisplayMousePointerBMP ( mouseBMPBuffer, mouse_x, mouse_y );


            //nova t�cnica.
            //+ copia no LFB um ret�ngulo do backbuffer para apagar o ponteiro antigo.
            //+ decodifica o mouse diretamente no LFB.			
			
            break;

        default:
		    count_mouse = 0;
            break;		
	};
	
	
	// #importante 
	// Por outro lado o mouse deve confrontar seu posicionamento com 
	// todas as janelas, para saber se as coordenadas atuais est�o passando 
	// por alguma das janelas. Se estiver, ent�o enviaremos mensagens para essa 
	// janela que o mouse passou por cima. Ela deve ser sinalizada como hover,
	// 
	// #importante:
	// Se houver um click, o elemento com mousehover ser� afetado, e 
	// enviaremos mesagens para ele, se apertarem enter ou application key, 
	// quem recebe a mensagem � o first responder, ou seja, a janela com o 
	// foco de entrada.
    // Se clicarmos com o bot�o da direita, quem recebe a mensagem � 
    // a janela que o mouse est� passando por cima.	
	
	
	//
	//  ## Button ##
	//
	
	// ===
	//Apenas obtendo o estado dos bot�es.
	
	mouse_buttom_1 = 0;
	mouse_buttom_2 = 0;
	mouse_buttom_3 = 0;
	
	if( ( mouse_packet_data & 0x01 ) == 0 )
	{
		//liberada.
		mouse_buttom_1 = 0;
	}else if( ( mouse_packet_data & 0x01 ) != 0 )
	    {
		    //pressionada.
		    //N�o tem como pressionar mais de um bot�o por vez.
	        mouse_buttom_1 = 1;
	        mouse_buttom_2 = 0;
	        mouse_buttom_3 = 0;		  
	    }
			  

	if( ( mouse_packet_data & 0x02 ) == 0 )
	{
	    //liberada.
	    mouse_buttom_2 = 0;
	}else if( ( mouse_packet_data & 0x02 ) != 0 )
	    {
		    //pressionada.
		    //N�o tem como pressionar mais de um bot�o por vez.
	        mouse_buttom_1 = 0;
	        mouse_buttom_2 = 1;
	        mouse_buttom_3 = 0;		  			 
	    }
			  
	if( ( mouse_packet_data & 0x04 ) == 0 )
	{
	    //liberada.
	    mouse_buttom_3 = 0;
	}else if( ( mouse_packet_data & 0x04 ) != 0 )
	    {
	        //pressionada.
		    //N�o tem como pressionar mais de um bot�o por vez.			 
	        mouse_buttom_1 = 0;
	        mouse_buttom_2 = 0;
	        mouse_buttom_3 = 1;		  			
	    }			
	
	
    // ===	
	// Confrontando o estado atual com o estado anterior para saber se ouve 
	// alguma altera��o ou n�o.
	// 1 = ouve altera��o.
	
	if( mouse_buttom_1 != old_mouse_buttom_1 ||
	    mouse_buttom_2 != old_mouse_buttom_2 ||
		mouse_buttom_3 != old_mouse_buttom_3 )
	{
		mouse_button_action = 1;
	}else{
		mouse_button_action = 0;
	};
	
	// #refletindo: 
	// ?? E no caso de apenas considerarmos que o mouse est� se movendo, 
	// mandaremos para janela over. ???
	
	// #refletindo:
	// Obs: A mensagem over pode ser enviada apenas uma vez. 
	// ser� usada para 'capturar' o mouse ... 
	// e depois tem a mensagem para 'descapturar'.
	
	//
	//  ## Scan ##
	//
	
	//===========
	// (capture) - On mouse over. 
	//

	
	
	// wID = ID da janela.
	// Escaneamos para achar qual janela bate com os valores indicados.
	// Ou seja. Sobre qual janela o mouse est� passando.

	// #IMPORTANTE
	// Se for v�lido e diferente da atual, significa que 
	// estamos dentro de uma janela.
	// -1 significa que ouve algum problema no escaneamento.
	
	wID = (int) windowScan ( mouse_x, mouse_y );	
	
	if ( wID == -1 )
	{ 
        
		//essa flag indica que podemos fazer o refresh da mouse ouver,
		//mas somente uma vez.
		if ( flagRefreshMouseOver == 1 )
		{
		    Window = (struct window_d *) windowList[mouseover_window];	
		    
			//#bugbug:
			//precisamos checar a validade da estrutura antes de usa-la.
			
			if ( (void *) Window != NULL ){
			    refresh_rectangle ( Window->left, Window->top, 20, 20 );
			}
			
			//n�o podemos mais fazer refresh.
			flagRefreshMouseOver = 0;
			
			//#importante
			//inicializa.
			mouseover_window = 0;
		}
		
		//Nothing.
		
    }else{
		
		Window = (struct window_d *) windowList[wID];
		
		if ( (void *) Window == NULL )
		{
			//fail
			return;
		}
		
		if ( Window->used != 1 || Window->magic != 1234)
		{
			//fail
			return;
		}
			
		//#importante:
		//Nesse momento temos uma janela v�lida, ent�o devemos 
		//pegar a thread associada � essa janela, dessa forma 
		//enviaremos a mensagem para a thread do aplicativo ao qual 
		//a janela pertence.
		
		t = (void *) Window->InputThread;

		if ( (void *) t == NULL )
		{
			//fail
			return;
		}
		
		//#bugbug 
		//#todo:
		// Deveriamos aqui checarmos a validade da estrutura ??
		
		//redraw_window(wScan);
        
        // #importante
		// Se um bot�o foi pressionado ou liberado, ent�o enviaremos uma 
		// mensagem relativa ao estado do bot�o, caso contr�rio, enviaremos 
		// uma mensagem sobre a movimenta��o do mouse.
		
		//#importante
		// Se houve mudan�a em rela��o ao estado anterior.
		if ( mouse_button_action == 1 )
		{
			//Qual bot�o mudou seu estado??
			//Checaremos um por um.
			
			//1
			//Igual ao estado anterior
			if( mouse_buttom_1 == old_mouse_buttom_1 )
			{
				//...
			
			//Diferente do estado anterior.
			}else{
				
				//down
				if ( mouse_buttom_1 == 1 )
				{                
					
					//clicou
					if ( old_mouse_buttom_1 == 0 ){
						
						//#importante 
						//enviaremos a mensagem para a thread atual.
						
						if ( (void *) Window != NULL ){
						
                            t->window = Window;
						    t->msg = MSG_MOUSEKEYDOWN;
							t->long1 = 1;
							t->long2 = 0;
							
							t->newmessageFlag = 1;
						}
										    
					    //atualiza o estado anterior.
					    old_mouse_buttom_1 = 1;
					}
					
				}else{
					
					//up
					
					//#importante 
					//enviaremos a mensagem para a thread atual.
						
					if ( (void *) Window != NULL ){
						
                        t->window = Window;
					    t->msg = MSG_MOUSEKEYUP;
						t->long1 = 1;
						t->long2 = 0;
							
						t->newmessageFlag = 1;
					}						
						
					old_mouse_buttom_1 = 0;	
				}
			}; 
			
			
			//2
			//Igual ao estado anterior
			if ( mouse_buttom_2 == old_mouse_buttom_2 )
			{
				//...
				
			//Diferente do estado anterior.	
			}else{
				
				//down
				if ( mouse_buttom_2 == 1 )
				{
					
					//clicou
					if( old_mouse_buttom_2 == 0 ){
						
						
						if ( (void *) Window != NULL ){
						
                            t->window = Window;
						    t->msg = MSG_MOUSEKEYDOWN;
							t->long1 = 2;
							t->long2 = 0;
							
							t->newmessageFlag = 1;
						}						
				    
					    //atualiza o estado anterior.
					    old_mouse_buttom_2 = 1;
					}

                }else{
					
					//up
					if ( (void *) Window != NULL ){
						
                        t->window = Window;
					    t->msg = MSG_MOUSEKEYUP;
						t->long1 = 2;
						t->long2 = 0;
							
						t->newmessageFlag = 1;
					}	
						
					old_mouse_buttom_2 = 0;
				}
			}; 
			
			
			//3
			//Igual ao estado anterior
			if ( mouse_buttom_3 == old_mouse_buttom_3 )
			{
				//...
				
				
			//Diferente do estado anterior.	
			}else{
				
				//down
				if ( mouse_buttom_3 == 1 )
				{
					//clicou
					if ( old_mouse_buttom_3 == 0 ){
                        
						
						if ( (void *) Window != NULL ){
						
                            t->window = Window;
						    t->msg = MSG_MOUSEKEYDOWN;
							t->long1 = 3;
							t->long2 = 0;
							
							t->newmessageFlag = 1;
						}	 						
				    
					    //atualiza o estado anterior.
					    old_mouse_buttom_3 = 1;
					}

                }else{
					
					//up
					if ( (void *) Window != NULL ){
						
                        t->window = Window;
					    t->msg = MSG_MOUSEKEYUP;
						t->long1 = 3;
						t->long2 = 0;
							
						t->newmessageFlag = 1;
					}	
						
					old_mouse_buttom_3 = 0;
				}
			}; 
			
			//A��o conclu�da.
			mouse_button_action = 0;
			
			
		    // Se N�O ouve altera��o no estado dos bot�es, ent�o apenas 
		    // enviaremos a mensagem de movimento do mouse e sinalizamos 
		    // qual � a janela que o mouse est� em cima.
		}else{
			
			// #importante
			// Lembrando que estamos dentro de uma janela ...
			// por isso a mensagem � over e n�o move.
			
            //Obs: Se a janela for a mesma que capturou o mouse,
			//ent�o n�o precisamos reenviar a mensagem.
			//Mas se o mouse estiver em cima de uma janela diferente da 
			//que ele estava anteriormente, ent�o precisamos enviar uma 
			//mensagem pra essa nova janela.
			
			//#bugbug:
			//estamos acessando a estrutura, mas precisamos antes saber se ela � v�lida.
			
			if ( (void *) Window != NULL )
			{
			
                //;;;
			    if ( Window->id != mouseover_window )
			    {
				
                    if ( mouseover_window != 0 )
				    {
					
					    //if ( (void *) Window != NULL ){
						
                        t->window = (struct window_d *) windowList[mouseover_window];
					    t->msg = MSG_MOUSEEXITED;
						t->long1 = 0;
						t->long2 = 0;
						
						t->newmessageFlag = 1;
					    //}	
				    };
				
				
				    //Agora enviamos uma mensagem pra a nova janela que o mouse 
				    //est� passando por cima.
						
                    t->window = Window;
					t->msg = MSG_MOUSEOVER;
					t->long1 = 0;
					t->long2 = 0;
						
					t->newmessageFlag = 1;
				
			
			        //ja que entramos em uma nova janela, vamos mostra isso.
				
				    //bot�o.
			        if ( Window->isButton == 1 )
				    {    
			            bmpDisplayCursorBMP ( fileIconBuffer, Window->left, Window->top );	
			        };
				
				    //n�o bot�o.
				    if ( Window->isButton == 0 )
				    {
				        bmpDisplayCursorBMP ( folderIconBuffer, Window->left, Window->top );		
				    };
				
				    //nova mouse over
				    mouseover_window = Window->id;
				
				    //#importante:
				    //flag que ativa o refresh do mouseover somente uma vez.
				    flagRefreshMouseOver = 1;
			
			    }else{ 
			    
				    //nothing ...
				    //n�o precisamos reenviar a mensagem, pois o mouse 
				    //continua na mesma janela que antes.
                
				    //windowSendMessage ( (unsigned long) wScan, 
		            //    (unsigned long) MSG_MOUSEOVER, 
			        //    (unsigned long) 0, 
			        //    (unsigned long) 0 );				
				
			    };			
			    
				//;;;
			};
			
			//A��o conclu�da.
			//Para o caso de um valor incostante na flag.
			mouse_button_action = 0;			
		};
		
		
		//if ( wScan->isButton == 1 )
		//{
			//colocamos ao passarmos por cima do bot�o,
			//#obs: tem que dar refresh do ret�ngulo no backbuffer quando sair
			//substituir essa imagem.
		//    bmpDisplayCursorBMP ( cursorIconBuffer, wScan->left, wScan->top);	
		//}
		
        //if ( wScan->isButton == 0 )
        //{
		    // #debug. (+)
		    // Isso coloca o (+) no frontbuffer.
		//    draw_text ( wScan, 0, 0, COLOR_YELLOW, "+" );
		//    refresh_rectangle ( wScan->left, wScan->top, 8, 8 );
		//}			
		
	};

    // EOI.		
    outportb ( 0xa0, 0x20 ); 
    outportb ( 0x20, 0x20 );
};





/* 
 * **************
 * P8042_install:
 *     Configurando o controlador PS/2, 
 *     e activar a segunda porta PS/2 (mouse).
 *     (Nelson Cole)
 */
void ps2_mouse_initialize (){
	
	unsigned char status;

    // Desativar dispositivos PS/2 , isto evita que os dispositivos PS/2 
	// envie dados no momento da configura��o.

//desablePorts:
	
	// Desativar a primeira porta PS/2.
  	kbdc_wait(1);
	outportb(0x64,0xAD);  
	
	// Desativar a segunda porta PS/2, 
	// hahaha por default ela j� vem desativada, s� para constar
	kbdc_wait(1);
	outportb(0x64,0xA7); 

//goAhead:
	
	 // Defina a leitura do byte actual de configura��o do controlador PS/2.
	kbdc_wait(1);    
	outportb(0x64,0x20);    

	// Activar o segundo despositivo PS/2, modificando o status de 
	// configura��o do controlador PS/2. 
	// Lembrando que o bit 1 � o respons�vel por habilitar, desabilitar o 
	// segundo despositivo PS/2  ( o rato). 
	// S� para constar se vedes aqui fizemos duas coisas lemos ao mesmo tempo 
	// modificamos o byte de configura��o do controlador PS/2 
	
	kbdc_wait(0);
	status = inportb(0x60)|2;  
	
	// defina, a escrita  de byte de configura��o do controlador PS/2.
	kbdc_wait(1);
	outportb(0x64,0x60);  

	// devolvemos o byte de configura��o modificado.
	kbdc_wait(1);
	outportb(0x60,status);  

	// Obs:
	// Agora temos dois dispositivos seriais teclado e mouse (PS/2).

    // Reabilitando portas.
	
//enablePorts:
	
	// Ativar a primeira porta PS/2.
	kbdc_wait(1);
	outportb(0x64,0xAE);   

	// Ativar a segunda porta PS/2.
	kbdc_wait(1);
	outportb(0x64,0xA8);  

    // Done!
	
	// espera.
	// ?? Pra que isso ??
	kbdc_wait(1);  
    
    // NOTA. 
	// Esta configura��o discarta do teste do controlador PS/2 e de seus dispositivos. 
	// Depois fa�amos a configura��o decente e minuciosa do P8042.
};



					 
					 
					 
					 

/*
 ***********************************************
 * init_mouse:
 *     Inicializando o mouse no controlador 8042.
 *     Carregando o bmp para o curso do mouse.
 */		
int ps2_mouse_globals_initialize (){
	
    unsigned char response = 0;
    unsigned char deviceId = 0;
    int i; 
	int bruto = 1;  //M�todo.
	int mouse_ret;
	
	
	//user.h
	ioControl_mouse = (struct ioControl_d *) malloc( sizeof(struct ioControl_d) );
	
	if ( (void *) ioControl_mouse == NULL )
	{
		printf("ldsic-init_mouse: ioControl_mouse fail");
		die();
	}else{
	    
	    ioControl_mouse->id = 0;
	    ioControl_mouse->used = 1;
	    ioControl_mouse->magic = 1234;
	    
		//Qual thread est� usando o dispositivo.
		ioControl_mouse->tid = 0;  
	    //ioControl_mouse->
	};	
	
	//
	// Estamos espa�o para o buffer de mensagens de mouse.
	// mousemsg = ( unsigned char *) malloc(32);

		
	//Inicializando as vari�veis usadas na rotina em Assemly
    //em hardwarelib.inc
    
	//Coordenadas do cursor.
	g_mousepointer_x = (unsigned long) 0;
    g_mousepointer_y = (unsigned long) 0;	
    mouse_x = 0;
    mouse_y = 0;
	
	//mouse_x = 0;
	//mouse_y = 0;
	
	//#bugbug: Essa inicializa��o est� travando o mouse.
	//fazer com cuidado.
	
	//#bugbug. Cuidado com essa inicializa�ao.
	g_mousepointer_width = 16;
	g_mousepointer_height = 16;
	


    //Bytes do controlador.
   // mouse_packet_data = 0;
   // mouse_packet_x = 0;
    //mouse_packet_y = 0;
    //mouse_packet_scroll = 0;
	
	
	//Mostraremos essa mensagem somente no ambiente de debug.
	
#ifdef KERNEL_VERBOSE	
    MessageBox(gui->screen, 1, "init_mouse:","initializing!");
#endif   
	
	//
	// Poderemos tentar de mais de um modo.
	// Obs: O modo bruto est� funcionando. 
	//
	
	
//tryModoBruto:	
	
	//Modo bruto.
	//Obs: Esse modo est� funcionando.
	/*
	if(bruto == 1){
	    mouse_write(0xFF);
	    mouse_write(0xF6); 
	    mouse_write(0xF4); 
		//while(!0xFA)mouse_read();
		while (mouse_read() != 0xfa);   // ACK
	};
	*/
	
 // Reseta mouse (reset ? lento!)...
  // Espero pelo byte 0xaa que encerra a sequ?ncia
  // de reset!
  kbdc_wait(1);
  mouse_write(0xff);
  while (mouse_read() != 0xaa);

  // Restaura defaults do PS/2 mouse.
  kbdc_wait(1);
  mouse_write(0xf6);
  while (mouse_read() != 0xfa);


// TODO: Pode ser interessante diminuir a sensibilidade do mouse
  // aqui!!!

  // Habilita o mouse streaming
  // Interessante notar que, no modo streaming,
  // 1 byte recebido do PS/2 mouse gerar  uma IRQ...
  // Talvez valha a pena DESABILITAR o modo streaming
  // para colher os 3 dados de uma s� vez na IRQ!
  kbdc_wait(1);
  mouse_write(0xf4);
  while (mouse_read() != 0xfa);         // ACK
  
	
	//
	// Aqui podemos tentar outros modos mais completos.
	//
	
//done:

    // Reabilitando as duas portas.
	


	// Ativar a segunda porta PS/2.
	kbdc_wait(1);
	outportb(0x64,0xA8); 


	//
	// ## BMP ##
	//
	
	//
	// Carregando o bmp do disco para a mem�ria
	// e apresentando pela primeira vez.
	//
	
	// ## test ##
	//susenso. Isso funciona.
    
	mouse_ret = (int) load_mouse_bmp ();	
	if (mouse_ret != 0)
	{
		printf("ldisc-init_mouse: load_mouse_bmp");
		die();
	}
	
#ifdef KERNEL_VERBOSE		
    MessageBox(gui->screen, 1, "init_mouse:","Mouse initialized!");   
#endif  

    //initialized = 1;
    //return (kernelDriverRegister(mouseDriver, &defaultMouseDriver));	
	return (int) 0;
};



					 
					 
					 
					 
					 
					 
					 
					 
					 

					 
								  
/*
 * DialogBox:
 *     Cria uma janela do tipo 3 (normal) e 
 * mostra uma mensagem.
 *
 */
void DialogBox ( struct window_d *parent_window, 
                 int type, 
				 char *title, 
				 char *string )
{

/*

    ### cancelada. Essa rotina agora est� na API em user mode. deletar essa fun��o ###	
	
	struct window_d *hWnd; 
	struct window_d *pWnd; 
	struct window_d *bWnd;
	
    //ret�ngulo abaixo do grid.
	
	//x
	unsigned long x  = (unsigned long) (3*(800/20));	 //>
    unsigned long cx = (unsigned long) (14*(800/20));    //sobra 3 de cada lado.
	
	//y
	unsigned long y  = (unsigned long) (2*(600/20));     //V
    unsigned long cy = (unsigned long) (2*(600/20));		
	
	//
	// Parent window.
	//
	
	if ( (void *) parent_window == NULL )
	{
	    pWnd = (void *) gui->screen;
	
	} else {
	    pWnd = (void *) parent_window;
	};

	//
	// Type.
	//
	
	switch (type)
	{
	    case 1:
	        hWnd = (void *) CreateWindow ( 3, 0, 0, title, 
			                    x, y, cx, cy, 
							    pWnd, 0, 0, COLOR_WINDOW ); 
		    break;
			
	    case 2:
	        hWnd = (void *) CreateWindow ( 3, 0, 0, "DialogBox2", 
			                   x, y, cx, cy, 
							   pWnd, 0, 0, COLOR_WINDOW ); 
	        break;
			
	    case 3:
	        hWnd = (void *) CreateWindow ( 3, 0, 0, "DialogBox3", 
			                   x, y, cx, cy, 
							   pWnd, 0, 0, COLOR_WINDOW ); 
	        break;
			
	    case 4:
	        hWnd = (void *) CreateWindow ( 3, 0, 0, "DialogBox4", 
			                   x, y, cx, cy, 
							   pWnd, 0, 0, COLOR_WINDOW ); 
	        break;
			
		default:
	        //hWnd = (void*) CreateWindow( 3, 0, 0, "Error", 
			//                    x, y, cx, cy, pWnd, 0, 0, COLOR_WINDOW); 
		    break;
	};
	
	// Register.
	
	if( (void *) hWnd == NULL)
	{
	    printf("DialogBox: hWnd\n");
		die();
	
	} else {
		
		RegisterWindow(hWnd);
		set_active_window(hWnd);
	};
	
	//
    // String. 
	//
	
	//(@todo: Get client area rect).
//d_string:

	draw_text ( hWnd, 1*(cx/16), 4*(cy/16), COLOR_WINDOWTEXT, string );

	//
    // Button. 
	//
	
    // (@todo: Get client area rect).
//d_button:
	 
	bWnd = (void *) CreateWindow ( 11, 0, 0, "Close", 
	                    2*(cx/6), 3*(cy/6), cx/4, cy/6, 
						hWnd, 0, 0, COLOR_BUTTONFACE );
								 
	if ( (void *) bWnd == NULL)
	{
	    printf("DialogBox error: bWnd\n");
		die();
		
	} else {
		
	    RegisterWindow(bWnd);
        SetFocus(bWnd);		
	};		
	
	//
	// Continua ...
	//

	SetProcedure ( (unsigned long) &DialogBoxProcedure );	
	
//done:

    refresh_screen();	
    
	//return;
	
*/

};




/*
 * DialogBoxProcedure:
 *     O procedimento padr�o de dialog box.
 *
 */																
unsigned long DialogBoxProcedure( struct window_d *window, 
                                  int msg, 
								  unsigned long long1, 
								  unsigned long long2 ) 
{
	
	
/*	
	    ### cancelada. Essa rotina agora est� na API em user mode. deletar essa fun��o ###
		
	//if (msg < 0)
	//    return 0;
	
    switch (msg)
	{	
        case MSG_KEYDOWN:
            switch(long1)
            {
                case VK_ESCAPE:
                   //KillFocus(window);				
				    SetProcedure( (unsigned long) &system_procedure);
                   	return (unsigned long) 0; 			   
				    break;
				   
                default:
                    break; 
            };
        break;
		
        case MSG_SYSKEYDOWN:                 
            switch(long1)	       
            {   
			
				//Start
				case VK_F1:
				    StatusBar(window,"Status Bar","F1");
                    break;
					
				//Reboot	
                case VK_F2:
                    break;
					
				case VK_F3:
                    break;					

				case VK_F4:
                    break;					

				//case VK_F5:
                //    break;	
					
                //
                // Testes: Install, format, create volume, delete volume, files operations...
                //
				
				default:
				    //SetProcedure( (unsigned long) &system_procedure);
				    break;
		    };              
        break;
		
		default:
		    //Nothing for now.
		    break;
	};
	
//done:

*/

	return (unsigned long) 0;
};


/*
int dialogboxInit()
{}
*/

//
// End.
//

