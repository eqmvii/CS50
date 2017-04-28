# greedy.py
# re-implementing change counting program in python

import cs50

def main():
    dollars = -1
    while(dollars  < 0):
        print("Change owed?")
        dollars = cs50.get_float()
    change = round(dollars*100)
    minCoins = 0;
    
    while (change >= 25):
        change -=25
        minCoins +=1 
        
    while (change >= 10):
        change -= 10
        minCoins +=1 
        
    while (change >= 5):
        change -= 5
        minCoins +=1 
        
    while (change >= 1):
        change -= 1
        minCoins +=1
        
    print("Minimum coins: " + str(minCoins))
    

if __name__ == "__main__":
    main()