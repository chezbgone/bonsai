# author :  samtenka
# change :  2020-05-22 
# create :  2020-03-15
# descrp :  Compilation commands for bonsai tests. 
# to use :  Run the following commands (replacing `logic_compress `with the
#           desired test):
#
#               make test_logic_compress
#               ./moo 
#
#           We briefly describe the available tests:
#
#               logic_compress  --- propose and score new logical combo.s based
#                                   on decision trees trained with toy arc-like
#                                   data 
#
#               lambda_compress --- propose new abstractions by inverse beta
#                                   substitution from a small, manually written
#                                   artificial library 
#
#               enumerate       --- enumerate from (the arcdsl's) pcfg up to a
#                                   given threshold
#
#               pool            --- verify that memory pools work correctly on 
#                                   simple fake stress test 

test_logic_compress:
	gcc alloc/pool.c \
		containers/*.c \
		data_scripts/toy_data.c \
		dectree/dataset.c \
		dectree/dectree.c \
		dsl/enumerator.c \
		dsl/interpreter.c \
		lambda/concept_table.c \
		lambda/lambda.c \
		lambda/test_train.c \
		lambda/type.c \
	 	utils/*.c \
		-o moo.o

test_lambda_compress:
	gcc alloc/pool.c \
		data_scripts/toy_data.c \
		lambda/concept_table.c \
		lambda/extract.c \
		lambda/lambda.c \
		lambda/test_lambda.c \
		lambda/type.c \
	 	utils/*.c \
		-o moo.o

test_enumerate:
	gcc alloc/pool.c \
		containers/*.c \
		data_scripts/toy_data.c \
		dectree/dataset.c \
		dectree/dectree.c \
		dsl/enumerator.c \
		dsl/interpreter.c \
		lambda/concept_table.c \
		lambda/lambda.c \
		dsl/test_enumerate.c \
		lambda/type.c \
	 	utils/*.c \
		-o moo.o

test_pool:
	gcc utils/colors.c \
		alloc/pool.c \
		alloc/test_pool.c \
		-o moo.o
