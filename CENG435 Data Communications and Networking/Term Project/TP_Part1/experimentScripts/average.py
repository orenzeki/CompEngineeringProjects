def avg(filename):
    file = open(filename, "r")
    a = 0
    for i in range(0,25):         # we have 25 values in each file
        a+=float(file.readline())  # read file
    print(a/25)                    # print on the console


avg("delay_exp0.txt")  # get the mean of delay_exp0.txt
avg("delay_exp1.txt")  # get the mean of delay_exp0.txt
avg("delay_exp2.txt")  # get the mean of delay_exp0.txt
avg("delay_exp3.txt")  # get the mean of delay_exp0.txt

