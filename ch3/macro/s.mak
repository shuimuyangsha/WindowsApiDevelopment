#--- �û��Զ���� ---
OUT_DIR = subdir
WINDIR = OK

all:showpath $(OUT_DIR)\showmacro.exe recurssion

#---- ��ʾ���û������� ----
showpath:
	echo	shwo environment variable macro
	echo	WINDIR=$(WINDIR)
	echo	---

#--- ��ʾ�ļ����� ---
$(OUT_DIR)\showmacro.exe:
	echo	show file name macro
	echo	$(@D)
	echo 	$(@F)
	
recurssion:
	echo ---
	$(MAKE) /N /$(MAKEFLAGS) /F s.mak showpath