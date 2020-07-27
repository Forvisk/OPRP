# GNU Makefile
# Revisão: ago/2019

CC = gcc
CCFLAGS = -Wall -O3
LDFLAGS =
TARGET =  gmat help mainSimples mainThread mainBloco main

all: $(TARGET)

%.o: %.c
	$(CC) $(CCFLAGS) -c $<

%: %.o
	$(CC) $(LDFLAGS) $^ -o $@

mainSimples: AZJ_main.c matrizv3.o toolsv3.o matriz-operacoesvMine_simples.o
		$(CC) $(CCFLAGS) matriz-operacoesvMine_simples.o matrizv3.o toolsv3.o AZJ_main.c -o $@ $(LDFLAGS)

gmat: matrizv3.o toolsv3.o gera_matrizv3.c
		$(CC) $(CCFLAGS) matrizv3.o toolsv3.o gera_matrizv3.c -o $@ $(LDFLAGS)

mainBloco: AZJ_main.c matrizv3.o toolsv3.o matriz-operacoesvMine.o
		$(CC) $(CCFLAGS) matriz-operacoesvMine.o matrizv3.o toolsv3.o AZJ_main.c -o $@ $(LDFLAGS)

mainThread: AZJ_main.c matrizv3.o toolsv3.o matriz-operacoesvThread.o
		$(CC) $(CCFLAGS) matriz-operacoesvThread.o matrizv3.o toolsv3.o AZJ_main.c -o $@ $(LDFLAGS)

main: AZJ_main.c matrizv3.o toolsv3.o matriz-operacoesvThread.o
		$(CC) $(CCFLAGS) matriz-operacoesvThread.o matrizv3.o toolsv3.o AZJ_main.c -o $@ $(LDFLAGS)


help:
	@echo
	@echo
	@echo "####### Exemplo de Execução #######"
	@echo "./mainBloco mat_a3x4.example mat_b4x3.example"
	@echo "./mainSimples mat_a3x4.example mat_b4x3.example"
	@echo "./mainThread mat_a3x4.example mat_b4x3.example"

clean:
	rm -f *.o *~ $(TARGET) *.map *.result