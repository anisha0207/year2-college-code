# CSE109 - Systems Software - Spring 2025

# Midterm Exam 2

⏰ **Due by: 4/11/2025 EOD**

## Ethics Contract

**FIRST**: Please read the following carefully:

- I am the sole author of the content within this exam unless otherwise cited.
- I am not an uncredited author of any content in any other exam.
- I will not dicsuss the exam until after the submission deadline.
- All resources I used (including text books and online references, websites, language models), are cited in this exam.
- I will not plagiarize someone else's work and turn it in as my own. If I use someone else's work in this exam, I will cite that work. Failure to cite work I used is plagiarism.
- I understand that acts of academic dishonesty may be penalized to the full extent allowed by the [Lehigh University Code of Conduct][0], including receiving a failing grade for the course. I recognize that I am responsible for understanding the provisions of the Lehigh University Code of Conduct as they relate to this academic exercise.

If you agree with the above, type your full name next to the pen emoji, along with the date. Your exam **will not be graded** without this assent.

---------------------------------------------
🖋️Anisha Dasgupta
---------------------------------------------

💥 **IMPORTANT:** When you are done, make your first commit with the commit message: `I, <your full name here>, agree to the ethics contract`.

💥 **IMPORTANT: As you are working on your midterm, commit your progress regularly.**

## Exam Prelude

This exam will test your ability to independently:

- Use the basics of the C and C++ progamming language including classes and libraries
- Implement algorithms and routines that use this data structure
- Evaluate the performance and correctness of this data structure.

You're going to implement a `HashMap` data structure, which is an associative data structure that maps keys to values.

This repository is blank except for this README file. It's your job to create this program from scratch. This time however, you will not be given explicit step-by-step instructions on *how* to do this, you are expected to demonstrate your ability to do this. You are going to lay out the directory structure, create a header file, create library source files, implement your exam. 

You can use any resources from previous assignments to aid in your completion of this exam. Just be sure to cite any work you use from another source (even if you are the author). 

You can use any libraries and functions you like to accomplish this. 

**There is an oral portion to this exam**. For this portion, I will ask you to record an explanation of your work along with your screen. For more info, see Part 2. You will not be able to pass this exam without doing this portion.

### Completeness

As a general note / tip, this exam specifies the highest expectations for your exam submission. But in general it's better to submit an exam that answers all questions partially than an exam that only answer the fisrt few quesitons fully. 

You may find in the course of doing your exam, that cutting a corner or not meeting a requirement allows your life to be easier and allows you to get past something that's blocking you, you should take that shortcut. It might cost you a couple points in functionality/robustness, but you'll earn  more points in completeness.

### Commit Policy

The exam is divided into discrete "questions", and you must make at least one commit per question to receieve full credit for the question. You can do the questions in any order, and you can go back to questions for which you have already made a commit. The commit message for each question should be "QUESTION N" where N is the number of the question.

## Part 1 - In Class

**To be taken during recitation on Tuesday or Wednesday. It will be due when you leave recitation.**

You will start this portion in recitation and you will have the whole time to take it. Recitation will be extended to 9:30PM to allow for enough time for those who have accomodations. If other accomodations are needed, see me. You will receive instructions on how to compelte that portion at recitation.

For this portion, you will extend your Homework 3 in some way. That's all I will say for now.

## Part 2 - Take Home

**To be taken at home. It will be due Friday 4/11 EOD.**

You will implement a hash map, a data structure that stores key-value pairs. Like a hash set, it allows for constant-time insertion, deletion, and lookup of values based on their associated keys. Internally, it uses a hash function to convert each key into an index in an array (called a bucket array), where the key-value pair is stored. If multiple keys hash to the same index, collisions are handled using techniques like linked lists or open addressing.

- A hash set only stores unique values (no keys), and is used to check for the presence or absence of individual items. You've implemented this already, and the equivalent C++ data structure in the STL is `std::unordered_set`.

- A hash map, on the other hand, stores (key, value) pairs and is used to associate values with keys, like a dictionary.  The equivalent C++ data structure in the STL is [`std::unordered_map`](https://en.cppreference.com/w/cpp/container/unordered_map).

### Question 1 

Lay out your project structure.
**All of these are references to the homework three project structure. It's in layout.txt file.


### Question 2

Implement a class "KeyValuePair" which will hold a key and a value. Choose appropriate data types for the keys and value. Write appropriate constructors, destructors, and member functions.

### Question 3

Implement a class "HashMap" which will be the basis for the data structure we will create. It should have the following member functions (you'll implement them in the next questions, just lay out the class for now):

- `bool insert(T key, U value);`: Inserts a key-value pair into the hash map. If the key already exists, it should update the value.
- `bool remove(T key);`: Removes a key-value pair from the hash map. If the key does not exist, it should return `false`. If it does, it should return `true`.
- `bool get(T key, U& value_out);`: Returns the value associated with the key. If the key does not exist, it should return false, otherwise it returns true, and a pointer to the value is returned in the `value_out` parameter.

### Question 4

Implement `insert()`

### Question 5

Implement `remove()`

### Question 6

Implement `get()`

## Part 3 - Code Demo and Evaluation

This is the oral portion of the exam. You will record an explanation for your program which demonstrates its implementation and functionality. You don't have to show your face but you do have to record your voice (accommodations are available upon request). You should be sure to cover the following points in your discussion:

First, demonstrate the HashMap's core functions:
  - Demonstrate `insert()`
  - Demonstrate `remove()`
  - Demonstrate `get()`

Next, demonstrate the HashMap's performance. Use a timer to measure the performance of the following operations:
  - insert/remove/get 100 items.
  - insert/remove/get 1000 items.
  - insert/remove/get 10000 items.
  
As part of your explaination, be sure to explain the following in detail:
  - How did you implement the insert function?
  - How did you implement the get function?
  - Does the performance of the hash map meet your expectations? Why or why not?
  - Make a note of your challenges and where you got stuck.

If you didn't finish the exam in is entirety, explain how you attempted to solve it and where you got stuck. This will get you at least some points. 

You can use Zoom to do this, [here is a link](https://support.zoom.com/hc/en/article?id=zm_kb&sysparm_article=KB0059856) to some instructions. You don't have to record your face, only your voice and the screen. Go through the answer and explain how you arrived there. Your goal with this question is to convince me you know what you are talking about, so I want you to do this without reading a script or written answer. Just go through line by line and explain what the program does. When you are done, upload your recording to your Lehigh Drive and add a link below. 

**⚠️IMPORTANT: Make sure you give blanket permission to the link holder to view the file**

🎥 Paste Recording Link(s) Here:

## Submission

Please submit your completed exam, which should include:

1. Your program source code
2. A recording link with permission to view granted to the link holder.
3. No need to include binary / executable files.

- Only files under version control in your forked assignment repository will be graded. Local files left untracked on your computer will not be considered.

- Only code committed *and pushed* prior to the time of grading will be accepted. Locally committed but unpushed code will not be considered.

- Your assignment will be graded according to the [Programming Assignment Grading Rubric](https://drive.google.com/open?id=1V0nBt3Rz6uFMZ9mIaFioLF-48DFX0VdkbgRUDM_eIFk).

Your submission should be organized, well-commented, and easy to understand. Remember to document any assumptions you made during the implementation process, as well as any limitations of your solution. Your final exam will be graded on the correctness, completeness, and clarity of your submission.

## Works Cited

List all sources used during the exam here.
