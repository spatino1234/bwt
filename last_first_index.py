transform = "TCCT$AA"
letters = [*transform] #Last Column
# print(letters) 
sorted_letters = [*transform] #First Column
sorted_letters.sort()
# print(sorted_letters)

encounters = {}

encounters['$'] = 0
encounters['G'] = 0
encounters['A'] = 0
encounters['C'] = 0
encounters['T'] = 0

last_index = {}
for i in range(len(letters)):
    encounters[letters[i]] += 1
    concatenation = letters[i] + str(encounters[letters[i]])
    letters[i] = concatenation
    last_index[concatenation] = i

encounters['$'] = 0
encounters['G'] = 0
encounters['A'] = 0
encounters['C'] = 0
encounters['T'] = 0

first_index = {}
for i in range(len(sorted_letters)):
    encounters[sorted_letters[i]] += 1
    concatenation = sorted_letters[i] + str(encounters[sorted_letters[i]])
    sorted_letters[i] = concatenation
    first_index[concatenation] = i

print("Last Column:", letters)
print("First Column:", sorted_letters)



last_first_index = {}
for key in last_index:
    last_first_index[last_index[key]] = first_index[key]

print(last_first_index)