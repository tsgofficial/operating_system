import threading

# -------------------------------------------------------
# Global data
# -------------------------------------------------------

# Unsorted array
unsorted_array = [12, 4, 7, 9, 1, 15, 3, 8]

# Second array to store final merged result
merged_array = [0] * len(unsorted_array)

# Size and midpoint index
size = len(unsorted_array)
mid = size // 2


# -------------------------------------------------------
# Sorting thread function
# -------------------------------------------------------
def sort_sublist(start_index, end_index):
    global unsorted_array
    # Sort the sublist in place
    sublist = unsorted_array[start_index:end_index]
    sublist.sort()
    unsorted_array[start_index:end_index] = sublist


# -------------------------------------------------------
# Merging thread function
# -------------------------------------------------------
def merge_sublists():
    global unsorted_array, merged_array, mid

    i = 0         # pointer for left half
    j = mid       # pointer for right half
    k = 0         # pointer for merged_array

    # Merge the two sorted halves
    while i < mid and j < len(unsorted_array):
        if unsorted_array[i] <= unsorted_array[j]:
            merged_array[k] = unsorted_array[i]
            i += 1
        else:
            merged_array[k] = unsorted_array[j]
            j += 1
        k += 1

    # Copy remaining elements from left half
    while i < mid:
        merged_array[k] = unsorted_array[i]
        i += 1
        k += 1

    # Copy remaining elements from right half
    while j < len(unsorted_array):
        merged_array[k] = unsorted_array[j]
        j += 1
        k += 1


# -------------------------------------------------------
# Main execution (parent thread)
# -------------------------------------------------------
if __name__ == "__main__":

    print("Original array:", unsorted_array)

    # Create sorting threads
    thread1 = threading.Thread(target=sort_sublist, args=(0, mid))
    thread2 = threading.Thread(target=sort_sublist, args=(mid, size))

    # Start sorting threads
    thread1.start()
    thread2.start()

    # Wait for both sorting threads to finish
    thread1.join()
    thread2.join()

    print("After sorting halves:", unsorted_array)

    # Create and start merge thread
    merge_thread = threading.Thread(target=merge_sublists)
    merge_thread.start()

    # Wait for merge thread to finish
    merge_thread.join()

    print("Final merged sorted array:", merged_array)

