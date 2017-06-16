assignment1:
	gcc -o assignment1 test_assign1_1.c storage_mgr.c dberror.c -I.
	./assignment1
Add_testcases:
	gcc -o Add_testcases testcase.c storage_mgr.c dberror.c -I.
	./Add_testcases
