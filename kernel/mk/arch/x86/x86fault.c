/*
 * File: x86fault.c
 * 
 *     x86 faults.
 *
 * History:
 *     2015 - Created by Fred Nora.
 */


#include <kernel.h>


extern unsigned long get_page_fault_adr (void);

//
// # Flags #
//

// Pagina��o sob demanda.
// Se essa flag estiver acionada, estamos avisando que devemos 
// continuar com o sistema pois estamos apenas alocando uma p�gina faltante.
int demand_paging_flag;

// Estamos fechando uma thread que apresentou problemas.
int thread_failed_flag;


// Estamos sinalizando que n�o h� mais o que fazer.
// Devemos parar.
int fatal_error_flag;


//Prot�tipo de fun��o interna.
void do_pagefault (void);


/*
 *****************************************
 * faults:
 *
 *    #importante:
 *    +Quem chamou isso, pois temos a inten��o de retornar,
 *     inicializando outra a mesma thread, as agora com a inclus�o da 
 *     p�gina faltante, ou reinicializar com outra thread, pois fechamos 
 *     a que estava com problemas.  
 *    Por fim ainda temos o caso quando n�o iremos retornar.
 */
 
void faults ( unsigned long number ){
	
    struct thread_d *t;
	
	// #bugbug
	// Isso n�o � necess�rio, pois j� foi feito antes.
	
	asm ("cli");
    
    kprintf ("\n *FAULTS: totalticks=%d \n\n", sys_time_ticks_total );
    
	//
	//   ## Thread ##
	//
	
	// #test
	// Filtrar current_thread.
	
	if ( current_thread < 0 )
	{
		printf ("x86fault: current_thread fail\n");		
		goto fail;
	}

	t = (void *) threadList[current_thread];
	
	if( (void *) t == NULL )
	{
		printf ("x86fault: t fail\n");		
		goto fail;
	
    }else{
        
	    // Salva o contexto se a tarefa j� esteve rodando.
	    // #bugbug
        // Devemos salvar tamb�m quando ainda n�o rodou, pois
        // podemos errar na inicializa��o da thread.
        
        /* 
        if ( ProcessorBlock.threads_counter >= 1 && 
		     t->step > 0 )
		{
            // #importante
            // pegamos o contexto de vari�veis globais e colocamos na estrutura
            // para poder exib�-los
            // Tem que salvar.
            
            printf ("faults: Saving context\n");
			save_current_context ();    
	    }
        */
        
        printf ("faults: Saving context\n");
		save_current_context ();            
        
	    printf ("Number={%d}\n", number);               

	    printf ("TID %d Step %d \n", current_thread, t->step );				
	    printf ("Running Threads %d \n", ProcessorBlock.threads_counter );        
        
        //printf ("Init Phase %d \n", KeInitPhase);
	    //printf ("logonStatus %d | guiStatus %d \n", logonStatus, guiStatus );
		
		refresh_screen (); 
	};
	
    // OP��O. 
	// KiInformation ();
	
	
	// Mostra erro de acordo com o n�mero.
	
    switch (number)
	{
	    //EXCEPTION
		case 1:
		case 3:
		    printf("EXCEPTION\n");
		    mostra_reg(current_thread);
			break;
		
		//DOUBLE FAULT
	    case 8:
			printf("DOUBLE FAULT\n");
			mostra_slot(current_thread);
		    break;

	    //STACK
	    case 12:
			printf("STACK FAULT\n");
			mostra_reg(current_thread);
		    break;

	    //GP
	    case 13:
		    printf("GP\n");
			mostra_reg(current_thread);
		    break;
		
		//PAGE FAULT
		//Obs: � o contr�rio de page hit.
	    case 14:
            printf ("PF\n");
		    do_pagefault ();
		    break;
	    
	    default:			
			printf("Default number\n");
            mostra_reg(current_thread);			
			break;
	};

	
	//
	// # Flags #
	//
	
    /*
	if( fatal_error_flag == 1 ){
        
		// We can't do anything else.
		goto done;			
	}else if ( demand_paging_flag == 1 ){
		
		// Uma p�gina faltante foi atribu�da � uma threa.
		goto tryagain;
	}else if ( thread_failed_flag == 1 ) {
		
		// Uma thread falhou, ent�o fechamos ela e vamos tentar continuar.
		goto tryagain;
	};
    */
	
    //
	// * DIE.
	//
	
done:	
    //printf("done");
    //die();
	
fail:
    //printf("# FAIL #");
    //die();	
	
tryagain:
    printf ("*HANG\n");
    //refresh_screen();
	
	// #debug
	// N�o estamos continuando por enquanto pois 
	// ainda estamos iplementando isso.
	
	die ();
}


/* Interface */
 
void KiCpuFaults ( unsigned long number ){
    
	// #todo
	// Checar a validade do argumento.
	
    faults (number);
}


/*
 ******************************
 * do_pagefault:
 *     14 PAGE FAULT.
 *     Obs: Podemos apenas fechar a thread atual e retomar o sistema.
 */
 
void do_pagefault (void){
    
	unsigned long page_fault_address;
	
	// We can't close idle thread.
	// Se a thread que apresentou problemas foi a idle.
	// uma op��o � tentar recri�-la.
	
    /*
	if ( current_thread == idle )
	{
		printf("do_pagefault: We can't close idle thread \n");
		fatal_error_flag = 1;
		return;
	};
	*/
	
	// #importante
    // Devemos ter informa��es na estrutura da thread que indiquem 
	// se todas as p�ginas foram atribu�das � ela ou n�o na hora 
	// do carregamento ...
    // Pois assim a rotina de pagefault poder� saber se deve 
	// realizar a pagina��o sob demanda ou fechar a thread.
    
	printf ("do_pagefault:\n");
	
	// #todo:
	// +Checar se a current � uma thread v�lida.
	// +Checar se a current est� esperando por pagina��o sob demanda.
	// +Pegar o endere�o.
	// +Realizar o mapeamento da p�gina faltante.
	
	//Page Fault Linear Address - PFLA.
	
	page_fault_address = (unsigned long) get_page_fault_adr ();

	printf ("Address={%x}\n", (unsigned long) page_fault_address);
	
    //
    // Mostra registradores.
    //
    
	// #bugbug
	// Precisamos mudar os nomes.
	
    mostra_reg (current_thread);
	mostra_slots ();
	
	/*
	// Se o endere�o for igual a 0 � porque o eip est� perdido.
	// Devemos fechar a thread.
	if( page_fault_address == 0 )
	{
		//tentando apenas terminar a thread, se isso for poss�vel.
		printf ("Killing thread %d \n" ,current_thread );
		kill_thread ( current_thread );
		current_thread = idle;
        thread_failed_flag = 1;
		
		return;	
	}
	*/
	
}



//
// End.
//

