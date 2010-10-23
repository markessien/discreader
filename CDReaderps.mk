
CDReaderps.dll: dlldata.obj CDReader_p.obj CDReader_i.obj
	link /dll /out:CDReaderps.dll /def:CDReaderps.def /entry:DllMain dlldata.obj CDReader_p.obj CDReader_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del CDReaderps.dll
	@del CDReaderps.lib
	@del CDReaderps.exp
	@del dlldata.obj
	@del CDReader_p.obj
	@del CDReader_i.obj
