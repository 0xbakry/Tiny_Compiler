# Compiler_Scanner
Compiler scanner to extract tokens from source codes 

INPUT: Source code.                                                                                                                                                       
OUTPUT:                                                                                                                                                                 
        1- The original token (ex: if, x, 4, etc..).                                                                                                                    
        2- The token (ex: IF, IDENTIFIER, NUM, etc..).                                                                                                                  
        3- The number of the line in which it appeared.                                                                                                                   
    
# Compiler_Parser
INPUT: file of tokens.                                                                                                                                                  
OUTPUT:                                                                                                                                                                         
        1- Parse Tree
        
                [READ][x]
                [if]
                   [oper][LESS_THAN]
                      [NUM][0]
                      [ID][x]

                   [ASSIGN][fact]
                      [Oper][SEMI_COLON]
                         [NUM][1]
                         [REPEAT][repeat]
                   [ASSIGN][fact]
                      [Oper][TIMES]
                         [ID][fact]
                         [ID][x]
                   [ASSIGN][x]
                      [Oper][MINUS]
                         [ID][x]
                         [NUM][1]
                   [ASSIGN][x]
                   [Oper][SEMI_COLON]
                      [NUM][0]
                      [WRITE][write]
                      
**This project will be constantly updated**
