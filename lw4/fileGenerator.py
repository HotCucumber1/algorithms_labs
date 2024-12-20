from random import randint


with open("works500.txt", mode="w") as inFile1:
    for i in range(1, 501):
        # fileString = f'{i} name{i} {randint(1, 10)}\n'
        fileString = f'{i} name{i} 5\n'
        inFile1.write(fileString)

with open('order500.txt', mode='w') as inFile2:
    for i in range(500, 1, -1):
        for j in range(i - 1, 0, -1):
            fileString = f'{i} {j}\n'
            inFile2.write(fileString)