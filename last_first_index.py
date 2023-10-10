transform = "TGTCCCACTAA$TTTTATATATATA"
last_column = [*transform] #Last Column
# print(last_column) 
first_column = [*transform] #First Column
first_column.sort()
# print(first_column)

encounters = {}

encounters['$'] = 0
encounters['G'] = 0
encounters['A'] = 0
encounters['C'] = 0
encounters['T'] = 0

last_index = {}
for i in range(len(last_column)):
    encounters[last_column[i]] += 1
    concatenation = last_column[i] + str(encounters[last_column[i]])
    last_column[i] = concatenation
    last_index[concatenation] = i

encounters['$'] = 0
encounters['G'] = 0
encounters['A'] = 0
encounters['C'] = 0
encounters['T'] = 0

first_index = {}
for i in range(len(first_column)):
    encounters[first_column[i]] += 1
    concatenation = first_column[i] + str(encounters[first_column[i]])
    first_column[i] = concatenation
    first_index[concatenation] = i




last_first_index = {}
for key in last_index:
    last_first_index[last_index[key]] = first_index[key]


length = len(transform)

first_original_mapping = {}
first_original_mapping[first_index['$1']] = length-1

original = ['$']
cur = last_column[last_first_index[last_index['$1']]]

first_original_mapping[first_index[cur]] = length-2

original.append(cur[:1])
for i in range(len(transform)-2):
    cur = last_column[last_first_index[last_index[cur]]]
    first_original_mapping[first_index[cur]] = length-3-i
    original.append(cur[:1]) #Slice out number
    
original.reverse()






query = "CAT"

top = 0
bottom = length-1

for i in range(1,len(query)+1):
    cur_query_char = query[-i]


    for i in range(length):
        top_set = False
        if last_column[top][:1] != cur_query_char:
            top += 1
        else:
            top_set = True
        if last_column[bottom][:1] != cur_query_char:
            bottom -= 1
        else:
            if top_set:
                break

    top = last_first_index[top]
    bottom = last_first_index[bottom]

for i in range(top, bottom+1):
    print(first_original_mapping[i],":",first_original_mapping[i]+(len(query)-1))