import os

lines = 0
files = os.listdir(os.getcwd())

for item in files:
    extension = item.split(".")[len(item.split(".")) - 1]
    if (extension == "cpp" or extension == "h"):
        opened = open(item)
        lines += len(opened.readlines())
        opened.close()

print lines
raw_input()
