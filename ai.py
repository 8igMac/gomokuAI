import time
x = -1
s = 'a'
while True:
    while str(x) != s:
        with open('state_27.txt', 'r') as f:
            s = f.read()
        if s == str(2) or s == str(1):
            x = int(s)
    print(x)
    time.sleep(1)
    with open('move_27.txt', 'w') as f:
        f.write(str(x))
    x = x+2
