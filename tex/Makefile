LATEX = pdflatex
BIBTEX = bibtex

FILES = arnasRodriguez

all: $(FILES) softclean

.PHONY: clean softclean

clean:
	rm -f *.aux *.log *.pdf

softclean:
	rm -f *.aux *.log

%.pdf: %.tex
	$(LATEX) $<
	$(BIBTEX) $<
	$(LATEX) $<
	$(LATEX) $<

$(FILES) :
	$(MAKE) $@.pdf
