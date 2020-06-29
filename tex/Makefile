LATEX = pdflatex
BIBTEX = bibtex

FILES = arnasRodriguez

all: $(FILES) softclean

.PHONY: clean softclean

softclean:
	rm -f *.aux *.log

%.pdf: %.tex
	$(LATEX) $<
	$(LATEX) $<

$(FILES) :
	$(MAKE) $@.pdf
	touch $@.tex