denominations = [0.01, 0.05, 0.1, 0.25, 0.5, 1]

def getChange (M, P):
    changeLeft = M - P
    changeToReturn = [0, 0, 0, 0, 0, 0]

    denominations.reverse()

    for i, denomination in enumerate(denominations):
        while True:
            if changeLeft < denomination:
                break;
            else:
                print(denomination)
                changeLeft -= denomination
                changeToReturn[i] += 1


    changeToReturn.reverse()
    return changeToReturn

print(getChange(5, 0.99))
print(getChange(3.14, 1.99))
print(getChange(3, 0.01))
print(getChange(4, 3.14))
print(getChange(0.45, 0.34))