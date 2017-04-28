import cs50

def main():
    #print("hello, world")
    print("Height: ")
    h = cs50.get_int()
    print(h)
    #print ("#" * 8)
    for i in range(h):
        spaces = h - (i + 1)
        hashes = i + 2
        print((spaces * " ") + (hashes * "#"))
    
if __name__ == "__main__":
    main()