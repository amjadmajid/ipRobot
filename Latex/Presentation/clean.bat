set TARGET=presentation
del /s /q /f *.aux
del /s /q /f *.log
del /s /q /f *.bak 
del /s /q /f %TARGET%.dvi 
del /s /q /f %TARGET%.pdf 
del /s /q /f %TARGET%.toc 
del /s /q /f %TARGET%.bbl 
del /s /q /f %TARGET%.blg
del /s /q /f %TARGET%.out
del /s /q /f %TARGET%.nav
del /s /q /f %TARGET%.snm
del /s /q /f %TARGET%.synctex.gz