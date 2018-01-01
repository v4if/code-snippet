#!Makefile 

CC 		=	javac
FILE	=	Main

#patsubt 模式字符串替换，在C_SRC字列中的字（一列文件名），如果结尾是'.c'，就用'.o'取代'.c'
JAVA_SRC		=	$(shell find . -name "*.java")
JAVA_OBJ		=	$(patsubst %.java,%.class,$(JAVA_SRC))

#基于依赖递归更新的编译规则
all:$(JAVA_OBJ) link 
#模式规则
#$<为所有依赖文件，$@为所有目标文件，相当于foreach处理操作
%.class:%.java
	@echo [+] compiling java files...
	$(CC) $< 
link:
	@echo "\n"[@] All By v4if [@]"\n"

#隐晦规则，伪目标文件
#-rm 某些文件出现问题，但不要管，继续做后面的事
.PHONY:run
run:
	java $(FILE)

.PHONY:clean
clean:
	-rm -f $(JAVA_OBJ) 
