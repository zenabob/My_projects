"""
hvmCodeWriter.py -- Emits assembly language code for the Hack VM translator.
Skeletonized by Janet Davis March 2016
Refactored by John Stratton April 2017
Refactored by Janet Davis March 2019
Adapted to JCE course, 2020-2023b
"""

import os
from hvmCommands import *

# If debug is True,
# then the VM commands will be written as comments into the output ASM file.
debug = True



class CodeWriter(object):

    
   

    def __init__(self, outputName):
        """
        Opens 'outputName' and gets ready to write it.
        """
        self.file = open(outputName, 'w')
        self.setFileName(outputName)

        # used to generate unique labels
        self.labelNumber = 0
        self.functionName = ""

    def close(self):
        """
        Writes the terminal loop and closes the output file.
        """
        label = self._uniqueLabel()
        self._writeComment("Infinite loop")
        self._writeCode('(%s), @%s, 0;JMP' % (label, label))
        self.file.close()

    def setFileName(self, fileName):
        """
        Sets the current file name to 'fileName'.
        Restarts the local label counter.

        Strips the path and extension.  The resulting name must be a
        legal Hack Assembler identifier.
        """
        self.fileName = os.path.basename(fileName)
        self.fileName = os.path.splitext(self.fileName)[0]
        self.labelNumber = 0


    def _uniqueLabel(self):
        self.labelNumber += 1
        return "label" + str(self.labelNumber)

    def write(self, text):
        """
        Write directly to the file.
        """
        self.file.write(text)
        
    def _writeCode(self, code):
        """
        Writes Hack assembly code to the output file.
        code should be a string containing ASM commands separated by commas,
        e.g., "@10, D=D+A, @0, M=D"
        """
        code = code.replace(',', '\n').replace(' ', '')
        self.file.write(code + '\n')
    

    def _writeComment(self, comment):
        """
        Writes a comment to the output ASM file.
        """
        if (debug):
            self.file.write('    // %s\n' % comment)

    def _pushD(self):
        """
        Writes Hack assembly code to push the value from the D register
        onto the stack.
        TODO - Stage I - see Figure 7.2
        """
        compiler="@SP\n"
        compiler+="A=M\n"
        compiler+="M=D\n"
        compiler+="@SP\n"
        compiler+="M=M+1\n"
        self._writeCode(compiler)

                
       

    def _popD(self):
        """"
        Writes Hack assembly code to pop a value from the stack
        into the D register.
        TODO - Stage I - see Figure 7.2
        """
        compiler="@SP\n"
        compiler+="A=M-1\n"
        compiler+="D=M\n"
        compiler+="@SP\n"
        compiler+="M=M-1\n"
        self._writeCode(compiler)      
        

     

    def writeArithmetic(self, command):
        """
        Writes Hack assembly code for the given command.
        TODO - Stage I - see Figure 7.5
        """
        self._writeComment(command)

        if command == T_ADD:
          compiler="@SP\n"
          compiler+="A=M-1\n"
          compiler+="D=M\n"
          compiler+="@SP\n"
          compiler+="M=M-1\n"
          compiler+="@SP\n"
          compiler+="A=M-1\n"
          compiler+="A=M\n"
          compiler+="D=D+A\n"
          compiler+="@SP\n"
          compiler+="M=M-1\n"
          compiler+="@SP\n"
          compiler+="A=M\n"
          compiler+="M=D\n"
          compiler+="@SP\n"
          compiler+="M=M+1\n"
          self._writeCode(compiler)
          pass
        elif command == T_SUB:
          
          ###
          compiler="@SP\n"
          compiler+="A=M-1\n"
          compiler+="D=M\n"
          compiler+="@SP\n"
          compiler+="M=M-1\n"
          compiler+="@SP\n"
          compiler+="A=M-1\n"
          compiler+="A=M\n"
          compiler+="D=A-D\n"
          compiler+="@SP\n"
          compiler+="M=M-1\n"
          compiler+="@SP\n"
          compiler+="A=M\n"
          compiler+="M=D\n"
          compiler+="@SP\n"
          compiler+="M=M+1\n"
          self._writeCode(compiler)
          pass
        elif command == T_NEG:
           compiler="@SP\n"
           compiler+="A=M-1\n"
           compiler+="D=0\n"
           compiler+="D=D-M\n"
           compiler+="M=D\n"
           self._writeCode(compiler)

           pass
        elif command == T_EQ:
          compiler="@SP\n"
          compiler+="A=M-1\n"
          compiler+="D=M\n"
          compiler+="@SP\n"
          compiler+="M=M-1\n"
          compiler+="@SP\n"
          compiler+="A=M-1\n"
          compiler+="A=M\n"
          compiler+="D=A-D\n"
          compiler+="@SP\n"
          compiler+="M=M-1\n"
          compiler+="(lable"
          compiler+=str(self.labelNumber)
          ind=self.labelNumber+1
          compiler+=")\n"
          compiler+=" @lable"
          compiler+=str(ind)
          compiler+="\n"
          compiler+="  D;JEQ\n"
          compiler+="  D=0\n"
          compiler+="  @lable"
          ind2=self.labelNumber+2
          compiler+=str(ind2)+"\n"
          compiler+="  0;JMP\n"
          compiler+="(lable"
         
          compiler+=str(ind)
          compiler+=")\n"
          
          compiler+="  D=-1\n"
          compiler+="(lable"
          compiler+=str(ind2)
          compiler+=")\n"
          compiler+="  @SP\n"
          compiler+="  A=M\n"
          compiler+="  M=D\n"
          compiler+="  @SP\n"
          compiler+="  M=M+1\n"
          self.labelNumber=self.labelNumber+3
          print(compiler    )
          self._writeCode(compiler)
      
        elif command == T_GT:
          compiler="@SP\n"
          compiler+="A=M-1\n"
          compiler+="D=M\n"
          compiler+="@SP\n"
          compiler+="M=M-1\n"
          compiler+="@SP\n"
          compiler+="A=M-1\n"
          compiler+="A=M\n"
          compiler+="D=A-D\n"
          compiler+="@SP\n"
          compiler+="M=M-1\n"
          compiler+="(lable"
          compiler +=str(self.labelNumber) + ")\n"
          ind=self.labelNumber+1
          compiler+="  @lable"
          compiler+=str(ind)+"\n"
          ind2=self.labelNumber+2
          compiler+="  D;JGT\n"
          compiler+="  D=0\n"
          compiler+="  @lable"
          compiler+=str(ind2)+"\n"#6
          compiler+="  0;JMP\n"
          compiler+="(lable"
          compiler+=str(ind) +")\n"
          compiler+="  D=-1\n"
          compiler+="(lable"
          compiler+=str(ind2)+")\n"
          compiler+="  @SP\n"
          compiler+="  A=M\n"
          compiler+="  M=D\n"
          compiler+="  @SP\n"
          compiler+="  M=M+1\n"
          self.labelNumber=self.labelNumber+3
          self._writeCode(compiler)
          pass
        elif command == T_LT:
          compiler="@SP\n"
          compiler+="A=M-1\n"
          compiler+="D=M\n"
          compiler+="@SP\n"
          compiler+="M=M-1\n"
          compiler+="@SP\n"
          compiler+="A=M-1\n"
          compiler+="A=M\n"
          compiler+="D=D-A\n"
          compiler+="@SP\n"
          compiler+="M=M-1\n"
          compiler+="(lable"#7
          compiler+=str(self.labelNumber)+")\n"
          ind=self.labelNumber+1
          compiler+="  @lable"
          compiler+=str(ind)+"\n"
          compiler+="  D;JGT\n"
          compiler+="  D=0\n"
          ind2=self.labelNumber+2
          compiler+="  @lable"
          compiler+=str(ind2)+"\n"
          compiler+="  0;JMP\n"
          compiler+="(lable"
          compiler+=str(ind)+ ")\n"
          compiler+="  D=-1\n"
          compiler+="(lable"
          compiler+=  str(ind2)+")\n"
          compiler+="  @SP\n"
          compiler+="  A=M\n"
          compiler+="  M=D\n"
          compiler+="  @SP\n"
          compiler+="  M=M+1\n"
          self.labelNumber=self.labelNumber+3
          self._writeCode(compiler)
          pass
        elif command == T_AND:
          compiler="@SP\n"
          compiler+="A=M-1\n"
          compiler+="D=M\n"
          compiler+="@SP\n"
          compiler+="M=M-1\n"
          compiler+="@SP\n"
          compiler+="A=M-1\n"
          compiler+="A=M\n"
          compiler+="D=D&A\n"
          compiler+="@SP\n"
          compiler+="M=M-1\n"
          compiler+="@SP\n"
          compiler+="A=M\n"
          compiler+="M=D\n"
          compiler+="@SP\n"
          compiler+="M=M+1\n"
          self._writeCode(compiler)
          pass
        elif command == T_OR:
          compiler="@SP\n"
          compiler+="A=M-1\n"
          compiler+="D=M\n"
          compiler+="@SP\n"
          compiler+="M=M-1\n"
          compiler+="@SP\n"
          compiler+="A=M-1\n"
          compiler+="A=M\n"
          compiler+="D=D|A\n"
          compiler+="@SP\n"
          compiler+="M=M-1\n"
          compiler+="@SP\n"
          compiler+="A=M\n"
          compiler+="M=D\n"
          compiler+="@SP\n"
          compiler+="M=M+1\n"
          
          self._writeCode(compiler)
          pass
        elif command == T_NOT:
          
           compiler="@SP\n"
           compiler+="A=M-1\n"
           compiler+="D=!M\n"
           compiler+="M=D\n"


           self._writeCode(compiler)

           pass
           
        else:
            raise (ValueError, 'Bad arithmetic command')

    def writePushPop(self, commandType, segment, index):
        """
        Write Hack code for 'commandType' (C_PUSH or C_POP).
        'segment' (string) is the segment name.
        'index' (int) is the offset in the segment.
        e.g., for the VM instruction "push constant 5",
        segment has the value "constant" and index has the value 5.
        TODO - Stage I - push constant only
        TODO - Stage II - See Figure 7.6 and pp. 142-3
        """
        if commandType == C_PUSH:
            self._writeComment("push %s %d" % (segment, index))

            if segment == T_CONSTANT:
                compiler="@"
                compiler+=str(index)
                compiler+="\n"
                compiler+="D=A\n"
                compiler+="@SP\n"
                compiler+="A=M\n"
                compiler+="M=D\n"
                compiler+="@SP\n"
                compiler+="M=M+1\n"
                self._writeCode(compiler)

                pass
            elif segment == T_STATIC:
                compiler="@"
                compiler+=self.fileName
                compiler+="."
                compiler+=str(index)
                compiler+="\n"
                compiler+="D=M\n"
                compiler+="@SP\n"
                compiler+="A=M\n"
                compiler+="M=D\n"
                compiler+="@SP\n"
                compiler+="M=M+1\n"
                self._writeCode(compiler)
                pass
            elif segment == T_POINTER:
                compiler="@"
                word=''
                if index==0:
                    word="THIS"
                if index==1:
                    word="THAT"
                compiler+=word
                compiler+="\n"
                compiler+="D=M\n"
                compiler+="@SP\n"
                compiler+="A=M\n"
                compiler+="M=D\n"
                compiler+="@SP\n"
                compiler+="M=M+1\n"
                self._writeCode(compiler)
                pass
            elif segment == T_TEMP:
                index+=5
                compiler="@"
                compiler+=str(index)
                compiler+="\n"
                compiler+="D=M\n"
                compiler+="@SP\n"
                compiler+="A=M\n"
                compiler+="M=D\n"
                compiler+="@SP\n"
                compiler+="M=M+1\n"
                self._writeCode(compiler)
               
                pass
            else:  # argument, local, this, that
                if T_ARGUMENT==segment:
                  compiler="@"
                  compiler+=str(index)
                  compiler+="\n"
                  compiler+="D=A\n"
                  compiler+="@ARG\n"
                  compiler+="A=M+D\n"
                  compiler+="D=M\n"
                  compiler+="@SP\n"
                  compiler+="A=M\n"
                  compiler+="M=D\n"
                  compiler+="@SP\n"
                  compiler+="M=M+1\n"
                  self._writeCode(compiler)
                  
                elif segment==T_LOCAL:
                  compiler="@"
                  compiler+=str(index)
                  compiler+="\n"
                  compiler+="D=A\n"
                  compiler+="@LCL\n"
                  compiler+="A=M+D\n"
                  compiler+="D=M\n"
                  compiler+="@SP\n"
                  compiler+="A=M\n"
                  compiler+="M=D\n"
                  compiler+="@SP\n"
                  compiler+="M=M+1\n"
                
                  self._writeCode(compiler)
                 
                 
                elif segment==T_THIS:
                  compiler="@"
                  compiler+=str(index)
                  compiler+="\n"
                  compiler+="D=A\n"
                  compiler+="@THIS\n"
                  compiler+="A=M+D\n"
                  compiler+="D=M\n"
                  compiler+="@SP\n"
                  compiler+="A=M\n"
                  compiler+="M=D\n"
                  compiler+="@SP\n"
                  compiler+="M=M+1\n"
                  self._writeCode(compiler)
                elif segment==T_THAT:
                  compiler="@"
                  compiler+=str(index)
                  compiler+="\n"
                  compiler+="D=A\n"
                  compiler+="@THAT\n"
                  compiler+="A=M+D\n"
                  compiler+="D=M\n"
                  compiler+="@SP\n"
                  compiler+="A=M\n"
                  compiler+="M=D\n"
                  compiler+="@SP\n"
                  compiler+="M=M+1\n"
                  self._writeCode(compiler)
                pass

        elif commandType == C_POP:
            self._writeComment("pop %s %d" % (segment, index))

            if segment == T_STATIC:
                
                compiler="@SP\n"
                compiler+="A=M-1\n"  
                compiler+="D=M\n"
                compiler+="@"
                compiler+=self.fileName
                compiler+="."
                compiler+=str(index)
                compiler+="\n"
                compiler+="M=D\n"
                compiler+="@SP\n"
                compiler+="M=M-1\n"
                self._writeCode(compiler)
                pass
            elif segment == T_POINTER:
                word=''
                if index==0:
                   word="THIS"
                if index==1:
                    word="THAT"
                compiler="@SP\n"
                compiler+="A=M-1\n"
                
                compiler+="D=M\n"
                compiler+="@"
                compiler+=word
                compiler+="\n"
                compiler+="M=D\n"
                compiler+="@SP\n"
                compiler+="M=M-1\n"
                self._writeCode(compiler)

                pass
            elif segment == T_TEMP:
                index+=5
                compiler="@SP\n"
                compiler+="A=M-1\n"
            
                compiler+="D=M\n"
                compiler+="@"
                compiler+=str(index)
                compiler+="\n"
                compiler+="M=D\n"
                compiler+="@SP\n"
                compiler+="M=M-1\n"
                self._writeCode(compiler)

                pass
            else:  # argument, local, this, that
                if segment==T_LOCAL:
                    compiler="@SP\n"
                    compiler+="M=M-1\n"
                    compiler+="@"
                    compiler+=str(index)
                    compiler+="\nD=A\n"
                    compiler+="@LCL\n"
                    compiler+="D=D+M\n"
                    compiler+="@i\n"
                    compiler+="M=D\n"
                    compiler+="@SP\n"
                    compiler+="A=M\n"
                    compiler+="D=M\n"
                    compiler+="@i\n"
                    compiler+="A=M\n"
                    compiler+="M=D\n"
                    
                    self._writeCode(compiler)
                elif segment==T_THAT:
                    compiler="@SP\n"
                    compiler+="M=M-1\n"
                    compiler+="@"
                    compiler+=str(index)
                    compiler+="\nD=A\n"
                    compiler+="@THAT\n"
                    compiler+="D=D+M\n"
                    compiler+="@i\n"
                    compiler+="M=D\n"
                    compiler+="@SP\n"
                    compiler+="A=M\n"
                    compiler+="D=M\n"
                    compiler+="@i\n"
                    compiler+="A=M\n"
                    compiler+="M=D\n"
                    self._writeCode(compiler)
                  
                elif segment==T_THIS:
                    compiler="@SP\n"
                    compiler+="M=M-1\n"
                    compiler+="@"
                    compiler+=str(index)
                    compiler+="\nD=A\n"
                    compiler+="@THIS\n"
                    compiler+="D=D+M\n"
                    compiler+="@i\n"
                    compiler+="M=D\n"
                    compiler+="@SP\n"
                    compiler+="A=M\n"
                    compiler+="D=M\n"
                    compiler+="@i\n"
                    compiler+="A=M\n"
                    compiler+="M=D\n"
                    self._writeCode(compiler)
                elif segment==T_ARGUMENT:
                    compiler="@SP\n"
                    compiler+="M=M-1\n"
                    compiler+="@"
                    compiler+=str(index)
                    compiler+="\nD=A\n"
                    compiler+="@ARG\n"
                    compiler+="D=D+M\n"
                    compiler+="@i\n"
                    compiler+="M=D\n"
                    compiler+="@SP\n"
                    compiler+="A=M\n"
                    compiler+="D=M\n"
                    compiler+="@i\n"
                    compiler+="A=M\n"
                    compiler+="M=D\n"
                    self._writeCode(compiler)
                pass

        else:
            raise (ValueError, 'Bad push/pop command')

    # Functions below this comment are for Project 08. Ignore for Project 07.
    def writeInit(self):
        """
        Writes assembly code that effects the VM initialization,
        also called bootstrap code. This code must be placed
        at the beginning of the output file.
        See p. 165, "Bootstrap Code"
        TODO - Stage IV
        """
        self._writeComment("Init")
        compiler="@256 \n"
        compiler+="D=A \n"
        compiler+="@SP \n"
        compiler+="M=D \n"
        compiler+="@Sys.init \n"
        compiler+="0;JMP \n"
        self._writeCode(compiler)
        
        pass

    def writeLabel(self, label):
        """
        Writes assembly code that effects the label command.
        See section 8.2.1 and Figure 8.6.
        TODO - Stage III
        """
        self._writeComment("label %s" % (label))
        compiler="("+label+")\n"
        
        self._writeCode(compiler)
        pass

    def writeGoto(self, label):
        """
        Writes assembly code that effects the goto command.
        See section 8.2.1 and Figure 8.6.
        TODO - Stage III
        """
        self._writeComment("goto %s" % (label))
        compiler="@"
        compiler+=label
        compiler+="\n0;JMP\n"
        self._writeCode(compiler)
        
        pass

    def writeIf(self, label):
        """
        Writes assembly code that effects the if-goto command.
        See section 8.2.1 and Figure 8.6.
        TODO - Stage III
        """
        self._writeComment("if-goto %s" % (label))
        compiler="@SP\n"
        compiler+="M=M-1\n"
        compiler+="A=M\n"
        compiler+="D=M\n"
        compiler+="@"
        compiler+=label
        compiler+="\nD;JGT\n"
        
        self._writeCode(compiler)
        

        pass

    def writeCall(self, functionName, numArgs):
        """
        Writes assembly code that effects the call command.
        See Figures 8.5 and 8.6.
        TODO - Stage IV
        """


        
        self._writeComment("call %s %d" % (functionName, numArgs))
        
        compiler="@retAddrLabel"+str(self.labelNumber)+"\n D=A\n @SP\n A=M\n M=D\n @SP\n M=M+1\n"
        compiler+="@LCL\n D=M\n  @SP\n A=M\n M=D\n @SP\n M=M+1\n"
        compiler+="@ARG\n   D=M\n   @SP\n A=M\n M=D\n @SP\n M=M+1\n"
        compiler+="@THIS\n  D=M\n  @SP\n A=M\n M=D\n @SP\n M=M+1\n"
        compiler+="@THAT\n D=M\n   @SP\n A=M\n M=D\n @SP\n M=M+1\n"
        compiler+="@SP\n D=M\n  @"+str(numArgs)+"\nD=D-A\n @5\n D=D-A\n @ARG \n M=D\n"
        compiler+="@SP\nD=M\n@LCL\n M=D\n "
        compiler+="@"+functionName+"\n0;JMP\n"
        compiler+="(retAddrLabel"+str(self.labelNumber)+")\n"
        self.labelNumber=self.labelNumber+1
        
        self._writeCode(compiler)
        pass

    def writeReturn(self):
        """
        Writes assembly code that effects the return command.
        See Figure 8.5.
        TODO - Stage IV
        """
        self._writeComment("return")
       
        compiler="@LCL\nD=M\n@endframe"+str(self.labelNumber)+"\nM=D\n"#endframe=LCL
        compiler+="@endframe"+str(self.labelNumber)+"\n D=M\n @5\n A=D-A\n D=M\n @returnadd"+str(self.labelNumber)+"\n M=D\n "#retrunadd
        compiler+="@SP\n A=M-1\n D=M\n @ARG\n A=M\n M=D\n "#pop() 
        compiler+="@ARG\n  D=M\n @SP\n  M=D+1\n"#sp+1
        compiler+="@endframe"+str(self.labelNumber)+"\n A=M-1\n D=M\n @THAT\n  M=D\n  "#that
        compiler+="@endframe"+str(self.labelNumber)+"\n D=M\n @2\n  A=D-A\n D=M\n @THIS\n  M=D\n "#this
        compiler+="@endframe"+str(self.labelNumber)+"\n D=M\n @3\n  A=D-A\n D=M\n @ARG\n  M=D\n "#ARG
        compiler+="@endframe"+str(self.labelNumber)+"\n D=M\n @4\n  A=D-A\n D=M\n @LCL\n   M=D\n "#LCL
        compiler+="@returnadd"+str(self.labelNumber)+"\n  A=M\n0;JMP\n "#returadd 
        self.labelNumber=self.labelNumber+1

        self._writeCode(compiler)
        pass

    def writeFunction(self, functionName, numLocals):
        """
        Writes assembly code that effects the call command.
        See Figures 8.5 and 8.6.
        TODO - Stage IV
        """
        self._writeComment("function %s %d" % (functionName, numLocals))
        self.functionName = functionName  # For local labels
        compiler="("+functionName+")\n"
       
        compiler+="@"+str(numLocals)+"\n"
        compiler+="D=A\n"
        compiler+="(Loop"+str(self.labelNumber)+")\n"
        compiler+="@SP\n"
        compiler+="A=M\n"
        compiler+="M=0\n"
        compiler+="@SP\n"
        compiler+="M=M+1\n"
        compiler+="D=D-1\n"
        compiler+="@Loop"+str(self.labelNumber)+"\n"
        compiler+="D;JGT\n"
        compiler+="(return"+str(self.labelNumber)+")\n"#
       
        self.labelNumber=self.labelNumber+1
        self._writeCode(compiler)
        pass

