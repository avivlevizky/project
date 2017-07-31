#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Assembler.h"
#include "AuxFunctions.h"


extern Symbol ** symbol_table;               /*The symbols table*/
extern void ** instructions_table;   /* for data and instruction order*/
//void ** data_table;           /**/
extern unsigned IC;                 /*Instruction table counter*/
//unsigned DC=0;                 /*Data table counter*/
extern unsigned SC;                 /*Symbol counter*/
extern char ** ErrorsAssembler;     /*Error in the compiling*/
extern unsigned EC;                 /*Error counter*/
extern unsigned LC;                 /*Line counter*/





void allocate_check(void * p)
{
    if(!p)
    {
        fprintf(stderr,"Error to allocate new memory\n");
        exit(0);
    }
    
}





void insertNewError(char * error)
{
    if (!EC)
    {
        ErrorsAssembler=(char **)calloc(1, sizeof(char *));
        allocate_check((char **)ErrorsAssembler);            /*-------------Need to check if (char **)commands is valid------------*/

    }
    ErrorsAssembler[EC]=error;
    EC++;
    ErrorsAssembler=(char **)realloc((char **)ErrorsAssembler, (EC+1)*sizeof(char *));
    allocate_check((char *)ErrorsAssembler[EC]);

}









/*get the whole command and transfer it to linked list, if returned ans>0 then there is an error in the input otherwise the input is valid*/
void CommandLineToLinkedList()
{
    char ** commands;           /*array of strings*/
    char c;              /*char variable to iterate on content std*/
    int chars_len,i;      /*chars_len: the char length of the current word, current_line: indicate in the current number line*/
    
    LC++;
    i=0;
    c='\0';
    chars_len=0;
    commands=(char **)calloc(1, sizeof(char *));
    allocate_check((char **)commands);            /*-------------Need to check if (char **)commands is valid------------*/

    
    while((c=getchar()!=EOF)&&(c!='\n'))
    {
        if((c!=' ')&&(c!=','))
        {
            commands[i]=(char *)realloc((char *)(commands[i]), (chars_len+1)*sizeof(char));
            allocate_check((char *)commands[i]);
            commands[i][chars_len-1]=c;
            commands[i][chars_len]='\0';
            chars_len++;
        }
       else
       {
           if(!chars_len)
           {
               i++;
               commands=(char **)realloc((char **)commands, (i+1)*sizeof(char *));
               commands[i]=NULL;
               chars_len=0;
           }
          
       }
    }
    if(c=='\n')
    {
        CheckingCommand(&commands);
        free(commands);
        CommandLineToLinkedList();
    }
    else   /*if c is EOF*/
    {
        CheckingCommand(&commands);
        free(commands);
    }
}






/*checking and processing the current command line*/
void CheckingCommand(char ** commands)
{
    int flag_symbol_type;

    if (isLabel(commands[0]))
    {
        /*In the case that the first string on the current command line is a label(symbol) */
        if((isValidLabel(commands[0]))&&((flag_symbol_type=isInstruction(commands[1],0)>=0)))
        {
            /*In the case that the current label is a valid label*/
            insertSymbolToTable(commands[0],flag_symbol_type);
            insertToIT(&commands[2],flag_symbol_type);   /*the commands[2] is first operand*/
        }
    }
    else /*if the commands[0] isn't label*/
    {
        if ((flag_symbol_type=isInstruction(commands[0],1)>=0))
        {
            if (flag_symbol_type>=19)
            {
                insertSymbolToTable(commands[1],19);
            }
            else
            {
                insertToIT(&commands[1],flag_symbol_type);  /*the commands[1] is first operand*/
            }
        }
        
    }
    
}




int main() {
    IC=0;
    SC=0;
    EC=0;
    LC=0;
    
    CommandLineToLinkedList();
    if (!EC)
    {
        /*Second pass of the program*/
        
    }
    return 0;
    
    
    
}


































