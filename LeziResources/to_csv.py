import xlrd

workbook: xlrd.Book = xlrd.open_workbook("CupheadLocalization.xls")
worksheel: xlrd.sheet.Sheet = workbook.sheet_by_index(0)
output = open("CupheadLocalization.csv", "w", encoding="utf-8")
for row in range(worksheel.nrows):
    print("@".join(worksheel.row_values(row)), file=output, end="")
output.close()
