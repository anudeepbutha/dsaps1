Question 1: Seam Carving

Problem Statement:
Apply seam carving content-aware image-resizing algorithm on a given image. Take the height and width (in pixels) of the output image as inputs from the user.

Compiling: g++ 2025202017_A1_Q1.cpp -o program `pkg-config --cflags --libs opencv4`
Execution: ./program <image_name> <height> <width>

Approach:
1. Energy Calculation: Convert the given RGB image into Grey scale and used sobel(opencv) function to calculate Energy Matrix
2. Build a DP table from the Energy Matrix that was generated in step 1
3. Find the least energy in the last row/column of the DP and backtrack by choosing the minimum of three pixels in the previous row/column
4. This is the seam having the least energy, we need to remove this seam from the original image.
5. We will continue the steps until we get the image with desired dimensions.
6. After generating the reduced image is saved with the name output_image.jpg

Question 2: Trie Harder

Problem Statement:
Design an efficient spell checker using the Trie data structure which supports the functionalities mentioned below.
1. Spell Check: Check if the input string is present in the dictionary.
2. Autocomplete: Find all the words in the dictionary which begin with the given input.
3. Autocorrect: Find all the words in the dictionary which are at an edit distance (Levenshtein distance) of at most 3 from the given input

Approach:
1. Initialize a structure trie having 3 members pointer array of type trie of size 26, flag of type boolean, visited of type integer
2. Insert the input elements into Trie for each character in the input create a child and replace the specific element in the array with the pointer address.
3. Spell Check: starting from the first character of the input string we check the root pointer array for the next element, we will continue this until until we reach end of input and compare the flag variable and print the result accordingly, if the array doesn't contain the child pointer of the particular character, stop execution and print '0'
4. Autocomplete: starting from the first character of the input string we check the root pointer array for the next element, continue this until until we reach end of input string, then check the not null elements in the child pointer array and append to the string until we encounter true flag, we will push the string into the array once the process is completed print the size of the vector and elements in the vector.
5. Autocorrect: starting from first character of string we will check all the possible strings that are at a lavenshtein distance less than 3 recursively, to perform this we consider 3 cases
Insertion: Append the not null character in the current array and check for the character in the same array and increment the Levenshtein distance by 1 and append the character to the output string.
Deletion: If we don't find the character in the array, increment the input character pointer and check in the same array and increment lavenshtein distance by 1.
Replacement: We will consider the character other than the current input character and search in the child structure and increment the input pointer and increment lavenshtein distance by 1 and append the character for output string.
continue the above three steps until the Levenshtein distance less than or equal to 3 or end of the string or end of trie nodes, and print the appended string
