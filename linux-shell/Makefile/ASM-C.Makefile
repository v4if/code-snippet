#!Makefile 

CC 	=	gcc
OUT	=	out

#patsubt 模式字符串替换，在C_SRC字列中的字（一列文件名），如果结尾是'.c'，就用'.o'取代'.c'
ASM_SRC		=	$(shell find . -name "*.s")
ASM_OBJ		=	$(patsubst %.s,%.o,$(ASM_SRC))
C_SRC		=	$(shell find . -name "*.c")
C_OBJ		=	$(patsubst %.c,%.o,$(C_SRC))

# 让设置的这段shellcode所放的内存有执行的权限 	-z execstack 
# 堆栈保护 								-fno-stack-protector
# 32bit架构 								-m32
CROSS=-m32
CC_FLAGS=-c -Wall -ggdb -gstabs+ -I include

#基于依赖递归更新的编译规则
all:$(ASM_OBJ) $(C_OBJ) link 
#模式规则
#$<为所有依赖文件，$@为所有目标文件，相当于foreach处理操作
%.o:%.s
	@echo [+] compiling asm files...
	$(CC) $(CROSS) $(CC_FLAGS) $< -o $@
%.o:%.c
	@echo [+] compiling c files...
	$(CC) $(CROSS) $(CC_FLAGS) $< -o $@
link:
	@echo [+] linking...
	$(CC) $(CROSS) $(ASM_OBJ) $(C_OBJ) -o $(OUT)
	@echo "\n"[@] All By v4if [@]"\n"

#隐晦规则，伪目标文件
#-rm 某些文件出现问题，但不要管，继续做后面的事
.PHONY:run
run:
	./out

.PHONY:clean
clean:
	-rm -f $(ASM_OBJ) $(C_OBJ) $(OUT)
