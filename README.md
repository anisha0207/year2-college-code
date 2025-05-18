# CSE109 - Systems Software - Spring 2025

# Homework 4 - Implementing a Protocol: Pack109

**⏰ Due Date: 5/2/2025 EOD**

## Instructions 

**Read thoroughly before starting your project:**

1. Fork this repository into your CSE109 project namespace. [Instructions](https://docs.gitlab.com/ee/workflow/forking_workflow.html#creating-a-fork)
2. Clone your newly forked repository onto your development machine. [Instructions](https://docs.gitlab.com/ee/gitlab-basics/start-using-git.html#clone-a-repository) 
3. As you are writing code you should commit patches along the way. *i.e.* don't just submit all your code in one big commit when you're all done. Commit your progress as you work. 

**💥IMPORTANT: You must commit frequently as you work on your project. As a general rule try to make at least one commit per function you implement.**

4. When you've committed all of your work, there's nothing left to do to submit the assignment.

## Assignment Background

Imagine talking over a 1-way communication channel. 

1. Tune to pre-agreed frequency
2. The sender starts with their name or ID. e.g. “Alice to Bob…”
3. After each transmission let go of transmit button and wait for response. 
  3.1 After timeout period, retransmit
4. Upon recipt of response, transmit message. Say "over" to indicate end of message. e.g: “meet at the park at 5. Over.”
5. Wait for response as in step 3. Continue until transmission is over.
6. Say "Over and out" to indicate there will be no further transmissions.

Protocols like this allow for reliable, clear, orderly communications over a channel with heavy limitations (one-way, broadcast). 

We use protocols all the time in computer systems to bring these properties to computer communication. You may have encountered such protocols in your own life such as HTTP - the Hypertext Transfer Protocol, which is used to transfer data over the web. Or TCP, the Transmission Control Protocol, which is used to ensure reliable communication over the internet. Or FTP, the File Transfer Protocol, which is used to transfer files over the internet.

In this assignment we are going to implement a protocol called Pack109, which is designed for this class and can be used to transmit objects that we write in C++ over a network. It involves a process of "serialization" and "deserialization" which we will discuss here.

### Serialization / Deserialization

This conversion process is known as **serialization** -- the act of transforming in-memory data structures or objects into a format (typically a linear stream of bytes) that can be stored or transmitted and later reconstructed. Serialization has been a fundamental concept in computer science since the early days of distributed systems, when developers needed a way to persist objects or transfer them over the network while preserving their structure and type.

The complementary process, **deserialization**, reconstructs the original data structure from a sequence of bytes, making it usable again within the program's memory. Together, these processes are essential not just for network communication, but also for saving application state, inter-process communication (IPC), and implementing remote procedure calls (RPC).

#### An Example - Serializing a struct

Let's consider this struct in C++, although the idea here is generally applicable to languages and data structures.

```c++
struct Person {
    std::string name; // e.g., "Bob"
    char age;         // e.g., 30
    float height;     // e.g., 1.75f
};
```

For this example assume:

- Architecture: 64-bit little-endian
- Strings are serialized as:
    - 1 byte for length
    - N bytes for characters
- float values follow IEEE 754 format (4 bytes)
- char is 1 byte

We're only serializing data, not the memory layout (so no raw pointers or padding). Meaning that the original struct might look like this in C++ memory:

```
| ptr to "Bob" | age (0x1E) | padding | height (1.75f) |
|   8 bytes    |   1 byte   | 3 bytes |   4 bytes      |
```

But we do not capture this in the serialization. Therefore the recipient might serialize the stuct to a different memory layout, but it should nevertheless have the same data as the original.


To serialize the struct, we have to serialize each of the fields of the struct:

1. name = "Bob"

```
Length = 3 → 0x03
Characters:
'B' → 0x42
'o' → 0x6F
'b' → 0x62
```

2. age = 30

```
ASCII value of 30 → 0x1E
```

3. height = 1.75f

```
In IEEE 754 (32-bit float), 1.75f = 0x3FE00000
In little-endian order: 00 00 E0 3F
```

So the final serialized struct would be 

```
[03 42 6F 62 1E 00 00 E0 3F]
```

Which again means:

```
0x03	        Name length (3)
0x42 6F 62	    "Bob"
0x1E	        Age = 30
0x00 00 E0 3F	Height = 1.75f
```

We could then send this over the network or write it to a file. When we receive this data, we can deserialize by apply the serialization routine backwards, reading the bytes in the same order and reconstructing the original struct.

But how does the recipient know how to deserialize this struct into the correct Person object on its side? That's where the protocol comes in; the recipient should know ahead of time that these byte represent a person struct, and it should know what all the bytes mean. Meaning the recipient should know ahead of time, according to the protocol:

```
 Name len    Age
 \/          \/
[03 42 6F 62 1E 00 00 E0 3F]
    \ Name /    \  Height /     
```

But this protocol will only work for Person data structures. If we want to serialize/deserialize general structs and objects, we must define a protocol that can be used for any data structure. This is what Pack109 does.

In this assignment, you will implement hte Pack109 protocol for C++, by writing a set of functions that convert data types and data structures in C++ into a vector of bytes (serialization, or "ser"). Additionally, you will write a set of functions that reverse this process, converting vectors of bytes back into their original data types or data structures (deserialization, or "de").

### Deliverables

The deliverables for this assignment are:

- serde function implementations
- unit tests for all implemented functions

#### Functions

There are a number of serialization and deserialization functions stubbed out in `include/pack109.hpp` that you are to implement in `src/lib.cpp`. I've implemented 4 of them for you as examples. You are to implement the rest. In addition, you are to write a test for each of them in `test/test.cpp`. I've given you examples of how to write these tests for the 4 functions I've implemented. One of the tests fails intentionally (fix it). I've also provided a function to print the byte vector contents. It may seem like a lot of functions at first, but many of them are similar with the only difference being an adjustment to the number of times a loop iterates

To run the tests, do `make test` and then run the test program in `build/bin/release/test`.

**REMEMBER** you must make at least one commit per function.

#### Tests

When you are done, you should have at least the following tests for all of the functions you have implemented. You should also write a continuous integration (CI) script that runs all of your tests against your codebase every time you commit to your project. At the end, you should have at least 26 tests, but probably more.

Make sure to test edge cases, including but not limited to:

- What happens when the transmission is malformed?
- What happens when there are missing sections or the lenghts are off?
- What happens for very large data?
- What happens for very small data, or empty data?

### Pack109 Object Serialization Format

Pack109 is a binary object serialization format that flattens a number of different datatypes into a format suitable for transmission over a channel.

#### Overview

In the table below I've listed the 16 available objects in the Pack109 serialization format.

| Type                    | Tag (Hex) | 
| ----------------------- | --------- |
| Bool (true)             |   0xa0    |
| Bool (false)            |   0xa1    |
| Unsigned 8-bit Integer  |   0xa2    |
| Unsigned 32-bit Integer |   0xa3    |
| Unsigned 64-bit Integer |   0xa4    |
| Signed 8-bit Integer    |   0xa5    |
| Signed 32-bit Integer   |   0xa6    |
| Signed 64-bit Integer   |   0xa7    |
| 32-bit Floating Number  |   0xa8    |
| 64-bit Floating Number  |   0xa9    |
| 8-bit String            |   0xaa    |
| 16-bit String           |   0xab    |
| 8-bit Array             |   0xac    |
| 16-bit Array            |   0xad    |
| 8-bit Map               |   0xae    |
| 16-bit Map              |   0xaf    |

#### Notation Key

```
one byte:
┌────────┐
│        │
└────────┘

a variable number of bytes:
╔════════╗
║        ║
╚════════╝

variable number of objects stored in Pack109 format:
╭╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╮
┆                 ┆
╰╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╯
```

#### Bool

Bool is for serializing `true` and `false` boolean values. Although we can represent these values in one bit in C and C++, we have to use 8 bits to represent them in our serialized format.

```
true:
┌────────┐
│  0xa0  │
└────────┘ 

false:
┌────────┐
│  0xa1  │
└────────┘ 
```

#### Integers

Integers are for storing signed and unsigned `chars`, `ints`, and `longs`.

```
u8 stores a 8-bit unsigned integer
┌────────┬────────┐
│  0xa2  │ZZZZZZZZ│
└────────┴────────┘ 

u32 stores a 32-bit big-endian unsigned integer
┌────────┬────────┬────────┬────────┬────────┐
│  0xa3  │ZZZZZZZZ│ZZZZZZZZ│ZZZZZZZZ│ZZZZZZZZ│
└────────┴────────┴────────┴────────┴────────┘ 

u64 stores a 64-bit big-endian unsigned integer
┌────────┬────────┬────────┬────────┬────────┬────────┬────────┬────────┬────────┐
│  0xa4  │ZZZZZZZZ│ZZZZZZZZ│ZZZZZZZZ│ZZZZZZZZ│ZZZZZZZZ│ZZZZZZZZ│ZZZZZZZZ│ZZZZZZZZ│
└────────┴────────┴────────┴────────┴────────┴────────┴────────┴────────┴────────┘ 

i8 stores a 8-bit signed integer
┌────────┬────────┐
│  0xa5  │ZZZZZZZZ│
└────────┴────────┘ 

i32 stores a 32-bit big-endian signed integer
┌────────┬────────┬────────┬────────┬────────┐
│  0xa6  │ZZZZZZZZ│ZZZZZZZZ│ZZZZZZZZ│ZZZZZZZZ│
└────────┴────────┴────────┴────────┴────────┘


i64 stores a 64-bit big-endian signed integer
┌────────┬────────┬────────┬────────┬────────┬────────┬────────┬────────┬────────┐
│  0xa7  │ZZZZZZZZ│ZZZZZZZZ│ZZZZZZZZ│ZZZZZZZZ│ZZZZZZZZ│ZZZZZZZZ│ZZZZZZZZ│ZZZZZZZZ│
└────────┴────────┴────────┴────────┴────────┴────────┴────────┴────────┴────────┘

```

#### Floats

Floats are for serializing big-endian IEEE 754 single and double precision floating point numbers like `floats` and `doubles`. Extension of precision from single-precision to double-precision does not lose precision.

```
f32 stores a single-width floating point number
┌────────┬────────┬────────┬────────┬────────┐
│  0xa8  │ZZZZZZZZ│ZZZZZZZZ│ZZZZZZZZ│ZZZZZZZZ│
└────────┴────────┴────────┴────────┴────────┘

f64 stores a double-width floating point number
┌────────┬────────┬────────┬────────┬────────┬────────┬────────┬────────┬────────┐
│  0xa9  │ZZZZZZZZ│ZZZZZZZZ│ZZZZZZZZ│ZZZZZZZZ│ZZZZZZZZ│ZZZZZZZZ│ZZZZZZZZ│ZZZZZZZZ│
└────────┴────────┴────────┴────────┴────────┴────────┴────────┴────────┴────────┘
```

#### Strings

Strings are for serializing strings and c-strings. If you want to serialize an array of bytes that aren't meant to represent characters, use an Array instead (see below).

```
s8 stores a byte array whose length is up to (2^8)-1 bytes:
┌────────┬────────┬════════╗
│  0xaa  │YYYYYYYY│  data  ║
└────────┴────────┴════════╝

YYYYYYYY is a 8-bit unsigned integer which represents the length of the data

s16 stores a byte array whose length is up to (2^16)-1 bytes:
┌────────┬────────┬────────┬════════╗
│  0xab  │ZZZZZZZZ│ZZZZZZZZ│  data  ║
└────────┴────────┴────────┴════════╝

ZZZZZZZZZZZZZZZZ is a 16-bit unsigned integer which represents the length of the data
```

#### Arrays

Arrays are for serializing arrays of homogenous datatypes. Each object in the serialized array should be one of the other objects in the Pack109 spec. For example, you can serialize an array of u8s, an array of strings, or even an array of arrays.

```
a8 stores an array whose length is up to (2^8)-1 elements:
┌────────┬────────┬╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╮
│  0xac  │YYYYYYYY│    N objects    ┆
└────────┴────────┴╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╯

YYYYYYYY is a 8-bit unsigned integer which represents the length of the data

a16 stores an array whose length is up to (2^16)-1 elements:
┌────────┬────────┬────────┬╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╮
│  0xad  │ZZZZZZZZ│ZZZZZZZZ│    N objects    ┆
└────────┴────────┴────────┴╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╯

ZZZZZZZZZZZZZZZZ is a 16-bit unsigned integer which represents the length of the data
```

#### Maps

Maps are for serializing datastructures that can be represented by key-value pairs. This includes associative arrays, structs, and objects.

```
m8 stores a map whose length is upto (2^8)-1 elements
┌────────┬────────┬╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╮
│  0xae  │YYYYYYYY│   N*2 objects   ┆
└────────┴────────┴╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╯

YYYYYYYY is a 8-bit unsigned integer which represents the length of the data

m16 stores a map whose length is upto (2^16)-1 elements
┌────────┬────────┬────────┬╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╮
│  0xaf  │ZZZZZZZZ│ZZZZZZZZ│   N*2 objects   ┆
└────────┴────────┴────────┴╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╯

ZZZZZZZZZZZZZZZZ is a 16-bit unsigned integer which represents the length of the data
```

For example, consider the following struct:

```
struct Person {
  char age;
  float height;
  string name;
}
```

You can serialize this into a map of maps. Let's say we have the following values in our struct:

```
struct Person ann = { age: 10, height: 3.4, name: "Ann" };
```

We could serialize this into the following byte vector:

```
┌─────────────────────────────────────────────────────────────────────────────┐
│0xae   // map tag                                                            │
│0x01   // 1 kv pair                                                          │
├─────────────────────────────────────────────────────┬───────────┬───────────┤
│0xaa   // string8 tag                                │           │           │
│0x06   // 6 characters                               │ key       │           │
│Person // the string "Person"                        │           │           │
├─────────────────────────────────────────────────────┼───────────┤  pair 1   │
│0xae   // the value associated with the key is a map │           │           │
│0x03   // 3 kv pairs                                 │           │           │
├──────────────────────────────┬───────────┬──────────┤           │           │
│0xaa   // string8 tag         │           │          │           │           │
│0x03   // 3 characters        │ key       │          │           │           │
│age    // the string "age"    │           │          │           │           │
├──────────────────────────────┼───────────┤ pair 1   │           │           │
│0xa2   // u8 tag              │ value     │          │           │           │
│0x0a   // 10                  │           │          │           │           │
├──────────────────────────────┼───────────┼──────────┤           │           │
│0xaa   // string8 tag         │           │          │ value     │           │
│0x06   // 6 characters        │ key       │          │           │           │
│height // the string "height" │           │          │           │           │
├──────────────────────────────┼───────────┤ pair 2   │           │           │
│0xa8   // f32 tag             │ value     │          │           │           │
│3.4    // float value 3.4     │           │          │           │           │
├──────────────────────────────┼───────────┼──────────┤           │           │
│0xaa   // string8 tag         │           │          │           │           │
│0x04   // 4 characters        │ key       │          │           │           │
│name   // the string "name"   │           │          │           │           │
├──────────────────────────────┼───────────┤ pair 3   │           │           │
│0xaa   // string8 tag         │           │          │           │           │
│0x03   // 3 characters        │ value     │          │           │           │
│Ann    // the string "Ann"    │           │          │           │           │
└──────────────────────────────┴───────────┴──────────┴───────────┴───────────┘           
```

The total length of this byte vector would be 43 bytes.

## Assignment Evaluation

Deliverables:

- Source code for your protocol implementation.

- Tests for every function

- A [.gitlab-ci.yml](https://docs.gitlab.com/ci/) script that runs your code against the provided tests.

Some things to keep in mind:

- Only files under vesion control in your forked assignment repository will be graded. Local files left untracked on your computer will not be considered.

- Only code committed *and pushed* prior to the time of grading will be accepted. Locally committed but unpushed code will not be considered.

- Your assignment will be graded according to the [Programming Assignment Grading Rubric](https://drive.google.com/open?id=1V0nBt3Rz6uFMZ9mIaFioLF-48DFX0VdkbgRUDM_eIFk).
