# pdftitle #

pdftitle employs formatting-based heuristics to determine the title of
a PDF text document. In other words, it returns an in-order
concatenation of the text with the largest font size on the first
page. Currently, it filters single words as these usually does not
convey titles. It works pretty well for most scientific papers and
this method appear to me more reliable than trusting the title
metadata field of PDFs. However, I wouldn't trust it for anything
mission critical.
