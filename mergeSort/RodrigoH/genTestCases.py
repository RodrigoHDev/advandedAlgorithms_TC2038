import random

N = 1  # Change to 100, 1000, 10000, etc.

# Generate random numbers (integers and decimals)
numbers = []
for _ in range(N):
    if random.random() < 0.3:  # 30% chance of decimal
        num = round(random.uniform(-1000, 1000), 2)
    else:
        num = random.randint(-1000, 1000)
    numbers.append(num)

# Write the test case
filename = f"testCase{N}.txt"

with open(filename, "w") as f:
    f.write("//Enter number of elements to sort:\n\n")
    f.write(f"{N}\n\n")
    f.write("//Enter the elements, one per line:\n\n")

    for num in numbers:
        f.write(f"{num}\n\n")

    f.write("\n//Expected result:\n\n")
    sorted_numbers = sorted(numbers, reverse=True)

    f.write("//")
    f.write(", ".join(f"{x:.2f}" for x in sorted_numbers))
    f.write("\n")

print(f"Generated {filename}")