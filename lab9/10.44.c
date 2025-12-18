#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PAGES 1000  // The length of the page reference string
#define MAX_PAGE_NUM 10 // Page numbers range from 0 to 9

// Function to generate a random page-reference string
void generatePageReference(int *pages, int length)
{
    for (int i = 0; i < length; i++)
    {
        pages[i] = rand() % MAX_PAGE_NUM; // Random page number between 0 and 9
    }
}

// FIFO (First-In-First-Out) page replacement algorithm
int fifo(int *pages, int num_pages, int num_frames)
{
    int frames[num_frames];
    int page_faults = 0;

    // Initialize frames to -1 (indicating empty slots)
    for (int i = 0; i < num_frames; i++)
    {
        frames[i] = -1;
    }

    for (int i = 0; i < num_pages; i++)
    {
        int page = pages[i];
        int found = 0;

        // Check if the page is already in the frames
        for (int j = 0; j < num_frames; j++)
        {
            if (frames[j] == page)
            {
                found = 1;
                break;
            }
        }

        // If the page is not found, page fault occurs
        if (!found)
        {
            // Replace the oldest page in FIFO manner
            for (int j = 0; j < num_frames - 1; j++)
            {
                frames[j] = frames[j + 1];
            }
            frames[num_frames - 1] = page;
            page_faults++;
        }
    }

    return page_faults;
}

// LRU (Least Recently Used) page replacement algorithm
int lru(int *pages, int num_pages, int num_frames)
{
    int frames[num_frames];
    int page_faults = 0;

    // Initialize frames to -1 (indicating empty slots)
    for (int i = 0; i < num_frames; i++)
    {
        frames[i] = -1;
    }

    for (int i = 0; i < num_pages; i++)
    {
        int page = pages[i];
        int found = 0;

        // Check if the page is already in the frames
        for (int j = 0; j < num_frames; j++)
        {
            if (frames[j] == page)
            {
                found = 1;
                // Move the page to the end to mark it as most recently used
                for (int k = j; k < num_frames - 1; k++)
                {
                    frames[k] = frames[k + 1];
                }
                frames[num_frames - 1] = page;
                break;
            }
        }

        // If the page is not found, page fault occurs
        if (!found)
        {
            // Replace the least recently used page (first position)
            for (int j = 0; j < num_frames - 1; j++)
            {
                frames[j] = frames[j + 1];
            }
            frames[num_frames - 1] = page;
            page_faults++;
        }
    }

    return page_faults;
}

// Optimal (OPT) page replacement algorithm
int optimal(int *pages, int num_pages, int num_frames)
{
    int frames[num_frames];
    int page_faults = 0;

    // Initialize frames to -1 (indicating empty slots)
    for (int i = 0; i < num_frames; i++)
    {
        frames[i] = -1;
    }

    for (int i = 0; i < num_pages; i++)
    {
        int page = pages[i];
        int found = 0;

        // Check if the page is already in the frames
        for (int j = 0; j < num_frames; j++)
        {
            if (frames[j] == page)
            {
                found = 1;
                break;
            }
        }

        // If the page is not found, page fault occurs
        if (!found)
        {
            int farthest = -1, farthest_index = -1;

            // Find the page to replace based on future references
            for (int j = 0; j < num_frames; j++)
            {
                int next_use = -1;
                for (int k = i + 1; k < num_pages; k++)
                {
                    if (pages[k] == frames[j])
                    {
                        next_use = k;
                        break;
                    }
                }

                // If the page is not found in the future, it's the best candidate to replace
                if (next_use == -1)
                {
                    farthest_index = j;
                    break;
                }

                // Track the farthest used page
                if (next_use > farthest)
                {
                    farthest = next_use;
                    farthest_index = j;
                }
            }

            // Replace the farthest used page
            frames[farthest_index] = page;
            page_faults++;
        }
    }

    return page_faults;
}

// Main function
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <num_frames> <num_pages>\n", argv[0]);
        return 1;
    }

    int num_frames = atoi(argv[1]);
    int num_pages = atoi(argv[2]);

    if (num_frames <= 0 || num_pages <= 0)
    {
        printf("Number of frames and pages must be positive integers.\n");
        return 1;
    }

    // Seed random number generator
    srand(time(NULL));

    // Generate a random page reference string
    int pages[num_pages];
    generatePageReference(pages, num_pages);

    // Apply FIFO algorithm
    int fifo_faults = fifo(pages, num_pages, num_frames);
    printf("FIFO Page Faults: %d\n", fifo_faults);

    // Apply LRU algorithm
    int lru_faults = lru(pages, num_pages, num_frames);
    printf("LRU Page Faults: %d\n", lru_faults);

    // Apply Optimal algorithm
    int optimal_faults = optimal(pages, num_pages, num_frames);
    printf("Optimal Page Faults: %d\n", optimal_faults);

    return 0;
}
