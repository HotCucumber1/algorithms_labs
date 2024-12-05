with open("../data/ру_дата.txt", 'w') as file:
    for i in range(30000):
        file.write(f'{i} {"абв" * 50}\n')