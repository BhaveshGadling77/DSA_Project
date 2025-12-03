# Event and Venue Handling system – Technical Documentation  

## Project Overview  
A complete console-based Event Management System implemented in C that demonstrates the effective application of advanced data structures and algorithms. The system supports event creation, modification, deletion, searching, sorting, and conflict-free venue booking with full persistence in CSV files. Every design decision is driven by performance requirements and theoretical guarantees.

## Let's get started
This document provides a comprehensive overview of the Event and Venue Handling system, developed as a part of the Event Management System DSA Project.

## Core Data Structures – Selection, Complexity & Justification

- **Singly Linked List (`EventNode *eventList`)**
  - Used for maintaining insertion order and sequential traversal of all events
  - Supports O(n) traversal for viewing, cleaning past events, and CSV rewriting
  - Enables easy node deletion during cleanup without index management
  - Chosen because event listing and iteration are frequent operations requiring natural sequential access
  - Perfect complement to the AVL tree — one structure for order, one for speed

- **AVL Tree (Self-Balancing Binary Search Tree) (`EventBST *eventTree`)**
  - Primary structure for storing events keyed by unique, increasing `eventID`
  - Guarantees O(log n) time for insertion, deletion, and search in worst case
  - The AVL tree is implemented using the following key points:
    - Height field in each node
    - Balance factor calculation (`left height – right height`)
    - All four rotations: LL rotation, RR rotation, LR and RL rotations(double rotations)
    - Bottom-up recalculation and rebalancing of height after every insert/delete
  - Chosen over plain BST to prevent O(n) degradation when IDs are inserted sequentially
  - Ideal for frequent search/modify/delete by eventID — core operations of the system

- **Hash Table with Separate Chaining (`VenueNode *venueHashTable[101]`)**
  - Stores all venues with `venueID` as key
  - Hash function: `venueID % 101` (Modulus method for better distribution)
  - Collision resolution via linked list chaining with O(1) prepend
  - Delivers average O(1) lookup time 
  - Chosen because venue lookup occurs on every add/modify event operation
  - Constant-time access ensures smooth user experience
  - Number of venues is limited, thus making it simple and effective
  - Chaining preferred over open addressing for simplicity

- **Interval Tree (Binary Search Tree on Time Intervals)**
  - Dynamically constructed inside `checkVenueAvailability()`
  - Each node stores start and end time converted to seconds since 00:00:00
  - Sorted by start time; supports efficient overlap queries
  - Handles midnight-crossing events correctly
  - Build time: O(m log m), query time: O(m) where m = number of events on that date/venue
  - Chosen over O(m²) pairwise-comparison to prevent unacceptable slowdown
  - Provides near-optimal conflict detection for real-world scheduling systems

- **Dynamic Array (via `listToArray()`)**
  - Converts linked list of events into contiguous memory
  - Enables random access required by QuickSort
  - Essential bridge between sequential and random-access paradigms

## Key Algorithms – Design & Complexity Analysis

- **AVL Tree Insertion & Deletion**
  - Full recursive implementation with post-operation height update and rebalancing
  - All four rotation cases explicitly handled
  - Guarantees worst-case O(log n) performance — critical for scalability

- **QuickSort with Function Pointer Comparators**
  - Generic implementation accepting comparison function pointer
  - Three comparators implemented:
    - `compareByDate`: year → month → day
    - `compareChronological`: date → start time (seconds)
    - `compareByID`: direct integer comparison
  - Average O(n log n), demonstrates flexible, reusable sorting logic

- **Interval Overlap Detection Algorithm**
  - Converts all times to seconds-since-midnight
  - Builds BST of existing bookings for target venue and date
  - Recursive traversal with early exit on first overlap
  - Correctly handles events that span across midnight

- **Date & Time Validation**
  - Comprehensive checks: leap year logic, month-day limits, and also makes sure that the event being organised is in the future.
  - Ensures registration deadline takes place before start time of the event, and start time is prior to end time of the event.
  - Prevents invalid states at input level.

- **Unique Event ID Generation**
  - Linear scan of events.csv to find maximum existing ID
  - Returns max + 1 → guarantees uniqueness
  - O(n) but executed infrequently

- **Past Event Cleanup**
  - Single pass over linked list using `isPastEvent()`
  - Targeted AVL deletion of expired events
  - Total complexity: O(n + k log n) where k = number of past events

## Why This Combination is Theoretically Optimal

- Fast event lookup and modification → AVL Tree (strict O(log n))
- Instant venue retrieval → Hash Table (average O(1))
- Conflict-free booking → Interval Tree (sub-quadratic overlap detection)
- Natural event listing → Linked List (O(1) iteration)
- Flexible sorting → QuickSort on array (O(n log n) average)
- Memory efficiency and cache friendliness maintained throughout
- No operation degrades to O(n) in critical paths

## Conclusion

This project provides a good demonstration of algorithmic design driven by rigorous complexity analysis. Every data structure was selected for its proven theoretical advantage in its specific role:

- AVL Tree ensures reliable logarithmic performance under all insertion orders  
- Hash Table delivers true constant-time lookups for static data  
- Interval Tree solves the classic scheduling conflict problem efficiently  
- QuickSort with comparators showcases generic algorithm design  
- Linked List provides the simplest and most natural sequential access  
