import os
import random
import string

# Define the number of files and the length of random content
num_files = 10
content_length = 256

# Create the 'Test' folder if it doesn't exist
if not os.path.exists('Test'):
    os.makedirs('Test')

# Function to generate random alphabetic content
def generate_random_content(length):
    return ''.join(random.choices(string.ascii_letters, k=length))

# Create and write random content to files
for i in range(1, num_files + 1):
    file_path = os.path.join('Test', f'file_{i}.txt')
    with open(file_path, 'w') as file:
        file.write(generate_random_content(content_length))

print(f"Created {num_files} files with {content_length} random characters each in the 'Test' folder.")
