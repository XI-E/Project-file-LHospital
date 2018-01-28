all:
	git -C "code" checkout master
	git -C "code" pull
	python script.py
	sed -i -e 's/_//g' list.tex
	pdflatex -synctex=1 -interaction=nonstopmode proj.tex
	pdflatex -synctex=1 -interaction=nonstopmode proj.tex
	mv proj.pdf code/proj.pdf
	git -C "code" stage proj.pdf
	git -C "code" commit -m "Updated project file"
	git -C "code" push
