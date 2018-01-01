#!Makefile 

CC 	=	g++
OUT	=	out

#patsubt 模式字符串替换，在C_SRC字列中的字（一列文件名），如果结尾是'.c'，就用'.o'取代'.c'
CPP_SRC		=	$(shell find . -name "*.cpp")
CPP_OBJ		=	$(patsubst %.cpp,%.o,$(CPP_SRC))

# 让设置的这段shellcode所放的内存有执行的权限 	-z execstack 
# 堆栈保护 								-fno-stack-protector
# 32bit架构 								-m32
CROSS=-m32
CC_FLAGS=-c -Wall -ggdb -gstabs+ -I include

#基于依赖递归更新的编译规则
all:$(CPP_OBJ) link 
#模式规则
#$<为所有依赖文件，$@为所有目标文件，相当于foreach处理操作
%.o:%.cpp
	@echo [+] compiling cpp files...
	$(CC) $(CROSS) $(CC_FLAGS) $< -o $@
link:
	@echo [+] linking...
	$(CC) $(CROSS) $(CPP_OBJ) -o $(OUT)
	@echo "\n"[@] All By v4if [@]"\n"

#隐晦规则，伪目标文件
#-rm 某些文件出现问题，但不要管，继续做后面的事
.PHONY:run
run:
	./out

.PHONY:clean
clean:
	-rm -f $(CPP_OBJ) $(OUT)
