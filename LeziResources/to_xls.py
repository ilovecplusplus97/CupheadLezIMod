import xlwt

workbook: xlwt.Workbook = xlwt.Workbook(encoding="utf-8")
worksheel: xlwt.Worksheet = workbook.add_sheet("Cuphead Localization")
infile = open("CupheadLocalization.csv", encoding="utf-8")
datas = map(lambda line: line.split("@"), infile.readlines())
for row, line in enumerate(datas):
    for col, data in enumerate(line):
        worksheel.write(row, col, data)
workbook.save("CupheadLocalization.xls")
