#!Makefile 

CC 	=	gcc
OUT	=	out

#patsubt ģʽ�ַ����滻����C_SRC�����е��֣�һ���ļ������������β��'.c'������'.o'ȡ��'.c'
ASM_SRC		=	$(shell find . -name "*.s")
ASM_OBJ		=	$(patsubst %.s,%.o,$(ASM_SRC))
C_SRC		=	$(shell find . -name "*.c")
C_OBJ		=	$(patsubst %.c,%.o,$(C_SRC))

# �����õ����shellcode���ŵ��ڴ���ִ�е�Ȩ�� 	-z execstack 
# ��ջ���� 								-fno-stack-protector
# 32bit�ܹ� 								-m32
CROSS=-m32
CC_FLAGS=-c -Wall -ggdb -gstabs+ -I include

#���������ݹ���µı������
all:$(ASM_OBJ) $(C_OBJ) link 
#ģʽ����
#$<Ϊ���������ļ���$@Ϊ����Ŀ���ļ����൱��foreach�������
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

#���޹���αĿ���ļ�
#-rm ĳЩ�ļ��������⣬����Ҫ�ܣ��������������
.PHONY:run
run:
	./out

.PHONY:clean
clean:
	-rm -f $(ASM_OBJ) $(C_OBJ) $(OUT)
