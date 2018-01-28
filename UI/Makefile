all:
	python script.py
	sed -i -e 's/_//g' list.tex
	pdflatex -synctex=1 -interaction=nonstopmode proj.tex
	pdflatex -synctex=1 -interaction=nonstopmode proj.tex
	xdg-open proj.pdf
